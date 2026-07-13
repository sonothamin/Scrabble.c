#include "game.h"
#include "ui.h"

void GameUpdate(AppState* state)
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

void GameDraw(AppState* state)
{
    DrawScreenBox("Game Screen", "Press B to return to Main Menu");
}
