#pragma once

#include "../events/game_events.h"
#include "game_state.h"

class Game
{
public:
    void init(unsigned long now);
    void update(unsigned long now);
    void handleEvent(const GameEvent &event);

    void stepIncrement();
    

    const GameState &getGameState() const ; 

private:
    GameState gameState;
    unsigned long startTime = 0;
};
