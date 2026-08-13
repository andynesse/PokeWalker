#include <Arduino.h>
#include "display.h"
#include "sensor.h"
#include <Wire.h>

static bool sensorReady = false;
static unsigned long lastSensorUpdateMs = 0;
static unsigned long lastDisplayUpdateMs = 0;
static SensorData latestSensorData;

static const unsigned long sensorUpdateIntervalMs = 50;
static const unsigned long displayUpdateIntervalMs = 100;

void setup() {
    Serial.begin(115200);
    Wire.begin(8, 9);

    if (!displayInit()) {
        while (true);
    }

    displayClear();
    displayShow();

    sensorReady = sensorInit();
    latestSensorData = getSensorData();
}

void loop() {
    unsigned long now = millis();

    if (sensorReady && now - lastSensorUpdateMs >= sensorUpdateIntervalMs) {
        lastSensorUpdateMs = now;
        sensorUpdate();
        latestSensorData = getSensorData();
    }

    if (now - lastDisplayUpdateMs >= displayUpdateIntervalMs) {
        lastDisplayUpdateMs = now;
        displayClear();

        if (!sensorReady) {
            displayPrint("MPU6050 not found", 0, 0);
        } else if (!sensorIsCalibrated()) {
            displayPrint("Calibrating MPU...", 0, 0);
        } else {
            displayPrint(String(getMagnitude(latestSensorData.ax, latestSensorData.ay, latestSensorData.az)).c_str(), 0, 0);
        }

        displayShow();
    }
}

const char *sensorDataToString(const SensorData &data) {
    static char buffer[100];
    snprintf(buffer, sizeof(buffer), "Accel: %.2f, %.2f, %.2f\nGyro: %.2f, %.2f, %.2f\nTemp: %.2f",
             data.ax, data.ay, data.az,
             data.gx, data.gy, data.gz,
             data.temperature);
    return buffer;
}