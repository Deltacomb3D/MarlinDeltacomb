#define STRING_SPLASH_LINE1 "DC-20 FLUX + HEATBEB"

#define CUSTOM_MACHINE_NAME     "Mod. DC-20 FCHB"
#define CUSTOM_MACHINE_VERSION  "r4.0"

#define EXTRUDERS 2
#define SINGLENOZZLE

#define TEMP_SENSOR_1 0
#define TEMP_SENSOR_BED 5

#define DELTA_PRINTABLE_RADIUS 95.0 // mm
#define DELTA_DIAGONAL_ROD 200.0 // mm
#define DELTA_HEIGHT 240.00 // L'altezza predefinita è più o meno 5mm maggiore dell'altezza massima
#define DELTA_RADIUS 89.0 //mm  Get this value from auto calibrate

#define JOB_ABORT_ROUNTINE "G91\nG0 E-15 F10000\nG1 Z2\nG4 P3000\nG0 E14.7 F10000\nG1 E-69.7 F10000\nG28\nM524"

#define FILAMENT_CHANGE_FAST_LOAD_LENGTH   380
#define FILAMENT_UNLOAD_PURGE_LENGTH         0

// Has Power loss circuit modifications
//#define POWER_LOSS_RECOVERY

// DC NewGen 19v + Ramps RRD
//#define DC_RAMPS_19V_SUPPLY

// TMC Drivers
//#define HAS_DRIVER_TMC

// Runout sensor
//#define FILAMENT_RUNOUT_SENSOR