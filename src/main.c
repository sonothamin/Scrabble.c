#include "raylib.h"
#include "resource_dir.h"
#include "app_state.h"

int main()
{
    // flags to work with high-dpi monitors and vsync
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(800, 600, "Scrabble.c");

    SearchAndSetResourceDir("resources");

    AppState appState;
    InitAppState(&appState);

    while (!WindowShouldClose())
    {
        UpdateAppState(&appState);

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawAppState(&appState);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
