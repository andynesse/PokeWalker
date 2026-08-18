#pragma once

#include "game_events.h"

class EventBus
{
public:
    bool publish(const GameEvent &event)
    {
        const uint8_t nextHead = (head + 1) % QUEUE_SIZE;
        if (nextHead == tail) {
            return false;
        }

        events[head] = event;
        head = nextHead;
        return true;
    }

    bool poll(GameEvent &event)
    {
        if (head == tail) {
            return false;
        }

        event = events[tail];
        tail = (tail + 1) % QUEUE_SIZE;
        return true;
    }

private:
    static const uint8_t QUEUE_SIZE = 16;
    GameEvent events[QUEUE_SIZE];
    uint8_t head = 0;
    uint8_t tail = 0;
};
