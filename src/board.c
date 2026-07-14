// Tanvir
#include "board.h"
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "game.h"

void LoadBoardLayout(CellType board_layout[BOARD_SIZE][BOARD_SIZE], const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        file = fopen("resources/board_layout.txt", "r");
    }
    if (file == NULL)
    {
        return;
    }

    char token[4]; // 3 for the type and one is the null character
    int row = 0, col = 0;
    while ((fscanf(file, "%3s", token)) != EOF && (row < BOARD_SIZE))
    {
        if (strcmp(token, "..") == 0)
            board_layout[row][col] = Normal;
        else if (strcmp(token, "2L") == 0)
            board_layout[row][col] = DL;
        else if (strcmp(token, "2W") == 0)
            board_layout[row][col] = DW;
        else if (strcmp(token, "3L") == 0)
            board_layout[row][col] = TL;
        else if (strcmp(token, "3W") == 0)
            board_layout[row][col] = TW;
        else if (strcmp(token, "ST") == 0)
            board_layout[row][col] = DW; // when the 1st word is formed it is automatically double word
        else
            board_layout[row][col] = Normal;
        col++;
        if (col >= BOARD_SIZE)
        {
            col = 0;
            row++;
        }
    }
    fclose(file);
}
void DrawScrabbleBoard(const CellType layout[BOARD_SIZE][BOARD_SIZE])
{

}