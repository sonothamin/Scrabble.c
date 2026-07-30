#ifndef WILD_TILE_H
#define WILD_TILE_H

#include <stdbool.h>
#include "raylib.h"
#include "board.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct WildTileOverlayState
{
    bool isActive;
    int targetGridX;
    int targetGridY;
    char selectedLetter; // 'A' through 'Z'
    int hoverIndex;      // Index 0-25 for grid/keyboard selection
} WildTileOverlayState;

void WildTileInit(WildTileOverlayState *state);
void WildTileOpen(WildTileOverlayState *state, int gridX, int gridY);
void WildTileCancel(WildTileOverlayState *state);
void WildTileUpdate(WildTileOverlayState *state);
void WildTileDraw(WildTileOverlayState *state, int screenWidth, int screenHeight, int baseFontSize);

#if defined(__cplusplus)
}
#endif

#endif // WILD_TILE_H
