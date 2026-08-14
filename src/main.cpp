#include <Arduino.h>
#include "display.h"
#include "sensor.h"
#include <Wire.h>
#include <math.h>

// --------------------------------------------------
// Timing
// --------------------------------------------------

static const unsigned long SENSOR_UPDATE_INTERVAL_MS = 20;
static const unsigned long DISPLAY_UPDATE_INTERVAL_MS = 100;
static const unsigned long STARTUP_TIME_MS = 1500;

static unsigned long startTime = 0;
static unsigned long lastSensorUpdateMs = 0;
static unsigned long lastDisplayUpdateMs = 0;


// --------------------------------------------------
// Sensor
// --------------------------------------------------

static bool sensorReady = false;
static SensorData latestSensorData;


// --------------------------------------------------
// Step detector
// --------------------------------------------------

static int steps = 0;

// Low-pass gravity estimate.
// Keep this stable while walking; otherwise the detector keeps chasing its own signal.
static float gravityMagnitude = 9.81f;
static bool gravityCalibrated = false;

// Smoothed dynamic acceleration.
static float filteredMotion = 0.0f;


// --------------------------------------------------
// Filtering parameters
// --------------------------------------------------

// Gravity follows slowly.
// 0.01 means very slow movement of the baseline.
static const float GRAVITY_ALPHA = 0.01f;

// Smooth the dynamic signal.
static const float MOTION_ALPHA = 0.30f;


// --------------------------------------------------
// Step detection parameters
// --------------------------------------------------

// Minimum acceleration required for a peak.
// Lower than before so normal walking is not lost in the noise floor.
static const float STEP_THRESHOLD = 1.6f;

// Once the signal drops back down to a small fraction of the burst,
// the step is considered complete.
static const float STEP_RECOVERY_THRESHOLD = 0.8f;

// Short spikes are usually vibration noise, not walking.
static const unsigned long MIN_PEAK_DURATION_MS = 60;

// A person cannot realistically take two steps
// this close together.
static const unsigned long MIN_STEP_INTERVAL_MS = 220;

// If we don't see another valid step for this long,
// reset the walking sequence.
static const unsigned long WALKING_TIMEOUT_MS = 1800;

// A single spike should not be allowed to last forever.
static const unsigned long MAX_PEAK_DURATION_MS = 500;


// --------------------------------------------------
// Peak detection state
// --------------------------------------------------

static unsigned long lastStepTime = 0;
static unsigned long peakStartTime = 0;
static unsigned long quietStartTime = 0;

static float peakValue = 0.0f;

static bool inPeak = false;


// --------------------------------------------------
// Calculate orientation-independent motion
// --------------------------------------------------

static float calculateMotionMagnitude()
{
    float magnitude = getMagnitude(
        latestSensorData.ax,
        latestSensorData.ay,
        latestSensorData.az
    );

    // Only adapt the gravity reference when the sensor is effectively still.
    if (!gravityCalibrated) {
        gravityMagnitude = magnitude;
        gravityCalibrated = true;
    }
    else if (fabsf(magnitude - gravityMagnitude) < 0.35f) {
        gravityMagnitude =
            gravityMagnitude * (1.0f - GRAVITY_ALPHA) +
            magnitude * GRAVITY_ALPHA;
    }

    // Remove gravity.
    float dynamicAcceleration =
        magnitude - gravityMagnitude;

    // Smooth the result.
    filteredMotion =
        filteredMotion * (1.0f - MOTION_ALPHA) +
        dynamicAcceleration * MOTION_ALPHA;

    // Use the magnitude of the dynamic acceleration so the detector is
    // orientation-independent and can handle both positive and negative peaks.
    return fabsf(filteredMotion);
}


// --------------------------------------------------
// Step detector
// --------------------------------------------------

static void updateStepDetector(
    float motion,
    unsigned long now
)
{
    // Ignore startup period.
    if (now - startTime < STARTUP_TIME_MS) {
        return;
    }

    // Reset stale state after a long pause.
    if (lastStepTime != 0 &&
        now - lastStepTime > WALKING_TIMEOUT_MS) {

        inPeak = false;
        peakValue = 0.0f;
        peakStartTime = 0;
        quietStartTime = 0;
        lastStepTime = 0;
    }

    // If the sensor goes quiet for a while, clear stale step timing so a brief stop
    // does not freeze the detector.
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

        // Finish the step when the motion falls back below the recovery threshold.
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

        // Prevent a stuck peak from lasting forever.
        if (now - peakStartTime > MAX_PEAK_DURATION_MS) {
            inPeak = false;
            peakValue = 0.0f;
            peakStartTime = 0;
            quietStartTime = 0;
        }

        return;
    }

    // Start a new peak only if the signal crosses the step threshold.
    if (motion > STEP_THRESHOLD) {
        inPeak = true;
        peakValue = motion;
        peakStartTime = now;
        quietStartTime = 0;
    }
}


// --------------------------------------------------
// Setup
// --------------------------------------------------

void setup()
{
    Serial.begin(115200);

    Wire.begin(8, 9);

    if (!displayInit()) {
        while (true) {
            delay(100);
        }
    }

    displayClear();
    displayShow();

    startTime = millis();

    sensorReady = sensorInit();

    if (sensorReady) {
        sensorUpdate();
        latestSensorData = getSensorData();

        // Initialize gravity estimate from actual sensor and keep it stable.
        gravityMagnitude = getMagnitude(
            latestSensorData.ax,
            latestSensorData.ay,
            latestSensorData.az
        );
        gravityCalibrated = true;
    }
}


// --------------------------------------------------
// Main loop
// --------------------------------------------------

void loop()
{
    unsigned long now = millis();


    // --------------------------------------------------
    // Sensor update
    // --------------------------------------------------

    if (sensorReady &&
        now - lastSensorUpdateMs >= SENSOR_UPDATE_INTERVAL_MS) {

        lastSensorUpdateMs = now;

        if (sensorUpdate()) {

            latestSensorData = getSensorData();

            float motion = calculateMotionMagnitude();

            updateStepDetector(
                motion,
                now
            );

            Serial.printf(
                "Motion: %.2f | Steps: %d\n",
                motion,
                steps
            );
        }
    }


    // --------------------------------------------------
    // Display update
    // --------------------------------------------------

    if (now - lastDisplayUpdateMs >= DISPLAY_UPDATE_INTERVAL_MS) {

        lastDisplayUpdateMs = now;

        displayClear();

        if (!sensorReady) {

            displayPrint(
                "MPU6050 not found",
                0,
                0
            );

        }
        else if (now - startTime < STARTUP_TIME_MS) {

            displayPrint(
                "Starting up...",
                0,
                0
            );

        }
        else {

            displayPrint(
                String(steps).c_str(),
                0,
                0
            );
        }

        displayShow();
    }
}