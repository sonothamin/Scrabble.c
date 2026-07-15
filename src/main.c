#include "raylib.h"
#include "resource_dir.h"
#include "app_state.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    InitWindow(1024, 768, "Scrabble.c");
    SetExitKey(KEY_F4);
    MaximizeWindow();
    SetWindowMinSize(1380, 820);

    SearchAndSetResourceDir("resources");

    AppState appState;
    InitAppState(&appState);
    

    appState.shouldClose = false; 
    appState.currentScreen = APP_SCREEN_LOADING;

    while (!WindowShouldClose() && !appState.shouldClose)
    {
        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();

        UpdateAppState(&appState);

        BeginDrawing();
           DrawAppState(&appState);
        EndDrawing();
    }

    CloseAppState(&appState);
    CloseWindow();

    return 0;
}