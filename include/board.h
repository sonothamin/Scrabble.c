#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 15

typedef enum
{
    Normal, // normal = 0
    DL, // double letter = 1
    DW, // double word = 2
    TL, //triple letter = 3
    TW // triple word = 4
}CellType;

void InitScrabbleBoard(const char *filename);
void DrawScrabbleBoard(void);
#endif