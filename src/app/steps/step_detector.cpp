#include "step_detector.h"
#include <math.h>

namespace
{
    static const unsigned long STARTUP_TIME_MS = 1500;
    static const unsigned long MIN_PEAK_DURATION_MS = 60;
    static const unsigned long MIN_STEP_INTERVAL_MS = 280;
    static const unsigned long WALKING_TIMEOUT_MS = 1800;
    static const unsigned long MAX_PEAK_DURATION_MS = 500;
    static const float GRAVITY_ALPHA = 0.01f;
    static const float MOTION_ALPHA = 0.30f;
    static const float STEP_THRESHOLD = 1.6f;
    static const float STEP_RECOVERY_THRESHOLD = 0.8f;
}

StepDetector::StepDetector(EventBus &eventBus)
    : eventBus(eventBus)
{
}

void StepDetector::init(unsigned long now)
{
    startTime = now;
    gravityCalibrated = false;
    filteredMotion = 0.0f;
    lastStepTime = 0;
    peakStartTime = 0;
    quietStartTime = 0;
    peakValue = 0.0f;
    inPeak = false;
}

void StepDetector::update(const SensorData &data, unsigned long now)
{
    if (now - startTime < STARTUP_TIME_MS) {
        return;
    }

    float magnitude = getMagnitude(data.ax, data.ay, data.az);
    if (!gravityCalibrated) {
        gravityMagnitude = magnitude;
        gravityCalibrated = true;
    }
    else if (fabsf(magnitude - gravityMagnitude) < 0.35f) {
        gravityMagnitude = gravityMagnitude * (1.0f - GRAVITY_ALPHA) +
            magnitude * GRAVITY_ALPHA;
    }

    float dynamicAcceleration = magnitude - gravityMagnitude;
    filteredMotion = filteredMotion * (1.0f - MOTION_ALPHA) +
        dynamicAcceleration * MOTION_ALPHA;
    float motion = fabsf(filteredMotion);

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
                eventBus.publish(GameStepEvent{now});
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
