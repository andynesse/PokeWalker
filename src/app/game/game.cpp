#include "game.h"
#include "game_button_event_handler.h"
#include "game_step_event_handler.h"

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
    if (std::holds_alternative<GameButtonEvent>(event))
    {
        const GameButtonEvent &buttonEvent = std::get<GameButtonEvent>(event);
        
        GameButtonEventHandler::handle(buttonEvent, gameState);
    }
    else if (std::holds_alternative<GameStepEvent>(event))
    {
        const GameStepEvent &stepEvent = std::get<GameStepEvent>(event);
        GameStepEventHandler::handle(stepEvent, gameState);
    }
}


const GameState &Game::getGameState() const
{
    return gameState;
}