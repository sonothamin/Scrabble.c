#include "raylib.h"
#include "resource_dir.h"

int main()
{
	// flags to work with high-dpi monitors and vsync
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(800, 600, "Scrabble.c");

	SearchAndSetResourceDir("resources");
	Texture logo = LoadTexture("scrabble_logo_256x256.png");

	// game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);
		DrawText("Scrabble.c", 200, 200, 25, WHITE);
		DrawTexture(logo, 400, 200, WHITE);

		EndDrawing();
	}

	UnloadTexture(logo);

	CloseWindow();
	return 0;
}
