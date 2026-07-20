#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Non-blocking temperature driver. Conversions take up to 750ms —
// instead of blocking with delay(), we start a conversion and
// check back on it later without stalling anything else.
class DS18B20 {
public:
    explicit DS18B20(uint8_t pin);

    bool begin();
    void requestReading();
    bool isReadingReady();      // call often; returns true once when a fresh value lands
    float getTemperatureC() const { return _temperatureC; }
    bool isConnected() const { return _connected; }

private:
    OneWire _oneWire;
    DallasTemperature _sensors;
    float _temperatureC = -127.0f;
    bool _connected = false;
    unsigned long _conversionStartedAt = 0;
    bool _conversionInProgress = false;
};
