#ifndef DRAG_DROP_H
#define DRAG_DROP_H

#include "raylib.h"
#include <stdbool.h>
#include"game.h"

#define BOARD_SIZE 15
#define MAX_RACK_TILES 7

typedef struct {
    bool isDragging;        // actively holding a tile
    int draggedTileIdx;     // index (0-6) of the tile from the active player's rack
} DragNDropState;

// Function Declarations
void HandleDragNDropInput(GameState *match, Rectangle boardBounds, Rectangle rackRect, float tileSize, float tileSpacing);
void DrawDragNDropOverlay(const GameState *match, Rectangle rackRect, float tileSize, float tileSpacing);

#endif // DRAG_DROP_H