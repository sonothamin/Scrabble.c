// Sonoth
#include "raylib.h"
#include "resource_dir.h"
#include "app_state.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    // To drag the window and make its size smaller and larger
    InitWindow(1024, 768, "Scrabble.c");
    MaximizeWindow();           // grabs the monitor size
    SetWindowMinSize(800, 600); // Guardrail to prevent accidental screen shrinkage

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