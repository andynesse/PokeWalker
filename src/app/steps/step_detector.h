#pragma once

#include "../events/event_bus.h"
#include "../../hardware/sensor.h"

class StepDetector
{
public:
    explicit StepDetector(EventBus &eventBus);

    void init(unsigned long now);
    void update(const SensorData &data, unsigned long now);

private:
    EventBus &eventBus;
    unsigned long startTime = 0;
    unsigned long lastStepTime = 0;
    unsigned long peakStartTime = 0;
    unsigned long quietStartTime = 0;
    float gravityMagnitude = 9.81f;
    float filteredMotion = 0.0f;
    float peakValue = 0.0f;
    bool gravityCalibrated = false;
    bool inPeak = false;
};
