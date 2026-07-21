#include "drag_drop.h"
#include <math.h>
#include "game.h"

void HandleDragNDropInput(GameState *match, Rectangle boardBounds, Rectangle rackRect, float tileSize, float tileSpacing)
{
    if (!match)
        return;

    Vector2 mousePos = GetMousePosition();
    int p = match->activePlayerIdx;
    Player *currentPlayer = &match->players[p];

    // Calculate uniform alignment layout for tiles inside the rack box
    float tileY = rackRect.y + (rackRect.height - tileSize) / 2.0f;
    float startX = rackRect.x + 15.0f;

    if (!match->dragState.isDragging && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int t = 0; t < currentPlayer->rack_count; t++)
        {
            Rectangle tileBounds = {startX + (t * (tileSize + tileSpacing)), tileY, tileSize, tileSize};

            if (CheckCollisionPointRec(mousePos, tileBounds))
            {
                match->dragState.isDragging = true;
                match->dragState.draggedTileIdx = t;
                break; // Break instantly so we don't grab multiple items
            }
        }
    }

    if (match->dragState.isDragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        // Check if the drop location is within the centered board frame lines
        if (CheckCollisionPointRec(mousePos, boardBounds))
        {
            float cellSize = boardBounds.width / (float)BOARD_SIZE;
            int gridX = (int)((mousePos.x - boardBounds.x) / cellSize);
            int gridY = (int)((mousePos.y - boardBounds.y) / cellSize);

            // Bounds check grid matrix limits
            if (gridX >= 0 && gridX < BOARD_SIZE && gridY >= 0 && gridY < BOARD_SIZE)
            {
                // Only commit if target board slot is completely empty
                if (match->board.grid[gridY][gridX].letter == '\0')
                {
                    int srcIdx = match->dragState.draggedTileIdx;

                    // 1. Commit tile parameters directly to the GameState board matrix
                    match->board.grid[gridY][gridX] = currentPlayer->rack[srcIdx];

                    // 2. Remove tile from the player's rack and shift trailing elements left
                    for (int i = srcIdx; i < currentPlayer->rack_count - 1; i++)
                    {
                        currentPlayer->rack[i] = currentPlayer->rack[i + 1];
                    }
                    currentPlayer->rack_count--;
                }
            }
        }

        // Always terminate transaction context and clear operational flags on mouse release
        match->dragState.isDragging = false;
        match->dragState.draggedTileIdx = -1;
    }
}

void DrawDragNDropOverlay(const GameState *match, Rectangle rackRect, float tileSize, float tileSpacing)
{
    if (!match || !match->dragState.isDragging)
        return;

    int p = match->activePlayerIdx;
    int draggedIdx = match->dragState.draggedTileIdx;

    // Bounds check to ensure index stability
    if (draggedIdx < 0 || draggedIdx >= match->players[p].rack_count)
        return;

    Tile activeTile = match->players[p].rack[draggedIdx];
    Vector2 mousePos = GetMousePosition();

    // Center the rendering bounds directly beneath the user's cursor position
    Rectangle dragBounds = {
        mousePos.x - (tileSize / 2.0f),
        mousePos.y - (tileSize / 2.0f),
        tileSize,
        tileSize};

    // Draw the wooden tile container background with smooth transparency overrides
    DrawRectangleRounded(dragBounds, 0.2f, 4, (Color){244, 228, 198, 230});
    DrawRectangleRoundedLines(dragBounds, 0.2f, 4, (Color){194, 169, 126, 255});

    // Print character layout details centered inside the tracking square container
    char letterStr[2] = {activeTile.letter, '\0'};
    int fontSize = (int)(tileSize * 0.55f);
    int textWidth = MeasureText(letterStr, fontSize);

    DrawText(
        letterStr,
        dragBounds.x + (dragBounds.width - textWidth) / 2.0f,
        dragBounds.y + (dragBounds.height - fontSize) / 2.0f,
        fontSize,
        (Color){38, 28, 16, 255});
}