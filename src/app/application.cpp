#include "application.h"

#include "../hardware/buttons.h"
#include "../hardware/sensor.h"

#include <Arduino.h>

Application::Application()
    : stepDetector(eventBus),
      buttonEventAdapter(eventBus)
{
}

void Application::init()
{
    buttonsInit();

    const unsigned long now = millis();
    stepDetector.init(now);
    game.init(now);
}

void Application::update(unsigned long now)
{
    buttonsUpdate(now);
    buttonEventAdapter.update();

    if (sensorIsReady() &&
        now - lastSensorUpdateMs >= SENSOR_UPDATE_INTERVAL_MS) {
        lastSensorUpdateMs = now;
        if (sensorUpdate()) {
            stepDetector.update(getSensorData(), now);
        }
    }

    GameEvent event;
    while (eventBus.poll(event)) {
        game.handleEvent(event);
    }

    game.update(now);

    if (now - lastDisplayUpdateMs >= DISPLAY_UPDATE_INTERVAL_MS && sensorIsReady()) {
        lastDisplayUpdateMs = now;
        renderer.render(game.getGameState());
    }
}
