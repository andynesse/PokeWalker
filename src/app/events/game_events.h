#pragma once

#include <stdint.h>
#include <variant>

enum class ButtonPressType : uint8_t
{
    BUTTON_SHORT_PRESS,
    BUTTON_LONG_PRESS,
    BUTTON_RELEASE,
};

enum class ButtonColor : uint8_t
{
    RED,
    BLACK,
    BLUE,
};

struct GameButtonEvent
{
    ButtonColor button;
    ButtonPressType pressType;
};

struct GameStepEvent
{
    uint32_t timestamp;
};

using GameEvent = std::variant<
    GameButtonEvent,
    GameStepEvent
>;
