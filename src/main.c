#include "raylib.h"
#include "resource_dir.h"
#include "app_state.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    InitWindow(1024, 768, "Scrabble.c");
    MaximizeWindow();
    SetWindowMinSize(800, 600);
    SetTargetFPS(30);

    SearchAndSetResourceDir("resources");

    AppState appState;
    InitAppState(&appState);

    appState.currentScreen = APP_SCREEN_LOADING;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();

        UpdateAppState(&appState);

        BeginDrawing();
           DrawAppState(&appState);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}