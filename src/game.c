// Tanvir
#include "game.h"
#include "ui.h"
#include "app_state.h"
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void GameInit(GameState *game)
{
    if (game == NULL)
        return; // the memory is not allocated yet

    game->playerScore = 0;
    game->opponentScore = 0;
    game->turnCount = 1;
    snprintf(game->currentRank, sizeof(game->currentRank), "Novice");
    LoadBoardLayout(game->board_layout, "board_layout.txt");
}

void GameUpdate(AppState *state, GameState *game)
{
    if (state == NULL || game == NULL)
    {
        return;
    }

    if (IsKeyPressed(KEY_B))
    {
        GameCleanUp(game);       // before exit clean up the assests and nested resources
        free(state->gameState);  // before exit clean up the RAM
        state->gameState = NULL; // for safety make the pointer zero
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void GameDraw(const GameState *game)
{
    if (game == NULL)
        return; // for safety, if the memory is not allocated

    DrawScrabbleBoard(game->board_layout);
}
