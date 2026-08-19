#pragma once

#include <stdint.h>

enum class ScreenMode
{
    STARTUP,
    MAIN,
    MENU,
};

enum class MenuMode
{
    MAIN_MENU,
    SETTINGS,
};

struct UIState
{
    ScreenMode screenMode = ScreenMode::STARTUP;
    MenuMode menuMode = MenuMode::MAIN_MENU;

    uint32_t selectedId = 0;
};