// Tanvir
#include "game.h"
#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 15

typedef enum
{
    Normal, // normal = 0
    DL,     // double letter = 1
    DW,     // double word = 2
    TL,     // triple letter = 3
    TW      // triple word = 4
} CellType;

void LoadBoardLayout(CellType board_layout[BOARD_SIZE][BOARD_SIZE], const char *filename);
void DrawScrabbleBoard(const CellType layout[BOARD_SIZE][BOARD_SIZE]);
#endif