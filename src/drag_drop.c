#include "drag_drop.h"
#include <math.h>
#include "game.h"
#include "board.h"

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

    // Pick up a tile on left click
    if (!match->dragState.isDragging && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int t = 0; t < currentPlayer->rack_count; t++)
        {
            Rectangle tileBounds = {startX + (t * (tileSize + tileSpacing)), tileY, tileSize, tileSize};

            if (CheckCollisionPointRec(mousePos, tileBounds))
            {
                match->dragState.isDragging = true;
                match->dragState.isFromRack = true;
                match->dragState.draggedTileIdx = t;
                match->dragState.draggedTile = currentPlayer->rack[t];
                return;
            }
        }

        // Try picking up from BOARD GRID
        if (CheckCollisionPointRec(mousePos, boardBounds))
        {
            float cellSize = boardBounds.width / (float)BOARD_SIDE;
            int gridX = (int)((mousePos.x - boardBounds.x) / cellSize);
            int gridY = (int)((mousePos.y - boardBounds.y) / cellSize);

            if (gridX >= 0 && gridX < BOARD_SIDE && gridY >= 0 && gridY < BOARD_SIDE)
            {
                // Only pick up if there's an existing tile
                if (match->board.grid[gridY][gridX].letter != '\0')
                {
                    match->dragState.isDragging = true;
                    match->dragState.isFromRack = false;
                    match->dragState.sourceGridX = gridX;
                    match->dragState.sourceGridY = gridY;
                    match->dragState.draggedTile = match->board.grid[gridY][gridX];

                    // Temporarily remove from board matrix while dragging
                    match->board.grid[gridY][gridX] = (Tile){.letter = '\0', .value = 0, .isWildCard = false};
                    return;
                }
            }
        }
    }

    // Drop the tile on left click release
    else if ((match->dragState.isDragging) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        bool dropSuccessful = false;

        if (CheckCollisionPointRec(mousePos, boardBounds))
        {
            float cellSize = boardBounds.width / (float)BOARD_SIDE;
            int gridX = (int)((mousePos.x - boardBounds.x) / cellSize);
            int gridY = (int)((mousePos.y - boardBounds.y) / cellSize);

            if (gridX < 0)
                gridX = 0;
            if (gridX >= BOARD_SIDE)
                gridX = BOARD_SIDE - 1;
            if (gridY < 0)
                gridY = 0;
            if (gridY >= BOARD_SIDE)
                gridY = BOARD_SIDE - 1;

            // Place tile if target board space is empty
            if (match->board.grid[gridY][gridX].letter == '\0')
            {
                // Copy tile to board
                match->board.grid[gridY][gridX] = match->dragState.draggedTile;

                // If it came from the rack, remove it from the rack array
                if (match->dragState.isFromRack)
                {
                    int srcIdx = match->dragState.draggedTileIdx;
                    if (srcIdx >= 0 && srcIdx < currentPlayer->rack_count)
                    {
                        for (int i = srcIdx; i < currentPlayer->rack_count - 1; i++)
                        {
                            currentPlayer->rack[i] = currentPlayer->rack[i + 1];
                        }
                        currentPlayer->rack[currentPlayer->rack_count - 1] = (Tile){.letter = '\0', .value = 0, .isWildCard = false};
                        currentPlayer->rack_count--;
                    }
                }
                dropSuccessful = true;
            }
        }
        // DROPPING BACK TO THE ACTIVE RACK
        else if (CheckCollisionPointRec(mousePos, rackRect))
        {
            if (currentPlayer->rack_count < RACK_SIZE)
            {
                // If it came from the board, add it back to the rack
                if (!match->dragState.isFromRack)
                {
                    currentPlayer->rack[currentPlayer->rack_count] = match->dragState.draggedTile;
                    currentPlayer->rack_count++;
                }
                // If it came from rack to rack, no change needed
                dropSuccessful = true;
            }
        }

        // C) CANCEL / REVERT IF DROPPED IN AN INVALID SPOT
        if (!dropSuccessful)
        {
            if (!match->dragState.isFromRack)
            {
                // Restore tile back to original board slot
                match->board.grid[match->dragState.sourceGridY][match->dragState.sourceGridX] = match->dragState.draggedTile;
            }
        }
        // Reset drag state on mouse release
        match->dragState.isDragging = false;
        match->dragState.isFromRack = false;
        match->dragState.draggedTileIdx = -1;
        match->dragState.draggedTile = (Tile){.letter = '\0', .value = 0, .isWildCard = false};
    }
}

void DrawDragNDropOverlay(const GameState *match, Rectangle rackRect, float tileSize, float tileSpacing)
{
    if (!match || !match->dragState.isDragging)
        return;

    Tile activeTile = match->dragState.draggedTile;
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

    const char *scoreStr = TextFormat("%d", activeTile.value);
    int scoreFontSize = (int)(tileSize * 0.22f);
    int scoreWidth = MeasureText(scoreStr, scoreFontSize);

    DrawText(
        scoreStr,
        dragBounds.x + tileSize - scoreWidth - (tileSize * 0.10f),
        dragBounds.y + tileSize - scoreFontSize - (tileSize * 0.08f),
        scoreFontSize,
        (Color){80, 65, 50, 255});
}