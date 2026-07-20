#include "ACS712.h"

ACS712::ACS712(uint8_t analogPin, float mvPerAmp)
    : _pin(analogPin), _mvPerAmp(mvPerAmp) {}

bool ACS712::begin() {
    pinMode(_pin, INPUT);

    // Calibrate the zero-current offset. IMPORTANT: no current
    // should be flowing through the sensor right now (motor
    // stopped), or this calibration will be wrong.
    _zeroOffsetMv = readAveragedMilliVolts(128);

    Serial.print("[ACS712] Zero-current offset: ");
    Serial.print(_zeroOffsetMv);
    Serial.println(" mV");

    return true;
}

void ACS712::update() {
    float mv = readAveragedMilliVolts(16);
    _currentAmps = (mv - _zeroOffsetMv) / _mvPerAmp;
}

float ACS712::readAveragedMilliVolts(int samples) {
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogReadMilliVolts(_pin); // calibrated mV reading, built into the ESP32 core
        delayMicroseconds(100);
    }
    return (float)sum / samples;
}
