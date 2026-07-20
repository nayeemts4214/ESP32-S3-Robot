#include <Arduino.h>
#include "Robot.h"

Robot robot;

void setup()
{
    Serial.begin(115200);
    delay(1000); // gives the Serial Monitor time to connect after upload
    Serial.println();
    Serial.println("ESP32-S3 autonomous robot booting up...");

    robot.begin();
}

void loop()
{
    robot.update();
}
