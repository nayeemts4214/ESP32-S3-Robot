// Select the "test_acs712" environment, then Upload.
#include <Arduino.h>
#include "ACS712.h"
#include "Pins.h"
#include "config.h"

ACS712 currentSensor(ACS712_PIN, ACS712_MV_PER_AMP);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("ACS712 test -- keep the motor OFF for the first moment (zero calibration)");
    currentSensor.begin();
}

void loop() {
    currentSensor.update();
    Serial.print("Current: ");
    Serial.print(currentSensor.getCurrentAmps(), 3);
    Serial.println(" A");
    delay(200);
}
