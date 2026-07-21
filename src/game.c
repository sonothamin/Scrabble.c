#include "game.h"
#include "ui.h"
#include "app_state.h"
#include "raylib.h"
#include "raygui.h"
#include "error_service.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "bag.h"

void GameInit(GameState *match)
{
    if (match == NULL)
        return;

    memset(match, 0, sizeof(*match));
    match->mode = GAME_MODE_LOCAL_1V1;
    match->specialTilesEnabled = true;
    BoardInit(&match->board, "board_layout.txt");
    init_player(&match->players[0]);
    init_player(&match->players[1]);
    init_bag_from_file(&match->tileBag);
    shuffle_tiles_in_bag(&match->tileBag);
    refill_rack(&match->players[0], &match->tileBag);
    refill_rack(&match->players[1], &match->tileBag);
    match->activePlayerIdx = 0;
    match->tileBagCount = match->tileBag.tiles_remaining;
    match->currentSubScreen = GAME_SUB_SCREEN_PLAYING;
}

void GameUpdate(AppState *state)
{
    if (!state)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered while updating Game.");
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        return;
    }
}

void GameDraw(AppState *state)
{
    if (!state)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered while drawing Game.");
        return;
    }

    if (state->gamestate == NULL)
    {
        state->gamestate = (GameState *)malloc(sizeof(GameState));
        if (state->gamestate != NULL)
            GameInit(state->gamestate);
    }

    GameState *match = state->gamestate;

    switch (match->currentSubScreen)
    {
    case GAME_SUB_SCREEN_PLAYING:
    case GAME_SUB_SCREEN_PAUSED:
    case GAME_SUB_SCREEN_MATCH_OVER:
        DrawGamePlayView(state, match);
        break;
    }
}