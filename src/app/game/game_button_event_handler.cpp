#include "game_button_event_handler.h" 

void GameButtonEventHandler::handle(
    const GameButtonEvent& event,
    GameState& state)
{
    if (state.ui.screenMode == ScreenMode::STARTUP) {
        return;
    }

    if (event.pressType == ButtonPressType::BUTTON_SHORT_PRESS &&
        event.button == ButtonColor::BLACK)
    {
        state.ui.screenMode =
            state.ui.screenMode == ScreenMode::MENU
                ? ScreenMode::MAIN
                : ScreenMode::MENU;
    }
}