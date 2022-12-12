#define STRING_SPLASH_LINE1 "DC-30"     

#define CUSTOM_MACHINE_NAME     "Mod. DC-30"
#define CUSTOM_MACHINE_VERSION  "r4.0"

#define EXTRUDERS 1

#define TEMP_SENSOR_1 0
#define TEMP_SENSOR_BED 0

#define DELTA_PRINTABLE_RADIUS 145.0 // mm
#define DELTA_DIAGONAL_ROD 310.0 // mm
//#define DELTA_HEIGHT 245.00 // L'altezza predefinita è più o meno 5mm maggiore dell'altezza massima
#define DELTA_HEIGHT 300.00 // Altezza dell'area di stampa
#define DELTA_RADIUS 139.0 //mm  Get this value from auto calibrate

#define JOB_ABORT_ROUNTINE "G91\nG1 E-4\nG28\nM524"

#define FILAMENT_CHANGE_FAST_LOAD_LENGTH   700
#define FILAMENT_UNLOAD_PURGE_LENGTH         1

// Has Power loss circuit modifications
//#define POWER_LOSS_RECOVERY

// DC NewGen 19v + Ramps RRD
//#define DC_RAMPS_19V_SUPPLY

// TMC Drivers
//#define HAS_DRIVER_TMC

// Runout sensor
//#define FILAMENT_RUNOUT_SENSOR