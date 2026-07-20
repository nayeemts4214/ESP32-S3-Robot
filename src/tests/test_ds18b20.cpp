// Select the "test_ds18b20" environment, then Upload.
#include <Arduino.h>
#include "DS18B20.h"
#include "Pins.h"

DS18B20 tempSensor(ONE_WIRE_PIN);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("DS18B20 standalone test");
    tempSensor.begin();
}

void loop() {
    tempSensor.requestReading();
    while (!tempSensor.isReadingReady()) {
        delay(10); // blocking is fine in a simple standalone test
    }

    Serial.print("Temperature: ");
    Serial.print(tempSensor.getTemperatureC());
    Serial.println(" C");

    delay(500);
}
