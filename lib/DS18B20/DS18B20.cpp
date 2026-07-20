#include "DS18B20.h"

DS18B20::DS18B20(uint8_t pin)
    : _oneWire(pin), _sensors(&_oneWire) {}

bool DS18B20::begin() {
    _sensors.begin();
    _sensors.setWaitForConversion(false); // non-blocking mode

    int deviceCount = _sensors.getDeviceCount();
    _connected = (deviceCount > 0);

    if (_connected) {
        Serial.print("[DS18B20] Found ");
        Serial.print(deviceCount);
        Serial.println(" sensor(s)");
    } else {
        Serial.println("[DS18B20] No sensor found -- check wiring and the 4.7k pull-up resistor.");
    }

    return _connected;
}

void DS18B20::requestReading() {
    if (!_connected) return;
    _sensors.requestTemperatures();
    _conversionStartedAt = millis();
    _conversionInProgress = true;
}

bool DS18B20::isReadingReady() {
    if (!_conversionInProgress) return false;

    if (millis() - _conversionStartedAt >= 750) {
        _temperatureC = _sensors.getTempCByIndex(0);
        _conversionInProgress = false;
        return true;
    }
    return false;
}
