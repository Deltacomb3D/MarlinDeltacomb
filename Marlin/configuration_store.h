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

#ifndef CONFIGURATION_STORE_H
#define CONFIGURATION_STORE_H

#include "MarlinConfig.h"

class MarlinSettings {
  public:
    MarlinSettings() { }

    static uint16_t datasize();

    static void reset();
    static bool save();   // Return 'true' if data was saved
    static void save_job_recovery_data(); // GbR
    static void load_job_recovery_data(); // GbR

    FORCE_INLINE static bool init_eeprom() {
      reset();
      #if ENABLED(EEPROM_SETTINGS)
        const bool success = save();
        #if ENABLED(EEPROM_CHITCHAT)
          if (success) report();
        #endif
        return success;
      #else
        return true;
      #endif
    }

    #if ENABLED(EEPROM_SETTINGS)
      static bool load();     // Return 'true' if data was loaded ok
      static bool validate(); // Return 'true' if EEPROM data is ok

      #if ENABLED(AUTO_BED_LEVELING_UBL) // Eventually make these available if any leveling system
                                         // That can store is enabled
        static uint16_t meshes_start_index();
        FORCE_INLINE static uint16_t meshes_end_index() { return meshes_end; }
        static uint16_t calc_num_meshes();
        static int mesh_slot_offset(const int8_t slot);
        static void store_mesh(const int8_t slot);
        static void load_mesh(const int8_t slot, void * const into=NULL);

        //static void delete_mesh();    // necessary if we have a MAT
        //static void defrag_meshes();  // "
      #endif
    #else
      FORCE_INLINE
      static bool load() { reset(); report(); return true; }
    #endif

    #if DISABLED(DISABLE_M503)
      static void report(const bool forReplay=false);
    #else
      FORCE_INLINE
      static void report(const bool forReplay=false) { UNUSED(forReplay); }
    #endif

  private:
    static void postprocess();

    #if ENABLED(EEPROM_SETTINGS)

      static bool eeprom_error, validating;

      #if ENABLED(AUTO_BED_LEVELING_UBL) // Eventually make these available if any leveling system
                                         // That can store is enabled
        static constexpr uint16_t meshes_end = E2END - 128; // 128 is a placeholder for the size of the MAT; the MAT will always
                                                            // live at the very end of the eeprom

      #endif

      static bool _load();
      static void write_data(int &pos, const uint8_t *value, uint16_t size, uint16_t *crc);
      static void read_data(int &pos, uint8_t *value, uint16_t size, uint16_t *crc, const bool force=false);
      static bool size_error(const uint16_t size);
    #endif
};

extern MarlinSettings settings;

#endif // CONFIGURATION_STORE_H
