#include "game_step_event_handler.h"

void GameStepEventHandler::handle(
    const GameStepEvent& event,
    GameState& state)
{
    if (state.ui.screenMode == ScreenMode::STARTUP) {
        return;
    }

    state.totalSteps++;
    state.stepsToday++;
}