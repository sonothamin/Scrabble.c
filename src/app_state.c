#include <stddef.h>
#include "app_state.h"

void InitAppState(AppState* state)
{
    if (state == NULL)
    {
        return;
    }

    state->currentScreen = APP_SCREEN_MAIN_MENU;
}

void UpdateAppState(AppState* state)
{
    if (state == NULL)
    {
        return;
    }

    switch (state->currentScreen)
    {
        case APP_SCREEN_MAIN_MENU:
            if (IsKeyPressed(KEY_S))
            {
                state->currentScreen = APP_SCREEN_SETTINGS;
            }
            break;

        case APP_SCREEN_SETTINGS:
            if (IsKeyPressed(KEY_B))
            {
                state->currentScreen = APP_SCREEN_MAIN_MENU;
            }
            break;

        default:
            state->currentScreen = APP_SCREEN_MAIN_MENU;
            break;
    }
}

static void DrawScreenBox(const char* title, const char* instructions)
{
    const int boxX = 100;
    const int boxY = 100;
    const int boxWidth = 600;
    const int boxHeight = 400;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(LIGHTGRAY, 0.55f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, GRAY);

    const int titleWidth = MeasureText(title, 40);
    DrawText(title, boxX + (boxWidth - titleWidth) / 2, boxY + 60, 40, DARKGRAY);

    const int instructionsWidth = MeasureText(instructions, 20);
    DrawText(instructions, boxX + (boxWidth - instructionsWidth) / 2, boxY + 200, 20, GRAY);
}

void DrawAppState(const AppState* state)
{
    if (state == NULL)
    {
        return;
    }

    switch (state->currentScreen)
    {
        case APP_SCREEN_MAIN_MENU:
            DrawScreenBox("Main Menu", "Press S to open Settings");
            break;

        case APP_SCREEN_SETTINGS:
            DrawScreenBox("Settings", "Press B to return to Main Menu");
            break;

        default:
            DrawScreenBox("Main Menu", "Press S to open Settings");
            break;
    }
}
