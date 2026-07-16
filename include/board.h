
#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIDE 15

typedef enum
{
    LUXURY_NONE = 0,
    LUXURY_DOUBLE_LETTER,
    LUXURY_TRIPLE_LETTER,
    LUXURY_DOUBLE_WORD,
    LUXURY_TRIPLE_WORD
} LuxuryType;

typedef struct Tile
{
    char letter;
    int value;
} Tile;

typedef struct GameBoard
{
    LuxuryType cells[BOARD_SIDE][BOARD_SIDE];
    unsigned char luxuryLayout[BOARD_SIDE][BOARD_SIDE];
    int sideSize;
} GameBoard;

void BoardInit(GameBoard *board, const char *filename);
void DrawTheCellTypes(GameBoard *match, float boardX, float boardY, float boardVisualSize, float cellSize);
void BoardReset(GameBoard *board);

#endif
