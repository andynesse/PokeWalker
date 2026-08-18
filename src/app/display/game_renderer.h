#pragma once

#include "../game/game_state.h"

class GameRenderer
{
public:
    void render(const GameState &state, bool sensorReady);
};
