/*
 * drag_drop.c
 *
 * Implements drag-and-drop input handling and overlay rendering for
 * the active player's tile rack.
 *
 * HandleDragNDropInput  — called from game_input.c each frame
 * DrawDragNDropOverlay  — called from game_renderer.c each frame
 */

#include <math.h>
#include "drag_drop.h"
#include "game.h"
#include "raylib.h"

/* ------------------------------------------------------------------ */
/*  HandleDragNDropInput                                                */
/* ------------------------------------------------------------------ */
void HandleDragNDropInput(GameState *match, Rectangle boardBounds, Rectangle rackRect, float tileSize, float tileSpacing)
{
    if (!match)
        return;

    DragNDropState *drag   = &match->dragState;
    Player         *active = &match->players[match->activePlayerIdx];
    Vector2         mouse  = GetMousePosition();

    /* ---- Start drag: left-button pressed over a rack tile ---- */
    if (!drag->isDragging && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int t = 0; t < active->rack_count; t++)
        {
            float tileX = rackRect.x + 15.0f + (t * (tileSize + tileSpacing));
            float tileY = rackRect.y + (rackRect.height - tileSize) / 2.0f + 4.0f;
            Rectangle tileBounds = {tileX, tileY, tileSize, tileSize};

            if (CheckCollisionPointRec(mouse, tileBounds))
            {
                drag->isDragging     = true;
                drag->draggedTileIdx = t;
                break;
            }
        }
    }

    /* ---- Drop: left-button released ---- */
    if (drag->isDragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        /* Check if dropped on a board cell */
        if (CheckCollisionPointRec(mouse, boardBounds))
        {
            float cellSize = boardBounds.width / (float)match->board.sideSize;
            int col = (int)((mouse.x - boardBounds.x) / cellSize);
            int row = (int)((mouse.y - boardBounds.y) / cellSize);

            /* Clamp to valid range */
            if (col >= 0 && col < match->board.sideSize &&
                row >= 0 && row < match->board.sideSize)
            {
                /* Only place if the cell is currently empty */
                if (match->board.placedTiles[row][col].letter == '\0')
                {
                    match->board.placedTiles[row][col] = active->rack[drag->draggedTileIdx];

                    /* Remove tile from rack by shifting remaining tiles left */
                    for (int i = drag->draggedTileIdx; i < active->rack_count - 1; i++)
                        active->rack[i] = active->rack[i + 1];
                    active->rack_count--;
                }
            }
        }

        drag->isDragging     = false;
        drag->draggedTileIdx = -1;
    }
}

/* ------------------------------------------------------------------ */
/*  DrawDragNDropOverlay                                                */
/* ------------------------------------------------------------------ */
void DrawDragNDropOverlay(const GameState *match, Rectangle rackRect, float tileSize, float tileSpacing)
{
    if (!match || !match->dragState.isDragging)
        return;

    const DragNDropState *drag   = &match->dragState;
    const Player         *active = &match->players[match->activePlayerIdx];
    Vector2               mouse  = GetMousePosition();

    if (drag->draggedTileIdx < 0 || drag->draggedTileIdx >= active->rack_count)
        return;

    Tile tile = active->rack[drag->draggedTileIdx];

    /* Draw tile centred on cursor */
    Rectangle tileBounds = {mouse.x - tileSize / 2.0f, mouse.y - tileSize / 2.0f, tileSize, tileSize};
    DrawRectangleRounded(tileBounds, 0.2f, 4, (Color){244, 228, 198, 200});
    DrawRectangleRoundedLines(tileBounds, 0.2f, 4, (Color){194, 169, 126, 255});

    char letterStr[2] = {tile.letter, '\0'};
    int  fontSize     = (int)(tileSize * 0.55f);
    int  textW        = MeasureText(letterStr, fontSize);
    DrawText(letterStr,
             tileBounds.x + (tileSize - textW) / 2.0f,
             tileBounds.y + (tileSize - fontSize) / 2.0f,
             fontSize,
             (Color){38, 28, 16, 220});

    const char *scoreStr  = TextFormat("%d", tile.value);
    int         scoreFontSize = (int)(tileSize * 0.22f);
    int         scoreW    = MeasureText(scoreStr, scoreFontSize);
    DrawText(scoreStr,
             tileBounds.x + tileSize - scoreW - (tileSize * 0.12f),
             tileBounds.y + tileSize - scoreFontSize - (tileSize * 0.10f),
             scoreFontSize,
             (Color){80, 65, 50, 200});

    (void)rackRect;      /* Suppress unused-parameter warning */
    (void)tileSpacing;
}
