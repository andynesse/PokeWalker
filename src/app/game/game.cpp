#include "game.h"

namespace
{
    static const unsigned long STARTUP_TIME_MS = 1500;
}

void Game::init(unsigned long now)
{
    startTime = now;
    state = GameState();
}

void Game::update(unsigned long now)
{
    if (state.mode == GameMode::STARTUP &&
        now - startTime >= STARTUP_TIME_MS) {
        state.mode = GameMode::MAIN;
    }

    state.gameTime = now - startTime;
}

void Game::handleEvent(const GameEvent &event)
{
    if (event.type == GameEventType::STEP_DETECTED) {
        state.totalSteps++;
        state.stepsToday++;
        return;
    }

    if (state.mode == GameMode::STARTUP) {
        return;
    }

    if (event.type == GameEventType::BUTTON_SHORT_PRESS &&
        event.button == GameButton::BLACK) {
        state.mode = state.mode == GameMode::MENU
            ? GameMode::MAIN
            : GameMode::MENU;
    }
}

const GameState &Game::getState() const
{
    return state;
}
