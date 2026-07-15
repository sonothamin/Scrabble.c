#include "board.h"
#include <string.h>
#include <stdio.h>

void BoardInit(GameBoard *board, const char *filename)
{
    if (board == NULL)
        return;

    BoardReset(board);
    board->sideSize = BOARD_SIDE;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        file = fopen("resources/board->cells.txt", "r");
    }
    if (file == NULL)
    {
        return;
    }

    char token[4]; // 3 for the type and one is the null character
    int row = 0, col = 0;
    while ((fscanf(file, "%3s", token)) != EOF && (row < BOARD_SIDE))
    {
        if (strcmp(token, "..") == 0)
            board->cells[row][col] = Normal;
        else if (strcmp(token, "2L") == 0)
            board->cells[row][col] = DL;
        else if (strcmp(token, "2W") == 0)
            board->cells[row][col] = DW;
        else if (strcmp(token, "3L") == 0)
            board->cells[row][col] = TL;
        else if (strcmp(token, "3W") == 0)
            board->cells[row][col] = TW;
        else if (strcmp(token, "ST") == 0)
            board->cells[row][col] = DW; // when the 1st word is formed it is automatically double word
        else
            board->cells[row][col] = Normal;
        col++;
        if (col >= BOARD_SIDE)
        {
            col = 0;
            row++;
        }
    }
    fclose(file);
}

void BoardReset(GameBoard *board)
{
    if (board == NULL)
        return;

    memset(board, 0, sizeof(*board));
    board->sideSize = BOARD_SIDE;
}
