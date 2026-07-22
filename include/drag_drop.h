#ifndef DRAG_DROP_H
#define DRAG_DROP_H

#include "raylib.h"
#include <stdbool.h>

#define BOARD_SIZE 15
#define MAX_RACK_TILES 7

// 1. Isolated Interactive State Structure
typedef struct {
    bool isDragging;        // True ONLY when mouse is actively holding a tile
    int draggedTileIdx;     // Tracks the index (0-6) of the tile from the active player's rack
} DragNDropState;

/* Forward declaration — avoids a circular include with game.h */
typedef struct GameState GameState;

// Function Declarations
void HandleDragNDropInput(GameState *match, Rectangle boardBounds, Rectangle rackRect, float tileSize, float tileSpacing);
void DrawDragNDropOverlay(const GameState *match, Rectangle rackRect, float tileSize, float tileSpacing);

#endif // DRAG_DROP_H