#include "menu.h"
#include "ui.h"

void MenuUpdate(AppState* state)
{
    if (state == NULL)
    {
        return;
    }

    if (IsKeyPressed(KEY_G))
    {
        state->currentScreen = APP_SCREEN_GAME;
    }
    else if (IsKeyPressed(KEY_S))
    {
        state->currentScreen = APP_SCREEN_SETTINGS;
    }
}

void MenuDraw(void)
{
    DrawScreenBox("Main Menu", "Press G to start game or S to open settings");
}
