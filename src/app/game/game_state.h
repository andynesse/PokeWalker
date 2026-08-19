#pragma once

#include <stdint.h>
#include "ui_state.h"


struct GameState
{
    UIState ui;

    uint32_t totalSteps = 0;
    uint32_t stepsToday = 0;

    uint16_t currentPokemon = 0;
    uint16_t pokemonLevel = 1;

    uint32_t experience = 0;

    uint32_t gameTime = 0;
};
