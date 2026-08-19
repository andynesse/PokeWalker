#include "game_button_event_handler.h" 

void GameButtonEventHandler::handle(
    const GameButtonEvent& event,
    GameState& state)
{
    if (state.ui.screenMode == ScreenMode::STARTUP) {
        return;
    }

    switch (state.ui.screenMode) {
        case ScreenMode::MAIN:
            mainHandle(event, state);
            break;

        case ScreenMode::MENU:
            menuHandle(event, state);
            break;

        default:
            break;
    }
}

void GameButtonEventHandler::menuHandle(
    const GameButtonEvent& event,
    GameState& state)
{
    switch (state.ui.menuMode) {
        case MenuMode::MAIN_MENU:
            switch (event.button) {
                case ButtonColor::RED:
                    if (event.pressType == ButtonPressType::BUTTON_SHORT_PRESS) {
                        state.ui.screenMode = ScreenMode::MAIN;
                        state.ui.menuMode = MenuMode::MAIN_MENU;
                    }
                    break;
                
                case ButtonColor::BLACK:
                    if (event.pressType == ButtonPressType::BUTTON_SHORT_PRESS) {
                        state.ui.menuMode = MenuMode::SETTINGS;
                    }
                    break;
                
                default:
                    break;
            }
            break;
        
        case MenuMode::SETTINGS:
            switch (event.button) {
                case ButtonColor::RED:
                    if (event.pressType == ButtonPressType::BUTTON_SHORT_PRESS) {
                        state.ui.menuMode = MenuMode::MAIN_MENU;
                    }
                    break;

                default:
                    break;
            }
            break;
        
        default:
            break;
    }
}

void GameButtonEventHandler::mainHandle(
    const GameButtonEvent& event,
    GameState& state)
{
    switch (event.button) {
        case ButtonColor::BLACK:
            if (event.pressType == ButtonPressType::BUTTON_SHORT_PRESS) {
                state.ui.screenMode = ScreenMode::MENU;
                state.ui.menuMode = MenuMode::MAIN_MENU;
            }
            break;
        
        default:
            break;
    }
}