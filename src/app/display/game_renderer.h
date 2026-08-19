#pragma once

#include "../game/game_state.h"

class GameRenderer
{
public:
    void render(const GameState &state);

    void print(const char *text, int x, int y, int size);

    void homeRender(const GameState &state);
    void menuRender(const GameState &state);
};
