// Select the "test_sdcard" environment, then Upload.
#include <Arduino.h>
#include "SDLogger.h"
#include "Pins.h"
#include <SD.h>

SDLogger sdLogger(SD_CS_PIN, SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("SD card standalone test");

    if (!sdLogger.begin()) {
        Serial.println("FAILED to initialize SD card.");
        return;
    }

    sdLogger.logLine("hello,from,esp32");
    Serial.println("Wrote a test line. Reading the file back:");

    File file = SD.open("/robot_log.csv");
    if (file) {
        while (file.available()) {
            Serial.write(file.read());
        }
        file.close();
    }
}

void loop() {
    // nothing to do here
}
