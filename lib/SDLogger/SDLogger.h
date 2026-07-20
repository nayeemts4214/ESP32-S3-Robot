#pragma once
#include <Arduino.h>

class SDLogger {
public:
    SDLogger(uint8_t csPin, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin);

    bool begin();
    bool logLine(const String& line);
    bool isReady() const { return _ready; }

private:
    uint8_t _cs, _sck, _miso, _mosi;
    bool _ready = false;
    const char* _logFileName = "/robot_log.csv";
};
