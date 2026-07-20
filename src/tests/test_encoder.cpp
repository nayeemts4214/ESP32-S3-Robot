// Select the "test_encoder" environment, then Upload.
// Spin the output shaft by hand and watch the tick count change --
// also how you calibrate TICKS_PER_REV in lib/Encoder/Encoder.cpp.
#include <Arduino.h>
#include "Encoder.h"
#include "Pins.h"

Encoder encoder(ENCODER_A_PIN, ENCODER_B_PIN);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Encoder standalone test -- rotate the output shaft by hand");
    encoder.begin();
}

void loop() {
    encoder.update();
    Serial.print("Ticks: "); Serial.print(encoder.getTicks());
    Serial.print("  RPM: "); Serial.println(encoder.getRPM());
    delay(200);
}
