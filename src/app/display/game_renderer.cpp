#include "game_renderer.h"

#include "../../hardware/display.h"

#include <Arduino.h>

void GameRenderer::render(const GameState &state, bool sensorReady)
{
    displayClear();

    if (!sensorReady) {
        displayPrint("MPU6050 not found", 0, 0, 1);
    }
    else if (state.mode == GameMode::STARTUP) {
        displayPrint("Starting up...", 0, 0, 1);
    }
    else if (state.mode == GameMode::MENU) {
        displayPrint("Menu", 0, 0, 2);
        displayPrint("Black: close", 0, 24, 1);
    }
    else {
        displayPrint(String(state.totalSteps).c_str(), 0, 16, 3);
    }

    displayShow();
}
