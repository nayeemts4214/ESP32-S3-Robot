// Select the "test_adxl345" environment in the PlatformIO
// toolbar, then Upload.
#include <Arduino.h>
#include <Wire.h>
#include "ADXL345.h"
#include "Pins.h"

ADXL345 accel(0x53);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("ADXL345 standalone test");

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    if (!accel.begin()) {
        Serial.println("FAILED to initialize. Check SDA/SCL wiring and the I2C address.");
    }
}

void loop() {
    accel.update();
    Serial.print("X: "); Serial.print(accel.getX());
    Serial.print("  Y: "); Serial.print(accel.getY());
    Serial.print("  Z: "); Serial.print(accel.getZ());
    Serial.print("  |a|: "); Serial.println(accel.getMagnitude());
    delay(200);
}
