#include <stddef.h>
#include "app_state.h"
#include "ui.h"
#include "menu.h"
#include "settings.h"
#include "game.h"

void InitAppState(AppState *state)
{
    if (state == NULL)
        return;

    state->currentScreen = APP_SCREEN_MAIN_MENU;
}

void UpdateAppState(AppState *state)
{
    if (state == NULL)
        return;

    switch (state->currentScreen)
    {
    case APP_SCREEN_MAIN_MENU:
        MenuUpdate(state);
        break;

    case APP_SCREEN_SETTINGS:
        SettingsUpdate(state);
        break;

    case APP_SCREEN_GAME:
        GameUpdate(state);
        break;

    default:
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        break;
    }
}

void DrawAppState(const AppState *state)
{
    if (state == NULL)
        return;

    switch (state->currentScreen)
    {
    case APP_SCREEN_MAIN_MENU:
        MenuDraw();
        break;

    case APP_SCREEN_SETTINGS:
        SettingsDraw();
        break;

    case APP_SCREEN_GAME:
        GameDraw();
        break;

    default:
        MenuDraw();
        break;
    }
}
