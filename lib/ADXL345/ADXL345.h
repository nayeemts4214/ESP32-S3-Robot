#pragma once
#include <Arduino.h>

// Self-contained I2C driver — uses only Wire.h (built into the
// core), no extra library needed. Does NOT call Wire.begin()
// itself: the I2C bus is shared, so it's set up once by whoever
// owns it (Robot::begin(), or a test's setup()) before begin()
// is called here — that way a second I2C device never causes
// a Wire.begin() conflict.
class ADXL345 {
public:
    explicit ADXL345(uint8_t i2cAddress = 0x53);

    bool begin();
    void update();

    float getX() const { return _x; }
    float getY() const { return _y; }
    float getZ() const { return _z; }
    float getMagnitude() const;

private:
    uint8_t _address;
    float _x = 0, _y = 0, _z = 0;
    bool _initialized = false;

    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    void readRegisters(uint8_t startReg, uint8_t count, uint8_t* buffer);
};
