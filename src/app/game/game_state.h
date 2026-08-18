#pragma once

#include <stdint.h>

enum class GameMode
{
    STARTUP,
    MAIN,
    MENU,
};

struct GameState
{
    GameMode mode = GameMode::STARTUP;

    uint32_t totalSteps = 0;
    uint32_t stepsToday = 0;

    uint16_t currentPokemon = 0;
    uint16_t pokemonLevel = 1;

    uint32_t experience = 0;

    uint32_t gameTime = 0;
};
