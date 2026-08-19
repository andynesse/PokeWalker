#include "game_renderer.h"

void GameRenderer::menuRender(const GameState &state)
{
    switch (state.ui.menuMode)
    {
    case MenuMode::MAIN_MENU:
        GameRenderer::print("Menu", 0, 0, 2);
        GameRenderer::print("Black: close", 0, 24, 1);
        break;
    
    case MenuMode::SETTINGS:
        GameRenderer::print("Settings", 0, 0, 2);
        break;
    
    default:
        GameRenderer::print("None", 0, 0, 2);
        break;
    }
}