#pragma once
// Every physical wire, in one place. Change a wire? Change it
// here — nowhere else in the project should mention a raw GPIO
// number. Chosen to avoid strapping pins (0,3,45,46), native-USB
// pins (19,20), and the pins reserved by this board's octal PSRAM.

// ----- I2C bus (ADXL345 accelerometer) -----
#define I2C_SDA_PIN        8
#define I2C_SCL_PIN        9

// ----- OneWire bus (DS18B20 temperature) -----
#define ONE_WIRE_PIN       4

// ----- Analog input (ACS712 current sensor), must be an ADC1 pin -----
#define ACS712_PIN         1     // ADC1_CH0

// ----- L298N motor driver -----
#define MOTOR_IN1_PIN      5
#define MOTOR_IN2_PIN      6
#define MOTOR_ENA_PIN      7     // PWM speed control

// ----- JGA25-370 encoder (Yellow=A, Green=B, Blue=VCC, Black=GND) -----
#define ENCODER_A_PIN      15
#define ENCODER_B_PIN      16

// ----- SD card module (SPI) -----
#define SD_CS_PIN          10
#define SD_SCK_PIN         12
#define SD_MISO_PIN        13
#define SD_MOSI_PIN        11
