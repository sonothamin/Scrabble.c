#include "board.h"
#include <string.h>

void BoardInit(GameBoard* board)
{
    if (board == NULL)
        return;

    BoardReset(board);
    board->sideSize = BOARD_SIDE;
}

void BoardReset(GameBoard* board)
{
    if (board == NULL)
        return;

    memset(board, 0, sizeof(*board));
    board->sideSize = BOARD_SIDE;
}
