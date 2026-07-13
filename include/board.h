#ifndef BOARD_H
#define BOARD_H

#include "tile.h"

#define BOARD_SIDE 15

typedef struct GameBoard
{
    Tile cells[BOARD_SIDE][BOARD_SIDE];
    unsigned char luxuryLayout[BOARD_SIDE][BOARD_SIDE];
    int sideSize;
} GameBoard;

void BoardInit(GameBoard* board);
void BoardReset(GameBoard* board);

#endif
