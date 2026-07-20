#include "ADXL345.h"
#include <Wire.h>
#include <math.h>

// ADXL345 register map (Analog Devices datasheet)
static const uint8_t REG_DEVID       = 0x00;
static const uint8_t REG_POWER_CTL   = 0x2D;
static const uint8_t REG_DATA_FORMAT = 0x31;
static const uint8_t REG_DATAX0      = 0x32;

static const uint8_t EXPECTED_DEVID  = 0xE5;
static const float   SCALE_FACTOR_G  = 0.0039f; // 3.9 mg/LSB, full-res mode

ADXL345::ADXL345(uint8_t i2cAddress) : _address(i2cAddress) {}

bool ADXL345::begin() {
    uint8_t id = readRegister(REG_DEVID);
    if (id != EXPECTED_DEVID) {
        Serial.print("[ADXL345] Unexpected device ID: 0x");
        Serial.println(id, HEX);
        Serial.println("[ADXL345] Check wiring, or try address 0x1D instead of 0x53.");
        return false;
    }

    writeRegister(REG_DATA_FORMAT, 0x0B); // full resolution, +/-16g
    writeRegister(REG_POWER_CTL, 0x08);   // measurement mode

    _initialized = true;
    return true;
}

void ADXL345::update() {
    if (!_initialized) return;

    uint8_t raw[6];
    readRegisters(REG_DATAX0, 6, raw);

    int16_t rawX = (int16_t)((raw[1] << 8) | raw[0]);
    int16_t rawY = (int16_t)((raw[3] << 8) | raw[2]);
    int16_t rawZ = (int16_t)((raw[5] << 8) | raw[4]);

    _x = rawX * SCALE_FACTOR_G;
    _y = rawY * SCALE_FACTOR_G;
    _z = rawZ * SCALE_FACTOR_G;
}

float ADXL345::getMagnitude() const {
    return sqrtf(_x * _x + _y * _y + _z * _z);
}

void ADXL345::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t ADXL345::readRegister(uint8_t reg) {
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom((int)_address, 1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0;
}

void ADXL345::readRegisters(uint8_t startReg, uint8_t count, uint8_t* buffer) {
    Wire.beginTransmission(_address);
    Wire.write(startReg);
    Wire.endTransmission(false);
    Wire.requestFrom((int)_address, (int)count);
    for (uint8_t i = 0; i < count && Wire.available(); i++) {
        buffer[i] = Wire.read();
    }
}
