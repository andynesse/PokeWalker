#include "button_event_adapter.h"

#include "../../hardware/buttons.h"

namespace
{
    ButtonColor toGameButton(ButtonId button)
    {
        switch (button) {
        case BUTTON_BLACK:
            return ButtonColor::BLACK;
        case BUTTON_BLUE:
            return ButtonColor::BLUE;
        case BUTTON_RED:
        default:
            return ButtonColor::RED;
        }
    }

    bool toButtonPressType(
        ButtonEventType buttonType,
        ButtonPressType &pressType
    )
    {
        switch (buttonType) {
        case BUTTON_EVENT_SHORT_PRESS:
            pressType = ButtonPressType::BUTTON_SHORT_PRESS;
            return true;
        case BUTTON_EVENT_LONG_PRESS:
            pressType = ButtonPressType::BUTTON_LONG_PRESS;
            return true;
        case BUTTON_EVENT_RELEASE:
            pressType = ButtonPressType::BUTTON_RELEASE;
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
        ButtonPressType pressType;
        if (toButtonPressType(buttonEvent.type, pressType)) {
            eventBus.publish(GameButtonEvent{
                toGameButton(buttonEvent.button),
                pressType
            });
        }
    }
}
