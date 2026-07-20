// Select the "test_motor" environment, then Upload.
// SAFETY: make sure the motor can spin freely (wheel off the
// ground, nothing in the way) before powering this on.
#include <Arduino.h>
#include "Motor.h"
#include "Pins.h"

Motor motor(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_ENA_PIN);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Motor standalone test");
    motor.begin();
}

void loop() {
    Serial.println("Forward, half speed...");
    motor.forward(128);
    delay(2000);

    Serial.println("Stop...");
    motor.stop();
    delay(1000);

    Serial.println("Reverse, half speed...");
    motor.reverse(128);
    delay(2000);

    Serial.println("Stop...");
    motor.stop();
    delay(2000);
}
