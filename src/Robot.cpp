#include "Robot.h"
#include <Wire.h>
#include "HealthMonitor.h"

HealthMonitor monitor;

// Optional: store latest data globally for reuse
MotorData currentData = {0};

void Robot::begin()
{
    Serial.println("============================================");
    Serial.println(" Robot: initializing modules...");
    Serial.println("============================================");

#if USE_ADXL345
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    if (!_accel.begin())
        Serial.println("[Robot] ADXL345 failed to initialize");
#endif

#if USE_ACS712
    _currentSensor.begin();
#endif

#if USE_DS18B20
    _tempSensor.begin();
#endif

#if USE_MOTOR
    _motor.begin();
#endif

#if USE_ENCODER
    _encoder.begin();
#endif

#if USE_SDCARD
    _sdLogger.begin();
#endif

    Serial.println("[Robot] All modules initialized. Running autonomously.");
}

void Robot::update()
{
    unsigned long now = millis();

    if (now - _lastSensorRead >= INTERVAL_SENSOR_READ_MS)
    {
        _lastSensorRead = now;
        readSensors();
    }

    if (now - _lastSafetyCheck >= INTERVAL_SAFETY_CHECK_MS)
    {
        _lastSafetyCheck = now;
        checkSafety();
    }

    if (now - _lastLogWrite >= INTERVAL_LOG_WRITE_MS)
    {
        _lastLogWrite = now;
        logData();
    }

    if (now - _lastStatusPrint >= INTERVAL_STATUS_PRINT_MS)
    {
        _lastStatusPrint = now;
        printStatus();
    }

#if USE_ENCODER
    _encoder.update();
#endif

#if USE_DS18B20
    _tempSensor.isReadingReady();
#endif

    // 🔴 SAFETY CONTROL (CRITICAL)
#if USE_MOTOR
    if (monitor.isFault() && !_safetyStopActive)
    {
        _motor.stop();
        _safetyStopActive = true;

        Serial.print("[Robot] SAFETY STOP: ");
        Serial.println(monitor.getStatus());
    }
    else if (!monitor.isFault() && _safetyStopActive)
    {
        _safetyStopActive = false;
        Serial.println("[Robot] Safety cleared");

        // Optional restart:
        // _motor.start();
    }
#endif
}

void Robot::readSensors()
{
#if USE_ADXL345
    _accel.update();
#endif

#if USE_ACS712
    _currentSensor.update();
#endif

#if USE_DS18B20
    _tempSensor.requestReading();
#endif
}

void Robot::checkSafety()
{
    MotorData data = {0};

#if USE_ACS712
    data.current = _currentSensor.getCurrentAmps();
#endif

#if USE_DS18B20
    data.temperature = _tempSensor.getTemperatureC();
#endif

#if USE_ADXL345
    data.vibration = _accel.getMagnitude();
#endif

#if USE_ENCODER
    data.rpm = (int)_encoder.getRPM();
#endif

    // Store globally
    currentData = data;

    // Update health system
    monitor.update(data);
}

void Robot::logData()
{
#if USE_SDCARD
    String line = "";

    line += String(millis()) + ",";
    line += String(currentData.temperature, 2) + ",";
    line += String(currentData.current, 3) + ",";
    line += String(currentData.vibration, 3) + ",";
    line += String(currentData.rpm) + ",";
    line += monitor.getStatus();

    _sdLogger.logLine(line);
#endif
}

void Robot::printStatus()
{
    Serial.print("[Status] ");

    Serial.print("Temp:");
    Serial.print(currentData.temperature);
    Serial.print("C  ");

    Serial.print("Current:");
    Serial.print(currentData.current);
    Serial.print("A  ");

    Serial.print("Vibration:");
    Serial.print(currentData.vibration);
    Serial.print("g  ");

    Serial.print("RPM:");
    Serial.print(currentData.rpm);
    Serial.print("  ");

#if USE_MOTOR
    Serial.print("MotorSpeed:");
    Serial.print(_motor.getSpeed());

    if (_safetyStopActive)
        Serial.print(" [SAFETY STOP]");
#endif

    Serial.print(" Health:");
    Serial.print(monitor.getStatus());

    Serial.println();
}