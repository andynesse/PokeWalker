#include "game_renderer.h"

#include "../../hardware/display.h"

void GameRenderer::render(const GameState &state)
{
    displayClear();

    switch (state.ui.screenMode) {
        case ScreenMode::STARTUP:
            GameRenderer::print("Starting up...", 0, 0, 1);
            break;

        case ScreenMode::MAIN:
            GameRenderer::print("Main screen", 0, 0, 2);
            GameRenderer::print("Black: menu", 0, 24, 1);
            break;

        case ScreenMode::MENU:
            GameRenderer::menuRender(state);
            break;
        
        default:
            GameRenderer::print("Unknown screen mode", 0, 0, 1);
            break;
    }

    displayShow();
}

void GameRenderer::print(const char *text, int x, int y, int size)
{
    displayPrint(text, x, y, size);
}