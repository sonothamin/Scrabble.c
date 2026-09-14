#ifndef WILD_TILE_H
#define WILD_TILE_H

#include <stdbool.h>
#include "raylib.h"
#include "board.h"
#include "player.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct WildTileOverlayState
{
    bool isActive;
    int targetGridX;
    int targetGridY;
    char selectedLetter;
    int hoverIndex;
} WildTileOverlayState;

void WildTileInit(WildTileOverlayState *state);
void WildTileOpen(WildTileOverlayState *state, int gridX, int gridY);
void WildTileCancel(WildTileOverlayState *state);
void WildTileUpdate(WildTileOverlayState *state);
void WildTileDraw(WildTileOverlayState *state, int screenWidth, int screenHeight, int baseFontSize);
bool WildTileApplyToBoard(WildTileOverlayState *state, GameBoard *board);
bool WildTileReturnCancelled(WildTileOverlayState *state, GameBoard *board, Player *player);
Tile WildTileAsRackTile(Tile tile);
bool WildTileHasUnassignedOnBoard(const GameBoard *board, const GameBoard *previousBoard);

#if defined(__cplusplus)
}
#endif

#endif // WILD_TILE_H
