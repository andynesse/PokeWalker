#include <Arduino.h>
#include <Wire.h>

#include "app/application.h"
#include "hardware/display.h"
#include "hardware/sensor.h"

namespace
{
    Application application;
}

void setup()
{
    Serial.begin(115200);

    Wire.begin(8, 9);

    displayInit();
    sensorInit();
    application.init();
}

void loop()
{
    unsigned long now = millis();

    application.update(now);
}