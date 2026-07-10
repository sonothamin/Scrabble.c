#include "settings.h"
#include "ui.h"

void SettingsUpdate(AppState* state)
{
    if (state == NULL)
    {
        return;
    }

    if (IsKeyPressed(KEY_B))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void SettingsDraw(void)
{
    DrawScreenBox("Settings", "Press B to return to Main Menu");
}
