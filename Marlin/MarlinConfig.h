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

#ifndef MARLIN_CONFIG_H
#define MARLIN_CONFIG_H

/**
 * Deltacomb preconfiguration shortcut. Available setups :
 *
 * PRINTER_MODEL_DC20       - Deltacomb DC-20
 * PRINTER_MODEL_DC20HB     - Deltacomb DC-20 + Heat Bed
 * PRINTER_MODEL_DC20FC     - Deltacomb DC-20 + Flux Capacitor 2x
 * PRINTER_MODEL_DC20FCHB   - Deltacomb DC-20 + Flux Capacitor 2x + Heat Bed
 * PRINTER_MODEL_DC202E     - Deltacomb DC-20 + Dual Extruder
 * PRINTER_MODEL_DC202EHB   - Deltacomb DC-20 + Dual Extruder + Heat Bed
 * 
 * PRINTER_MODEL_DC20_V35   - Deltacomb DC-20 (v3.5)
 * PRINTER_MODEL_DC20HB_V35 - Deltacomb DC-20 + Heat Bed (v3.5)
 *
 */

#define PRINTER_MODEL_DC20 "Configuration_DC20.h"
#define PRINTER_MODEL_DC20_190 "Configuration_DC20_190.h"
#define PRINTER_MODEL_DC20FC "Configuration_DC20FC.h"
#define PRINTER_MODEL_DC20FCHB "Configuration_DC20FCHB.h"
#define PRINTER_MODEL_DC20FCHB_190 "Configuration_DC20FCHB_190.h"
#define PRINTER_MODEL_DC20HB "Configuration_DC20HB.h"
#define PRINTER_MODEL_DC20HB_190 "Configuration_DC20HB_190.h"
#define PRINTER_MODEL_DC202E "Configuration_DC202E.h"
#define PRINTER_MODEL_DC202EHB "Configuration_DC202EHB.h"
#define PRINTER_MODEL_DC202EHB_190 "Configuration_DC202EHB_190.h"

#include "boards.h"
#include "macros.h"
#include "Version.h"
#include PRINTER_MODEL_DC20HB
#include "Configuration_common.h"
#include "Conditionals_LCD.h"
#include "drivers.h"
#include "Configuration_adv.h"
//#include "Configuration_adv.h"

#if USE_MARLINSERIAL
  #define HardwareSerial_h // trick to disable the standard HWserial
#endif

#include "types.h"
#include "HAL.h"
#include "pins.h"
#include "Conditionals_post.h"
#include "SanityCheck.h"
#include "enum.h"
#include "language.h"
#include "utility.h"
#include "serial.h"

#endif // MARLIN_CONFIG_H
