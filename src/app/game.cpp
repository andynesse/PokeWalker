#include "app/game.h"

#include "hardware/buttons.h"
#include "hardware/display.h"
#include "hardware/sensor.h"

#include <Arduino.h>
#include <math.h>

namespace
{
    static const unsigned long SENSOR_UPDATE_INTERVAL_MS = 20;
    static const unsigned long DISPLAY_UPDATE_INTERVAL_MS = 100;
    static const unsigned long STARTUP_TIME_MS = 1500;

    static unsigned long startTime = 0;
    static unsigned long lastSensorUpdateMs = 0;
    static unsigned long lastDisplayUpdateMs = 0;

    static bool sensorReady = false;
    static SensorData latestSensorData;
    static int steps = 0;

    static float gravityMagnitude = 9.81f;
    static bool gravityCalibrated = false;
    static float filteredMotion = 0.0f;

    static const float GRAVITY_ALPHA = 0.01f;
    static const float MOTION_ALPHA = 0.30f;
    static const float STEP_THRESHOLD = 1.6f;
    static const float STEP_RECOVERY_THRESHOLD = 0.8f;
    static const unsigned long MIN_PEAK_DURATION_MS = 60;
    static const unsigned long MIN_STEP_INTERVAL_MS = 280;
    static const unsigned long WALKING_TIMEOUT_MS = 1800;
    static const unsigned long MAX_PEAK_DURATION_MS = 500;

    static unsigned long lastStepTime = 0;
    static unsigned long peakStartTime = 0;
    static unsigned long quietStartTime = 0;
    static float peakValue = 0.0f;
    static bool inPeak = false;

    float calculateMotionMagnitude()
    {
        float magnitude = getMagnitude(
            latestSensorData.ax,
            latestSensorData.ay,
            latestSensorData.az
        );

        if (!gravityCalibrated) {
            gravityMagnitude = magnitude;
            gravityCalibrated = true;
        }
        else if (fabsf(magnitude - gravityMagnitude) < 0.35f) {
            gravityMagnitude =
                gravityMagnitude * (1.0f - GRAVITY_ALPHA) +
                magnitude * GRAVITY_ALPHA;
        }

        float dynamicAcceleration = magnitude - gravityMagnitude;
        filteredMotion =
            filteredMotion * (1.0f - MOTION_ALPHA) +
            dynamicAcceleration * MOTION_ALPHA;

        return fabsf(filteredMotion);
    }

    void updateStepDetector(float motion, unsigned long now)
    {
        if (now - startTime < STARTUP_TIME_MS) {
            return;
        }

        if (lastStepTime != 0 && now - lastStepTime > WALKING_TIMEOUT_MS) {
            inPeak = false;
            peakValue = 0.0f;
            peakStartTime = 0;
            quietStartTime = 0;
            lastStepTime = 0;
        }

        if (motion < STEP_RECOVERY_THRESHOLD) {
            if (quietStartTime == 0) {
                quietStartTime = now;
            }
            else if (now - quietStartTime > 450) {
                inPeak = false;
                peakValue = 0.0f;
                peakStartTime = 0;
                lastStepTime = 0;
                quietStartTime = 0;
            }
        }
        else {
            quietStartTime = 0;
        }

        if (inPeak) {
            if (motion > peakValue) {
                peakValue = motion;
            }

            if (motion < STEP_RECOVERY_THRESHOLD &&
                now - peakStartTime >= MIN_PEAK_DURATION_MS) {

                if (peakValue >= STEP_THRESHOLD &&
                    (lastStepTime == 0 || now - lastStepTime >= MIN_STEP_INTERVAL_MS)) {
                    steps++;
                    lastStepTime = now;
                }

                inPeak = false;
                peakValue = 0.0f;
                peakStartTime = 0;
                quietStartTime = 0;
                return;
            }

            if (now - peakStartTime > MAX_PEAK_DURATION_MS) {
                inPeak = false;
                peakValue = 0.0f;
                peakStartTime = 0;
                quietStartTime = 0;
            }

            return;
        }

        if (motion > STEP_THRESHOLD) {
            inPeak = true;
            peakValue = motion;
            peakStartTime = now;
            quietStartTime = 0;
        }
    }

    void renderGameDisplay(unsigned long now)
    {
        displayClear();

        if (!sensorReady) {
            displayPrint("MPU6050 not found", 0, 0, 1);
        }
        else if (now - startTime < STARTUP_TIME_MS) {
            displayPrint("Starting up...", 0, 0, 1);
        }
        else {
            displayPrint(String(steps).c_str(), 0, 16, 3);
        }

        displayShow();
    }
}

void gameInit()
{
    startTime = millis();
    sensorReady = sensorIsReady();

    if (sensorReady) {
        sensorUpdate();
        latestSensorData = getSensorData();

        gravityMagnitude = getMagnitude(
            latestSensorData.ax,
            latestSensorData.ay,
            latestSensorData.az
        );
        gravityCalibrated = true;
    }
}

void gameUpdate(unsigned long now)
{
    if (sensorReady && now - lastSensorUpdateMs >= SENSOR_UPDATE_INTERVAL_MS) {
        lastSensorUpdateMs = now;

        if (sensorUpdate()) {
            latestSensorData = getSensorData();
            float motion = calculateMotionMagnitude();
            updateStepDetector(motion, now);
        }
    }

    if (now - lastDisplayUpdateMs >= DISPLAY_UPDATE_INTERVAL_MS) {
        lastDisplayUpdateMs = now;
        renderGameDisplay(now);
    }
}