#pragma once

#include <stdint.h>

enum class GameEventType : uint8_t
{
    BUTTON_SHORT_PRESS,
    BUTTON_LONG_PRESS,
    BUTTON_RELEASE,
    STEP_DETECTED,
};

enum class GameButton : uint8_t
{
    RED,
    BLACK,
    BLUE,
};

struct GameEvent
{
    GameEventType type;
    GameButton button;
    uint32_t value;
};
