#include <Arduino.h>
#include<SPI.h>
#include<Wire.h>
#include<DS3231.h>

#include "Hardware/Hardware.h"
#include "Circadia.h"

#define DISP_CS 5

#define SD_CS 16

Hardware hardware(DISP_CS, SD_CS, 32, 33, 27, 14);
Circadia circadia(hardware);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    hardware.setup();
}

void loop() {
    circadia.update();
}