#include <Arduino.h>
#include <Wire.h>

#include "app/game.h"
#include "hardware/buttons.h"
#include "hardware/display.h"
#include "hardware/sensor.h"

void setup()
{
    Serial.begin(115200);

    Wire.begin(8, 9);

    buttonsInit();
    displayInit();
    sensorInit();

    gameInit();
}

void loop()
{
    unsigned long now = millis();

    buttonsUpdate(now);
    gameUpdate(now);
}