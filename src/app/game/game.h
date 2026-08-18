#pragma once

#include "../events/game_events.h"
#include "game_state.h"

class Game
{
public:
    void init(unsigned long now);
    void update(unsigned long now);
    void handleEvent(const GameEvent &event);

    const GameState &getState() const;

private:
    GameState state;
    unsigned long startTime = 0;
};
