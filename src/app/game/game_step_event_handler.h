#pragma once

#include "../events/game_events.h"
#include "game_state.h"

namespace GameStepEventHandler
{
    void handle(
        const GameStepEvent& event,
        GameState& state
    );
}