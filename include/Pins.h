#pragma once

#include <cstdint>

/*
===============================================================================
ESP32-S3 Motor Health Monitoring System
-------------------------------------------------------------------------------
File        : Pins.h
Description : Central GPIO configuration for the entire project.

Author  : Nayeem
Version : 1.0
===============================================================================
*/

namespace Pins
{
    //=========================================================================
    // ADXL345 Accelerometer (I2C)
    //=========================================================================
    constexpr uint8_t I2C_SDA = 8;
    constexpr uint8_t I2C_SCL = 9;

    //=========================================================================
    // DS18B20 Temperature Sensor (OneWire)
    //=========================================================================
    constexpr uint8_t ONE_WIRE = 4;

    //=========================================================================
    // ACS712 Current Sensor
    // NOTE:
    // Sensor VCC -> ESP32 5V (VIN)
    // Signal OUT -> GPIO1 (ADC1)
    //=========================================================================
    constexpr uint8_t ACS712 = 1;

    //=========================================================================
    // L298N Motor Driver
    //=========================================================================
    constexpr uint8_t MOTOR_IN1 = 5;
    constexpr uint8_t MOTOR_IN2 = 6;
    constexpr uint8_t MOTOR_ENA = 7;

    //=========================================================================
    // JGA25-370 Quadrature Encoder
    // Yellow -> GPIO2
    // Green  -> GPIO3
    //=========================================================================
    constexpr uint8_t ENCODER_A = 2;
    constexpr uint8_t ENCODER_B = 3;

    //=========================================================================
    // SD Card Module (SPI)
    //=========================================================================
    constexpr uint8_t SD_CS = 10;
    constexpr uint8_t SD_MOSI = 11;
    constexpr uint8_t SD_SCK = 12;
    constexpr uint8_t SD_MISO = 13;
}