#pragma once

/*
===============================================================================
ESP32-S3 Motor Health Monitoring System
GPIO Configuration
===============================================================================
*/

//
// ADXL345 (I2C)
//
#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

//
// DS18B20
//
#define ONE_WIRE_PIN 4

//
// ACS712
//
#define ACS712_PIN 1

//
// L298N
//
#define MOTOR_IN1_PIN 5
#define MOTOR_IN2_PIN 6
#define MOTOR_ENA_PIN 7

//
// Encoder
//
#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

//
// SD Card
//
#define SD_CS_PIN 10
#define SD_MOSI_PIN 11
#define SD_SCK_PIN 12
#define SD_MISO_PIN 13