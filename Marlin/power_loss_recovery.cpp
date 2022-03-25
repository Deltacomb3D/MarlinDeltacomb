/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * power_loss_recovery.cpp - Resume an SD print after power-loss
 */

#include "MarlinConfig.h"

#if ENABLED(POWER_LOSS_RECOVERY)

#include "power_loss_recovery.h"

#include "cardreader.h"
#include "configuration_store.h" // GbR: POWER_LOSS_RECOVERY TO EEPROM
#include "planner.h"
#include "printcounter.h"
#include "serial.h"
#include "temperature.h"
#include "ultralcd.h"

// Recovery data
job_recovery_info_t job_recovery_info;
JobRecoveryPhase job_recovery_phase = JOB_RECOVERY_IDLE;

// Extern
extern uint8_t active_extruder, commands_in_queue, cmd_queue_index_r;

#if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
  void job_recovery_dump_data() {
    SERIAL_PROTOCOLLNPGM("---- Job Recovery Info ----");
    SERIAL_PROTOCOLPAIR("valid_head:", int(job_recovery_info.valid_head));
    SERIAL_PROTOCOLLNPAIR(" valid_foot:", int(job_recovery_info.valid_foot));
    if (job_recovery_info.valid_head) {
      if (job_recovery_info.valid_head == job_recovery_info.valid_foot) {
        SERIAL_PROTOCOLPGM("current_position: ");
        LOOP_XYZE(i) {
          SERIAL_PROTOCOL(job_recovery_info.current_position[i]);
          if (i < E_AXIS) SERIAL_CHAR(',');
        }
        SERIAL_EOL();
        SERIAL_PROTOCOLLNPAIR("feedrate: ", job_recovery_info.feedrate);

        #if HOTENDS > 1
          SERIAL_PROTOCOLLNPAIR("active_hotend: ", int(job_recovery_info.active_hotend));
        #endif

        SERIAL_PROTOCOLPGM("target_temperature: ");
        HOTEND_LOOP() {
          SERIAL_PROTOCOL(job_recovery_info.target_temperature[e]);
          if (e < HOTENDS - 1) SERIAL_CHAR(',');
        }
        SERIAL_EOL();

        #if HAS_HEATED_BED
          SERIAL_PROTOCOLLNPAIR("target_temperature_bed: ", job_recovery_info.target_temperature_bed);
        #endif

        #if FAN_COUNT
          SERIAL_PROTOCOLPGM("fanSpeeds: ");
          for (int8_t i = 0; i < FAN_COUNT; i++) {
            SERIAL_PROTOCOL(job_recovery_info.fanSpeeds[i]);
            if (i < FAN_COUNT - 1) SERIAL_CHAR(',');
          }
          SERIAL_EOL();
        #endif

        #if HAS_LEVELING
          SERIAL_PROTOCOLPAIR("leveling: ", int(job_recovery_info.leveling));
          SERIAL_PROTOCOLLNPAIR(" fade: ", int(job_recovery_info.fade));
        #endif
        SERIAL_PROTOCOLLNPAIR("cmd_queue_index_r: ", int(job_recovery_info.cmd_queue_index_r));
        SERIAL_PROTOCOLLNPAIR("commands_in_queue: ", int(job_recovery_info.commands_in_queue));
        for (uint8_t i = 0; i < job_recovery_info.commands_in_queue; i++)
          SERIAL_PROTOCOLLNPAIR("> ", job_recovery_info.command_queue[i]);
        SERIAL_PROTOCOLLNPAIR("sd_filename: ", job_recovery_info.sd_filename);
        SERIAL_PROTOCOLLNPAIR("sdpos: ", job_recovery_info.sdpos);
        SERIAL_PROTOCOLLNPAIR("print_job_elapsed: ", job_recovery_info.print_job_elapsed);
      }
      else
        SERIAL_PROTOCOLLNPGM("INVALID DATA");
    }
    SERIAL_PROTOCOLLNPGM("---------------------------");
  }
#endif // DEBUG_POWER_LOSS_RECOVERY

/**
 * Check for a valid Print Job Recovery
 *
 * If a saved state exists, populate job_recovery_commands with
 * commands to restore the machine state and continue the file.
 */
bool job_recovery_check() {
  if (!card.cardOK) card.initsd();

  if (card.cardOK) {
    // Check pointed resume file exists
    SERIAL_ECHOLNPAIR(" file: ", job_recovery_info.sd_filename);
  
    SdFile *curDir;
    char sd_filename[MAXPATHNAMELENGTH];
    COPY(sd_filename, job_recovery_info.sd_filename);

    if (sd_filename[0] == '/') sd_filename[0] = ' ';    
    const char * const fname = card.diveToFile(curDir, sd_filename, false);

    if (!fname) {
      LCD_ALERTMESSAGEPGM(MSG_SD_FILE_NOT_FOUND);
      return false;
    }

    #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
      job_recovery_dump_data();
    #endif

    if (!job_recovery_info.valid_head || job_recovery_info.valid_head != job_recovery_info.valid_foot) {
      LCD_ALERTMESSAGEPGM("INVALID DATA");
      memset(&job_recovery_info, 0, sizeof(job_recovery_info));
      return false;
    }
  }

  else {
    LCD_ALERTMESSAGEPGM(MSG_NO_CARD);
    return false;
  }

  return true;
}

/**
 * Resume the power-loss job from the machine state
 */
bool job_recovery_resume(uint8_t index, char * command) {

  if(--index == 0) sprintf_P(command, PSTR("M109 S150"));

  // Leveling off before G92 or G28
  #if HAS_LEVELING
    if (--index == 0) strcpy_P(command, PSTR("M420 S0"));
  #endif

  #if !IS_KINEMATIC
    // Ensure Z is equal to 0
    if (--index == 0) strcpy_P(command, PSTR("G92.0 Z0"));
    // Raise Z by 2mm (we hope!)
    if (--index == 0) strcpy_P(command, PSTR("G1 Z2"));
  #endif

  // home
  if (--index == 0) strcpy_P(command, PSTR("G28 R0"
    #if ENABLED(MARLIN_DEV_MODE)
      " S"
    #endif

    // Home X and Y for Cartesian
    #if !IS_KINEMATIC
      " X Y"
    #endif
  ));

  #if HAS_HEATED_BED
    if (--index == 0) {
      const int16_t bt = job_recovery_info.target_temperature_bed;
      // Restore the bed temperature      
      if (bt) sprintf_P(command, PSTR("M190 S%i"), bt);        
    }
  #endif

  // Restore all hotend temperatures
  HOTEND_LOOP() {
    const int16_t et = job_recovery_info.target_temperature[e];
    if (et) {
      #if HOTENDS > 1
        if (--index == 0) sprintf_P(command, PSTR("T%i"), e);
      #endif
      if (--index == 0) sprintf_P(command, PSTR("M109 S%i"), et);
    }
  }

  #if HOTENDS > 1
    if (--index == 0) sprintf_P(command, PSTR("T%i"), job_recovery_info.active_hotend);
  #endif

  // Restore print cooling fan speeds
  for (uint8_t i = 0; i < FAN_COUNT; i++) {
    int16_t f = job_recovery_info.fanSpeeds[i];
    if (f) {
      if (--index == 0) sprintf_P(command, PSTR("M106 P%i S%i"), i, f);
    }
  }

  char str_X[16], str_Y[16], str_Z[16], str_E[16];

  #if HAS_LEVELING
    if ((job_recovery_info.fade || job_recovery_info.leveling)) {
      // Restore leveling state before G92 sets Z
      // This ensures the steppers correspond to the native Z
      dtostrf(job_recovery_info.fade, 1, 1, str_Z);
      if (--index == 0) sprintf_P(command, PSTR("M420 S%i Z%s"), int(job_recovery_info.leveling), str_Z);
    }
  #endif

  #if !IS_KINEMATIC
    dtostrf(job_recovery_info.current_position[Z_AXIS] + 2, 1, 3, str_Z);
  #else
    dtostrf(job_recovery_info.current_position[X_AXIS] , 1, 3, str_X);
    dtostrf(job_recovery_info.current_position[Y_AXIS] , 1, 3, str_Y);
    dtostrf(job_recovery_info.current_position[Z_AXIS] , 1, 3, str_Z);
  #endif

  dtostrf(job_recovery_info.current_position[E_CART] - PAUSE_PARK_RETRACT_LENGTH
    #if ENABLED(SAVE_EACH_CMD_MODE)
      - 5
    #endif
    , 1, 3, str_E
  );

  #if IS_KINEMATIC
    if (--index == 0) sprintf_P(command, PSTR("G1 X%s Y%s Z%s F5000"), str_X, str_Y, str_Z); // Move to Z Position
  #endif

  if (--index == 0) sprintf_P(command, PSTR("G92.0 Z%s E%s"), str_Z, str_E ); // Current Z and E

  //if (--index == 0) strcpy_P(command, PSTR("G91"));

  // purge a small quantity of filament 
  //if (--index == 0) sprintf_P(command, PSTR("G1 E%i"), PAUSE_PARK_RETRACT_LENGTH);
  if (--index == 0) strcpy_P(command, PSTR("G90"));

  uint8_t r = job_recovery_info.cmd_queue_index_r, c = job_recovery_info.commands_in_queue;
  while (c--) {
    // remove "M125" if the job recovery was saved by the pause routine
    if(strcmp(job_recovery_info.command_queue[r], "M125\0") != 0) {
      if (--index == 0) strcpy(command, job_recovery_info.command_queue[r]);
    }
    r = (r + 1) % BUFSIZE;
  }

  //if (job_recovery_info.sd_filename[0] == '/') job_recovery_info.sd_filename[0] = ' ';
  if (--index == 0) sprintf_P(command, PSTR("M23 %s"), job_recovery_info.sd_filename);
  if (--index == 0) sprintf_P(command, PSTR("M24 S%ld T%ld"), job_recovery_info.sdpos, job_recovery_info.print_job_elapsed);

  // After resuming, void the powerloss data and stop sending resume commands
  if (--index == 0) {
    job_recovery_void();
    return true;
  }

  return false;
}

/**
 * Save the current machine state to the power-loss recovery file
 */
void job_recovery_save(const bool force) {
  #if SAVE_INFO_INTERVAL_MS > 0
    static millis_t next_save_ms; // = 0;  // Init on reset
    millis_t ms = millis();
  #endif
  if (
    // Save on every command
    #if ENABLED(SAVE_EACH_CMD_MODE)
      true
    #else
      // Save if power loss pin is triggered
      #if PIN_EXISTS(POWER_LOSS)
        READ(POWER_LOSS_PIN) == POWER_LOSS_STATE ||
      #endif
      // Save if interval is elapsed
      #if SAVE_INFO_INTERVAL_MS > 0
        ELAPSED(ms, next_save_ms) ||
      #endif
      // Save on every new Z height
      #if ENABLED(SAVE_EACH_Z_HEIGHT)
        (current_position[Z_AXIS] > 0 && current_position[Z_AXIS] > job_recovery_info.current_position[Z_AXIS]) ||
      #endif
      // Force save on request
      force
    #endif
  ) {
    #if SAVE_INFO_INTERVAL_MS > 0
      next_save_ms = ms + SAVE_INFO_INTERVAL_MS;
    #endif

    // Head and foot will match if valid data was saved
    if (!++job_recovery_info.valid_head) ++job_recovery_info.valid_head; // non-zero in sequence
    job_recovery_info.valid_foot = job_recovery_info.valid_head;

    // Machine state
    if (!did_pause_print)
      COPY(job_recovery_info.current_position, current_position);
    else {
      COPY(job_recovery_info.current_position, resume_position);
      job_recovery_info.current_position[3] = resume_position[3] - PAUSE_PARK_RETRACT_LENGTH - 4; 
    }

    job_recovery_info.feedrate = feedrate_mm_s;

    #if HOTENDS > 1
      job_recovery_info.active_hotend = active_extruder;
    #endif

    COPY(job_recovery_info.target_temperature, thermalManager.target_temperature);

    #if HAS_HEATED_BED
      job_recovery_info.target_temperature_bed = thermalManager.target_temperature_bed;
    #endif

    #if FAN_COUNT
      COPY(job_recovery_info.fanSpeeds, fanSpeeds);
    #endif

    #if HAS_LEVELING
      job_recovery_info.leveling = planner.leveling_active;
      job_recovery_info.fade = (
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          planner.z_fade_height
        #else
          0
        #endif
      );
    #endif

    // Commands in the queue
    job_recovery_info.cmd_queue_index_r = cmd_queue_index_r;
    job_recovery_info.commands_in_queue = commands_in_queue;
    COPY(job_recovery_info.command_queue, command_queue);

    // Elapsed print job time
    job_recovery_info.print_job_elapsed = print_job_timer.duration();

    // If current job war resumed, dont touch the file name (avoid M23 bug)
    if(job_recovery_phase != JOB_RECOVERY_DONE)
      card.getAbsFilename(job_recovery_info.sd_filename);
    
    // SD file position
    job_recovery_info.sdpos = card.getIndex();    

    // Save to eeprom
    (void)settings.save_job_recovery_data();

    // If power-loss pin was triggered, write just once then kill
    #if PIN_EXISTS(POWER_LOSS)
      if (READ(POWER_LOSS_PIN) == POWER_LOSS_STATE) kill(PSTR(MSG_POWER_LOSS_RECOVERY));
    #endif
  }
}

void job_recovery_void() {
  #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
    SERIAL_PROTOCOLLNPGM("[Job Recovery] Void data");
  #endif
  job_recovery_info.valid_foot = 100;
  job_recovery_info.valid_head = 0;
  (void)settings.save_job_recovery_data();
  
}

#endif // POWER_LOSS_RECOVERY
