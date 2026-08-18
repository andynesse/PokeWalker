#pragma once

#include <stdint.h>

static const uint16_t GAME_SAVE_VERSION = 1;

struct GameSave
{
    uint16_t version = GAME_SAVE_VERSION;

    uint32_t totalSteps = 0;

    uint16_t pokemonSpecies = 0;
    uint8_t pokemonLevel = 1;
    uint32_t pokemonExperience = 0;

    uint8_t unlockedAreas = 0;
};
