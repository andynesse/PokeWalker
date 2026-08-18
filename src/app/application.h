#pragma once

#include "events/event_bus.h"
#include "game/game.h"
#include "display/game_renderer.h"
#include "input/button_event_adapter.h"
#include "steps/step_detector.h"

class Application
{
public:
    Application();

    void init();
    void update(unsigned long now);

private:
    static const unsigned long SENSOR_UPDATE_INTERVAL_MS = 20;
    static const unsigned long DISPLAY_UPDATE_INTERVAL_MS = 100;

    EventBus eventBus;
    Game game;
    StepDetector stepDetector;
    ButtonEventAdapter buttonEventAdapter;
    GameRenderer renderer;
    unsigned long lastSensorUpdateMs = 0;
    unsigned long lastDisplayUpdateMs = 0;
};
