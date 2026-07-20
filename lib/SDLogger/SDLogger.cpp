#include "SDLogger.h"
#include <SPI.h>
#include <SD.h>

SDLogger::SDLogger(uint8_t csPin, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin)
    : _cs(csPin), _sck(sckPin), _miso(misoPin), _mosi(mosiPin) {}

bool SDLogger::begin()
{
    SPI.begin(_sck, _miso, _mosi, _cs);

    if (!SD.begin(_cs))
    {
        Serial.println("[SDLogger] Card mount failed -- check wiring and that the card is FAT32.");
        _ready = false;
        return false;
    }

    if (SD.cardType() == CARD_NONE)
    {
        Serial.println("[SDLogger] No SD card detected.");
        _ready = false;
        return false;
    }

    if (!SD.exists(_logFileName))
    {
        File file = SD.open(_logFileName, FILE_WRITE);
        if (file)
        {
            file.println("timestamp_ms,temp_c,current_a,accel_x,accel_y,accel_z,motor_speed,encoder_rpm");
            file.close();
        }
    }

    _ready = true;
    Serial.println("[SDLogger] Ready");
    return true;
}

bool SDLogger::logLine(const String &line)
{
    if (!_ready)
        return false;

    File file = SD.open(_logFileName, FILE_APPEND);
    if (!file)
    {
        Serial.println("[SDLogger] Failed to open log file for append");
        return false;
    }

    file.println(line);
    file.close();
    return true;
}
