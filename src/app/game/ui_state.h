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
    NONE,
    MAIN_MENU,
    SETTINGS,
};

struct UIState
{
    ScreenMode screenMode = ScreenMode::STARTUP;
    MenuMode menuMode = MenuMode::NONE;

    uint32_t selectedId = 0;
};