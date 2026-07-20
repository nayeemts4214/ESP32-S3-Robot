#pragma once
#include <Arduino.h>
#include "config.h"
#include "Pins.h"

#if USE_ADXL345
#include "ADXL345.h"
#endif
#if USE_ACS712
#include "ACS712.h"
#endif
#if USE_DS18B20
#include "DS18B20.h"
#endif
#if USE_MOTOR
#include "Motor.h"
#endif
#if USE_ENCODER
#include "Encoder.h"
#endif
#if USE_SDCARD
#include "SDLogger.h"
#endif

// Ties every module together and makes the "smart" decisions:
// what to read, when to log, and when to protect the motor from
// overcurrent or overtemperature on its own.
class Robot {
public:
    void begin();
    void update();

private:
    void readSensors();
    void checkSafety();
    void logData();
    void printStatus();

    unsigned long _lastSensorRead = 0;
    unsigned long _lastSafetyCheck = 0;
    unsigned long _lastLogWrite = 0;
    unsigned long _lastStatusPrint = 0;
    bool _safetyStopActive = false;

#if USE_ADXL345
    ADXL345 _accel{0x53};
#endif
#if USE_ACS712
    ACS712 _currentSensor{ACS712_PIN, ACS712_MV_PER_AMP};
#endif
#if USE_DS18B20
    DS18B20 _tempSensor{ONE_WIRE_PIN};
#endif
#if USE_MOTOR
    Motor _motor{MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_ENA_PIN};
#endif
#if USE_ENCODER
    Encoder _encoder{ENCODER_A_PIN, ENCODER_B_PIN};
#endif
#if USE_SDCARD
    SDLogger _sdLogger{SD_CS_PIN, SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN};
#endif
};
