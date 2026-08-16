#include "game.h"
#include "pause.h"
#include "ui.h"
#include "app_state.h"
#include "raylib.h"
#include "raygui.h"
#include "sound.h"
#include "error_service.h"
#include "bag.h"
#include "drag_drop.h"
#include "settings.h"
#include "shuffle.h"

#include "game_layout.h"
#include "game_actions.h"
#include "game_render_components.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

void GameInit(GameState *match)
{
    if (match == NULL)
        return;

    memset(match, 0, sizeof(*match));
    match->mode = GAME_MODE_LOCAL_1V1;
    match->specialTilesEnabled = true;

    if (!Load_Dictionary(&match->dictionary, "dictionary.txt"))
    {
        TraceLog(LOG_WARNING, "Failed to load dictionary file! Check path.");
    }

    BoardInit(&match->board, "board_layout.txt");
    memcpy(&match->previousBoard, &match->board, sizeof(GameBoard));
    init_player(&match->players[0]);
    init_player(&match->players[1]);
    init_bag_from_file(&match->tileBag);
    shuffle_tiles_in_bag(&match->tileBag);
    refill_rack(&match->players[0], &match->tileBag);
    refill_rack(&match->players[1], &match->tileBag);
    match->activePlayerIdx = 0;
    match->tileBagCount = match->tileBag.tiles_remaining;
    match->consecutivePassCount = 0;

    match->dragState.isDragging = false;
    match->dragState.draggedTileIdx = -1;

    SaveAndExitInit(&match->saveExitState);
    WildTileInit(&match->wildTileState);

    PlaySoundEffect(SFX_GAME_START);
}

void GameCleanUp(GameState *match)
{
    if (match != NULL)
    {
        Free_Dictionary(&match->dictionary);
    }
}

void GameUpdate(AppState *state)
{
    if (!state)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered while updating Game.");
        return;
    }

    if (state->gamestate != NULL && state->gamestate->saveExitState.isActive)
        return;

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))
    {
        if (state->gamestate == NULL || !state->gamestate->shuffleState.isActive)
        {
            if (state->pauseState)
            {
                state->pauseState->isPaused = !state->pauseState->isPaused;
                PlaySoundEffect(SFX_BUTTON);
            }
            return;
        }
    }

    if (IsKeyPressed(KEY_M) && state->settingsState != NULL)
    {
        state->settingsState->bgmEnable = !state->settingsState->bgmEnable;
        SetMusicVolumeLevel(state->settingsState->bgmEnable ? state->settingsState->bgmVolume : 0.0f);
        PlaySoundEffect(SFX_BUTTON);
    }

    if (IsKeyPressed(KEY_Q))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        PlaySoundEffect(SFX_BACK_NAV);
        return;
    }

    if (state->gamestate == NULL)
    {
        ReportCriticalError("Invalid Game State", "NULL GameState pointer encountered in AppState while updating Game.");
        return;
    }

    GameState *match = state->gamestate;
    int baseFontSize = fmaxf(15, GetScreenHeight() / 42);
    GameLayout layout = CalculateGameLayout(GetScreenWidth(), GetScreenHeight(), baseFontSize);

    if (match->wildTileState.isActive)
        WildTileUpdate(&match->wildTileState);

    WildTileApplyToBoard(&match->wildTileState, &match->board);
    WildTileReturnCancelled(&match->wildTileState, &match->board, &match->players[match->activePlayerIdx]);

    if (match->wildTileState.isActive)
        return;

    if (!match->shuffleState.isActive)
        HandleDragNDropInput(match, layout.boardBounds, layout.rackRect, layout.activeTileSize, layout.activeTileSpacing);

    if (!match->shuffleState.isActive && IsKeyPressed(KEY_S))
    {
        Action_PassTurn(match);
    }

    if (!match->shuffleState.isActive && IsKeyPressed(KEY_Z))
    {
        Action_UndoTurn(match);
    }

    if (match->shuffleState.isActive)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ShuffleCancel(&match->shuffleState);
            return;
        }
        if (IsKeyPressed(KEY_C) && match->shuffleState.selectedCount > 0)
        {
            if (ShuffleCommit(&match->shuffleState, &match->players[match->activePlayerIdx], &match->tileBag, &match->tileBagCount))
            {
                memcpy(&match->board, &match->previousBoard, sizeof(GameBoard));
                match->consecutivePassCount++;
                if (match->consecutivePassCount >= 6)
                {
                    Action_FinalizeGameOver(match);
                }
                else
                {
                    match->activePlayerIdx = (match->activePlayerIdx + 1) % 2;
                }
            }
            return;
        }

        ShuffleUpdate(&match->shuffleState, &match->players[match->activePlayerIdx], GetScreenWidth(), GetScreenHeight(), baseFontSize);
    }

    Vector2 mousePos = GetMousePosition();
    if (!match->shuffleState.isActive && CheckCollisionPointRec(mousePos, layout.submitBtnRect))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Action_SubmitMove(match);
        }
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
        return;

    GameState *match = state->gamestate;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    ClearBackground((Color){24, 32, 38, 255});
    int baseFontSize = fmaxf(15, screenHeight / 42);
    ApplyScrabbleTheme(baseFontSize);

    GameLayout layout = CalculateGameLayout(screenWidth, screenHeight, baseFontSize);

    const bool wildOverlayActive = match->wildTileState.isActive;
    if (wildOverlayActive)
        GuiLock();

    // Render components
    Draw_BoardView(match, &layout);
    Draw_HotkeyView(state, &layout, baseFontSize);
    Draw_MatchStatusView(match, &layout);
    Draw_ScoreBoardView(match, &layout, baseFontSize);
    Draw_RackView(match, &layout, baseFontSize);
    Draw_ActionBarView(match, &layout, baseFontSize);
    Draw_HistoryView(&layout, baseFontSize);
    Draw_FooterView(match, &layout, baseFontSize);

    // Render Drag Overlay
    DrawDragNDropOverlay(match, layout.rackRect, layout.activeTileSize, layout.activeTileSpacing);

    // Overlays
    if (match->shuffleState.isActive)
    {
        int shuffleResult = ShuffleDraw(&match->shuffleState, &match->players[match->activePlayerIdx], screenWidth, screenHeight, baseFontSize);
        if (shuffleResult == 1 && match->shuffleState.selectedCount > 0)
        {
            if (ShuffleCommit(&match->shuffleState, &match->players[match->activePlayerIdx], &match->tileBag, &match->tileBagCount))
            {
                memcpy(&match->board, &match->previousBoard, sizeof(GameBoard));
                match->consecutivePassCount++;
                if (match->consecutivePassCount >= 6)
                {
                    Action_FinalizeGameOver(match);
                }
                else
                {
                    match->activePlayerIdx = (match->activePlayerIdx + 1) % 2;
                }
            }
        }
        else if (shuffleResult == -1)
        {
            ShuffleCancel(&match->shuffleState);
        }
    }

    SaveAndExitDraw(state, match, screenWidth, screenHeight, baseFontSize);

    if (wildOverlayActive)
        GuiUnlock();

    WildTileDraw(&match->wildTileState, screenWidth, screenHeight, baseFontSize);
    WildTileApplyToBoard(&match->wildTileState, &match->board);
    WildTileReturnCancelled(&match->wildTileState, &match->board, &match->players[match->activePlayerIdx]);
}