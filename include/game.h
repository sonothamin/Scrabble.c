// Tanvir
#pragma once

#include "app_state.h"
#include "board.h" // for CellType

#if defined(__cplusplus)
extern "C"
{
#endif

    struct AppState; // Forward declaration
    typedef struct GameState
    {
        CellType board_layout[BOARD_SIZE][BOARD_SIZE];
        int playerScore;
        int opponentScore;
        int turnCount;
        char currentRank[16];
    } GameState;

    void GameInit(GameState *game);
    void GameUpdate(AppState *state, GameState *game);
    void GameDraw(const GameState *game);
    void GameCleanup(GameState *game);

#if defined(__cplusplus)
}
#endif
