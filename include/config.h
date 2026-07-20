#pragma once
// The master control panel. This is the ONLY file you edit to
// turn modules on/off or tune how "smart" the robot behaves —
// main.cpp never changes.

// ----- Module enable/disable switches (1 = included, 0 = excluded) -----
#define USE_ADXL345    1   // Accelerometer — vibration / tilt
#define USE_ACS712     1   // Current sensor — motor current draw
#define USE_DS18B20    1   // Temperature sensor
#define USE_MOTOR      1   // L298N + JGA25-370 motor
#define USE_ENCODER    1   // JGA25-370 encoder feedback (RPM)
#define USE_SDCARD     1   // SD card data logging

// ----- Sensor calibration -----
// ACS712 sensitivity: 185 mV/A (5A model) / 100 mV/A (20A) / 66 mV/A (30A)
#define ACS712_MV_PER_AMP     100.0f

// ----- Safety thresholds: the autonomous "smart" part -----
// The robot watches these continuously and cuts the motor on its
// own if either limit is crossed — no input from you needed.
#define SAFE_CURRENT_LIMIT_A   2.0f     // Amps
#define SAFE_TEMP_LIMIT_C      60.0f    // Celsius

// ----- Non-blocking scheduling intervals (milliseconds) -----
#define INTERVAL_SENSOR_READ_MS    100
#define INTERVAL_SAFETY_CHECK_MS   50
#define INTERVAL_LOG_WRITE_MS      1000
#define INTERVAL_STATUS_PRINT_MS   1000
