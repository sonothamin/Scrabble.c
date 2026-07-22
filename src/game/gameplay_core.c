/*
 * gameplay_core.c
 *
 * Central coordinator for the game screen lifecycle.
 * Owns GameInit, GameUpdate, and GameDraw — the three entry-points
 * called by app_state.c.  All heavy lifting is delegated to:
 *   game_renderer.c  — everything that touches Draw* / Gui*
 *   game_input.c     — everything that processes mouse / keyboard input
 */

#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "app_state.h"
#include "raylib.h"
#include "sound.h"
#include "error_service.h"
#include "bag.h"

/* Forward declarations for the sibling modules in this folder */
void GameRenderFrame(AppState *state);
void GameHandleInput(AppState *state);

/* ------------------------------------------------------------------ */
/*  GameInit                                                            */
/* ------------------------------------------------------------------ */
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

    /* Reset Drag and Drop state */
    match->dragState.isDragging = false;
    match->dragState.draggedTileIdx = -1;
    PlaySoundEffect(SFX_GAME_START);
}

/* ------------------------------------------------------------------ */
/*  GameUpdate                                                          */
/* ------------------------------------------------------------------ */
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

    if (state->gamestate == NULL)
        return;

    /* Delegate all input + drag-drop processing */
    GameHandleInput(state);
}

/* ------------------------------------------------------------------ */
/*  GameDraw                                                            */
/* ------------------------------------------------------------------ */
void GameDraw(AppState *state)
{
    if (!state)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered while drawing Game.");
        return;
    }

    /* Lazy-init the game state the first time the screen is drawn */
    if (state->gamestate == NULL)
    {
        state->gamestate = (GameState *)malloc(sizeof(GameState));
        if (state->gamestate != NULL)
            GameInit(state->gamestate);
    }

    if (!state->gamestate)
        return;

    /* Delegate all rendering */
    GameRenderFrame(state);
}
