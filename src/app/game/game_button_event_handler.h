#pragma once

#include "../events/game_events.h"
#include "game_state.h"

namespace GameButtonEventHandler
{
    void handle(
        const GameButtonEvent& event,
        GameState& state
    );
}