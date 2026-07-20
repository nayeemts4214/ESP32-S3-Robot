#pragma once
#include <Arduino.h>

// Analog current sensor driver.
//
// HARDWARE NOTE: the ACS712 is a 5V sensor whose output swings
// around 2.5V. The ESP32's ADC tops out at 3.3V. If your ACS712
// is powered from 5V, put a voltage divider (e.g. 10k/20k) on its
// OUT pin before it reaches the ESP32, or power the sensor from
// 3.3V instead (check its datasheet for the resulting mV/A at 3.3V).
class ACS712 {
public:
    // mvPerAmp: 185 for the 5A model, 100 for 20A, 66 for 30A
    explicit ACS712(uint8_t analogPin, float mvPerAmp = 100.0f);

    bool begin();
    void update();
    float getCurrentAmps() const { return _currentAmps; }

private:
    uint8_t _pin;
    float _mvPerAmp;
    float _zeroOffsetMv = 0;
    float _currentAmps = 0;

    float readAveragedMilliVolts(int samples);
};
