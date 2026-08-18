#pragma once

#include "../events/event_bus.h"

class ButtonEventAdapter
{
public:
    explicit ButtonEventAdapter(EventBus &eventBus);

    void update();

private:
    EventBus &eventBus;
};
