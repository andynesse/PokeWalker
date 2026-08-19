#include "game.h"

#include <variant>

namespace
{
    static const unsigned long STARTUP_TIME_MS = 1500;
}

void Game::init(unsigned long now)
{
    startTime = now;
    gameState = GameState();
}

void Game::update(unsigned long now)
{
    if (gameState.ui.screenMode == ScreenMode::STARTUP &&
        now - startTime >= STARTUP_TIME_MS) {
        gameState.ui.screenMode = ScreenMode::MAIN;
    }

    gameState.gameTime = now - startTime;
}

void Game::handleEvent(const GameEvent &event)
{
    if (std::holds_alternative<GameStepEvent>(event)) {
        stepIncrement();
        return;
    }

    if (gameState.ui.screenMode == ScreenMode::STARTUP) {
        return;
    }

    const GameButtonEvent &buttonEvent = std::get<GameButtonEvent>(event);
    if (buttonEvent.pressType == ButtonPressType::BUTTON_SHORT_PRESS &&
        buttonEvent.button == ButtonColor::BLACK) {
        gameState.ui.screenMode = gameState.ui.screenMode == ScreenMode::MENU
            ? ScreenMode::MAIN
            : ScreenMode::MENU;
    }
}

void Game::stepIncrement()
{
    gameState.totalSteps++;
    gameState.stepsToday++;
}


const GameState &Game::getGameState() const
{
    return gameState;
}