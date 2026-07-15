
#ifndef BOARD_H
#define BOARD_H

#include "tile.h"

#define BOARD_SIDE 15

typedef enum
{
    Normal,
    DL,
    DW,
    TL,
    TW
} CellType;
typedef struct GameBoard
{
    CellType cells[BOARD_SIDE][BOARD_SIDE];
    unsigned char luxuryLayout[BOARD_SIDE][BOARD_SIDE];
    int sideSize;
} GameBoard;

void BoardInit(GameBoard *board, const char *filename);
void DrawTheCellTypes(GameBoard *match, float boardX, float boardY, float boardVisualSize, float cellSize);
void BoardReset(GameBoard *board);

#endif
