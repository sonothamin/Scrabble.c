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

// Minimal data mocks to ensure compilation independent of your codebase structures
typedef struct {
    char letter;
    int value;
} Tile;

typedef struct {
    Tile grid[BOARD_SIZE][BOARD_SIZE]; 
} GameBoard;

typedef struct {
    Tile rack[MAX_RACK_TILES];
    int rack_count;
} Player;

typedef struct {
    GameBoard board;
    Player players[2];
    int activePlayerIdx;
    DragNDropState dragState;
} GameState;

// Function Declarations
void HandleDragNDropInput(GameState *match, Rectangle boardBounds, Rectangle rackRect, float tileSize, float tileSpacing);
void DrawDragNDropOverlay(const GameState *match, Rectangle rackRect, float tileSize, float tileSpacing);

#endif // DRAG_DROP_H