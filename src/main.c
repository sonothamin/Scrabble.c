#include "raylib.h"
#include "resource_dir.h"
#include "app_state.h"
#include "settings.h"
#include "error_service.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    InitWindow(1024, 768, "Scrabble.c");
    SetExitKey(KEY_F4);
    MaximizeWindow();
    SetWindowMinSize(1380, 820);

    SearchAndSetResourceDir("resources");

    AppState appState = {0};
    InitAppState(&appState);

    appState.shouldClose = false;

    if (appState.settingsState != NULL && appState.settingsState->showLoadingScreen)
    {
        appState.currentScreen = APP_SCREEN_LOADING;
    }
    else
    {
        appState.currentScreen = APP_SCREEN_MAIN_MENU;
    }

    // Explicitly reset on clean boot
    ClearGlobalError();

    while (!WindowShouldClose() && !appState.shouldClose)
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        if (!HasGlobalError())
        {
            UpdateAppState(&appState);
        }

        BeginDrawing();
        DrawAppState(&appState);
        if (HasGlobalError() && ShowErrorDialog())
        {
            appState.shouldClose = true;
        }
        EndDrawing();
    }

    CloseAppState(&appState);
    CloseWindow();

    return 0;
}