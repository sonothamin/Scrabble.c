#include "game.h"
#include "ui.h"
#include <stdbool.h>
#include <stdio.h>

#define BOARD_SIZE 15
#define BOARD_CELL_SIZE 25
#define BOARD_ORIGIN_X 120
#define BOARD_ORIGIN_Y 90
#define RACK_ORIGIN_X 120
#define RACK_ORIGIN_Y 450

// These variables keep the board selection state inside this file only.
// They are simple enough for another student to understand and modify later.
static int g_selectedRow = -1;
static int g_selectedCol = -1;
static int g_selectedRackIndex = -1;

// This demo rack is kept lightweight so the design can be explained easily.
// It is only a visual placeholder for the board layout task.
static const char g_demoRackLetters[] = { 'A', 'E', 'T', 'R', 'U', 'Q', 'S' };
static const int g_demoRackValues[] = { 1, 1, 1, 1, 1, 10, 1 };

// This helper draws one cell of the board.
// The cell color changes depending on whether it is selected or on a special square.
static void DrawBoardCell(int row, int col)
{
    const int cellX = BOARD_ORIGIN_X + (col * BOARD_CELL_SIZE);
    const int cellY = BOARD_ORIGIN_Y + (row * BOARD_CELL_SIZE);
    const bool isSelected = (g_selectedRow == row && g_selectedCol == col);

    // The center square is highlighted to mimic a Scrabble board look.
    if (row == 7 && col == 7)
    {
        DrawRectangle(cellX, cellY, BOARD_CELL_SIZE, BOARD_CELL_SIZE, ORANGE);
    }
    else if (isSelected)
    {
        DrawRectangle(cellX, cellY, BOARD_CELL_SIZE, BOARD_CELL_SIZE, YELLOW);
    }
    else if ((row == 0 && col == 0) || (row == 0 && col == 14) || (row == 14 && col == 0) || (row == 14 && col == 14))
    {
        DrawRectangle(cellX, cellY, BOARD_CELL_SIZE, BOARD_CELL_SIZE, DARKBROWN);
    }
    else
    {
        DrawRectangle(cellX, cellY, BOARD_CELL_SIZE, BOARD_CELL_SIZE, GREEN);
    }

    // Draw the square border so the board looks structured and clean.
    DrawRectangleLines(cellX, cellY, BOARD_CELL_SIZE, BOARD_CELL_SIZE, DARKGREEN);
}

// This helper renders the full 15x15 board area.
// It keeps the logic simple so the project can be explained easily in front of an examiner.
static void DrawGameBoard(void)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            DrawBoardCell(row, col);
        }
    }
}

// This helper draws the player's rack at the bottom of the game screen.
// The rack is only a visual mockup for now, which is enough for the board-design task.
static void DrawRack(void)
{
    const int rackTileWidth = 40;
    const int rackTileHeight = 40;
    const int gap = 8;

    for (int index = 0; index < 7; index++)
    {
        const int tileX = RACK_ORIGIN_X + (index * (rackTileWidth + gap));
        const int tileY = RACK_ORIGIN_Y;
        const bool isSelected = (g_selectedRackIndex == index);

        // Highlight the selected rack tile to simulate interaction.
        if (isSelected)
        {
            DrawRectangle(tileX, tileY, rackTileWidth, rackTileHeight, SKYBLUE);
        }
        else
        {
            DrawRectangle(tileX, tileY, rackTileWidth, rackTileHeight, LIGHTGRAY);
        }

        DrawRectangleLines(tileX, tileY, rackTileWidth, rackTileHeight, GRAY);

        // Draw the letter inside the rack tile.
        char letterText[2] = { g_demoRackLetters[index], '\0' };
        const int letterWidth = MeasureText(letterText, 20);
        DrawText(letterText, tileX + (rackTileWidth - letterWidth) / 2, tileY + 8, 20, BLACK);

        // Draw the tile score value below the letter.
        char scoreText[8];
        snprintf(scoreText, sizeof(scoreText), "%d", g_demoRackValues[index]);
        DrawText(scoreText, tileX + 12, tileY + 24, 12, DARKGRAY);
    }
}

// This helper listens for mouse input and remembers the selected board position.
// It is intentionally kept small and easy to explain in a presentation.
static void UpdateBoardSelection(void)
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return;
    }

    const Vector2 mousePos = GetMousePosition();
    const int row = (int)((mousePos.y - BOARD_ORIGIN_Y) / BOARD_CELL_SIZE);
    const int col = (int)((mousePos.x - BOARD_ORIGIN_X) / BOARD_CELL_SIZE);

    // Only accept valid positions inside the board grid.
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        g_selectedRow = row;
        g_selectedCol = col;
    }
}

// This helper listens for rack selection in a very simple way.
// It uses the keyboard number keys so the board design can be demonstrated without adding extra files.
static void UpdateRackSelection(void)
{
    if (IsKeyPressed(KEY_ONE))
    {
        g_selectedRackIndex = 0;
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        g_selectedRackIndex = 1;
    }
    else if (IsKeyPressed(KEY_THREE))
    {
        g_selectedRackIndex = 2;
    }
    else if (IsKeyPressed(KEY_FOUR))
    {
        g_selectedRackIndex = 3;
    }
    else if (IsKeyPressed(KEY_FIVE))
    {
        g_selectedRackIndex = 4;
    }
    else if (IsKeyPressed(KEY_SIX))
    {
        g_selectedRackIndex = 5;
    }
    else if (IsKeyPressed(KEY_SEVEN))
    {
        g_selectedRackIndex = 6;
    }
}

void GameUpdate(AppState* state)
{
    if (state == NULL)
    {
        return;
    }

    // This branch keeps the original screen navigation intact.
    // Pressing B still returns to the menu, as the previous code already did.
    if (IsKeyPressed(KEY_B))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        return;
    }

    // The update logic is kept intentionally lightweight.
    // The board design part is represented by mouse selection and rack selection.
    UpdateBoardSelection();
    UpdateRackSelection();
}

void GameDraw(void)
{
    // The screen title is preserved from the original placeholder.
    // The content below is now a simple board-design prototype for the game screen.
    DrawScreenBox("Game Screen", "Click a square or press 1-7 to select a rack tile");

    // The board is drawn first so the game area is visible right away.
    DrawGameBoard();

    // The rack is drawn below the board to complete the screen layout.
    DrawRack();

    // This status line helps the examiner understand the current board selection.
    if (g_selectedRow >= 0 && g_selectedCol >= 0)
    {
        char statusText[64];
        snprintf(statusText, sizeof(statusText), "Selected square: row %d, col %d", g_selectedRow, g_selectedCol);
        DrawText(statusText, 120, 40, 18, DARKGRAY);
    }

    if (g_selectedRackIndex >= 0)
    {
        char rackStatus[64];
        snprintf(rackStatus, sizeof(rackStatus), "Selected rack tile: %c", g_demoRackLetters[g_selectedRackIndex]);
        DrawText(rackStatus, 120, 60, 18, DARKGRAY);
    }
}
