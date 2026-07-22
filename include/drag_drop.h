#ifndef DRAG_DROP_H
#define DRAG_DROP_H

#include "raylib.h"
#include <stdbool.h>

typedef struct GameState GameState; // Forward Declaration

typedef struct DragNDropState
{
    bool isDragging;
    int draggedTileIdx;
    bool isFromRack;
    int sourceGridX;
    int sourceGridY;
    Tile draggedTile;
} DragNDropState;

void HandleDragNDropInput(GameState *match, Rectangle boardBounds, Rectangle rackRect, float tileSize, float tileSpacing);
void DrawDragNDropOverlay(const GameState *match, Rectangle rackRect, float tileSize, float tileSpacing);

#endif