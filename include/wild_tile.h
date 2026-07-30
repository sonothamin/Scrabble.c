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
/** If overlay just confirmed, write selectedLetter onto the board cell and clear pending state. */
bool WildTileApplyToBoard(WildTileOverlayState *state, GameBoard *board);
/** Rack form of a tile: wildcards always return as '?' (letter choice is board-only until submit). */
Tile WildTileAsRackTile(Tile tile);

#if defined(__cplusplus)
}
#endif

#endif // WILD_TILE_H
