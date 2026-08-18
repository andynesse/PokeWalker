#include "buttons.h"

const int NUM_BUTTONS = 3;
const int buttonPins[NUM_BUTTONS] = {
    41, // red
    38, // black
    39 // blue
};

// Button timing

static const unsigned long DEBOUNCE_DELAY_MS = 30;
static const unsigned long LONG_PRESS_MS = 600;
static const unsigned long HELD_INTERVAL_MS = 100;

// Button state

struct ButtonState {
    bool rawState;
    bool stableState;

    unsigned long lastRawChangeTime;
    unsigned long pressStartTime;
    unsigned long lastHeldTime;

    bool longPressSent;
};

static ButtonState buttonStates[NUM_BUTTONS];

// Event queue

static const int EVENT_QUEUE_SIZE = 8;

static ButtonEvent eventQueue[EVENT_QUEUE_SIZE];
static int eventHead = 0;
static int eventTail = 0;

// Add event to queue

static void pushEvent(ButtonId button, ButtonEventType type) {
    int nextHead = (eventHead + 1) % EVENT_QUEUE_SIZE;

    if (nextHead == eventTail) {
        return;
    }

    eventQueue[eventHead].button = button;
    eventQueue[eventHead].type = type;
    eventHead = nextHead;
}

// Initialize

void buttonsInit() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT);

    bool state = digitalRead(buttonPins[i]) == HIGH;
    buttonStates[i].rawState = state;
    buttonStates[i].stableState = state;

    buttonStates[i].lastRawChangeTime = millis();
    buttonStates[i].pressStartTime = 0;
    buttonStates[i].lastHeldTime = 0;
    buttonStates[i].longPressSent = false;
  }
}

// Update buttons

void buttonsUpdate(unsigned long now) {
    for (int i = 0; i < NUM_BUTTONS; i++) {

        ButtonState &button = buttonStates[i];
        
        bool rawState = digitalRead(buttonPins[i]) == HIGH;

        // Raw state changed, reset debounce timer

        if (rawState != button.rawState) {

            button.rawState = rawState;
            button.lastRawChangeTime = now;
        }

        // Wait for debounce

        if (now - button.lastRawChangeTime < DEBOUNCE_DELAY_MS) {
            continue;
        }

        // Stable state changed

        if (button.stableState != button.rawState) {

    button.stableState = button.rawState;

    

    if (button.stableState) {

        // PRESS
        button.pressStartTime = now;
        button.lastHeldTime = now;
        button.longPressSent = false;

        
    }
    else {

        // RELEASE
        unsigned long pressDuration = 0;

        if (button.pressStartTime != 0) {
            pressDuration = now - button.pressStartTime;
        }

        

        if (!button.longPressSent &&
            button.pressStartTime != 0 &&
            pressDuration < LONG_PRESS_MS) {

            

            pushEvent(
                static_cast<ButtonId>(i),
                BUTTON_EVENT_SHORT_PRESS
            );
        }

        pushEvent(
            static_cast<ButtonId>(i),
            BUTTON_EVENT_RELEASE
        );

        button.pressStartTime = 0;
        button.lastHeldTime = 0;
        button.longPressSent = false;
    }
}

        // Button held

        if (button.stableState) {
            unsigned long heldDuration = now - button.pressStartTime;

            // Long press

            if (!button.longPressSent && heldDuration > LONG_PRESS_MS) {
                pushEvent(
                    static_cast<ButtonId>(i),
                    BUTTON_EVENT_LONG_PRESS
                );
                button.longPressSent = true;
                button.lastHeldTime = now;
            }

            // Repeat held events

            if (button.longPressSent && 
                now - button.lastHeldTime >= HELD_INTERVAL_MS) {
                pushEvent(
                    static_cast<ButtonId>(i),
                    BUTTON_EVENT_HELD
                );

                button.lastHeldTime = now;
            }
        }
    }
}

// Get next event

bool buttonsGetEvent(ButtonEvent &event) {
    if (eventHead == eventTail) {
        return false;
    }

    event = eventQueue[eventTail];

    eventTail = (eventTail + 1) % EVENT_QUEUE_SIZE;

    return true;
}
