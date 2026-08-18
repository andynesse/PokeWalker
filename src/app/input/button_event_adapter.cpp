#include "button_event_adapter.h"

#include "../../hardware/buttons.h"

namespace
{
    GameButton toGameButton(ButtonId button)
    {
        switch (button) {
        case BUTTON_BLACK:
            return GameButton::BLACK;
        case BUTTON_BLUE:
            return GameButton::BLUE;
        case BUTTON_RED:
        default:
            return GameButton::RED;
        }
    }

    bool toGameEventType(ButtonEventType buttonType, GameEventType &gameType)
    {
        switch (buttonType) {
        case BUTTON_EVENT_SHORT_PRESS:
            gameType = GameEventType::BUTTON_SHORT_PRESS;
            return true;
        case BUTTON_EVENT_LONG_PRESS:
            gameType = GameEventType::BUTTON_LONG_PRESS;
            return true;
        case BUTTON_EVENT_RELEASE:
            gameType = GameEventType::BUTTON_RELEASE;
            return true;
        case BUTTON_EVENT_NONE:
        case BUTTON_EVENT_HELD:
        default:
            return false;
        }
    }
}

ButtonEventAdapter::ButtonEventAdapter(EventBus &eventBus)
    : eventBus(eventBus)
{
}

void ButtonEventAdapter::update()
{
    ButtonEvent buttonEvent;
    while (buttonsGetEvent(buttonEvent)) {
        GameEventType eventType;
        if (toGameEventType(buttonEvent.type, eventType)) {
            eventBus.publish({
                eventType,
                toGameButton(buttonEvent.button),
                0
            });
        }
    }
}
