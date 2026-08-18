#pragma once

#ifndef BUTTONS_H
#define BUTTONS_H
#include <Arduino.h>

enum ButtonId {
    BUTTON_RED = 0,
    BUTTON_BLACK = 1,
    BUTTON_BLUE = 2
};

enum ButtonEventType {
    BUTTON_EVENT_NONE,
    BUTTON_EVENT_SHORT_PRESS,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_HELD,
    BUTTON_EVENT_RELEASE
};

struct ButtonEvent {
    ButtonEventType type;
    ButtonId button;
};


void buttonsInit();
void buttonsUpdate(unsigned long now);
bool buttonsGetEvent(ButtonEvent &event);
#endif // BUTTONS_H