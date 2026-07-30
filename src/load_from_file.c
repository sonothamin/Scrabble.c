#include "load_from_file.h"
#include "app_state.h"
#include "game.h"
#include "pause.h"
#include "game_over.h"
#include "ui.h"
#include "sound.h"
#include "settings/settings_internal.h"
#include "raygui.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

bool LoadMatchFromFile(GameState *match, const char *filePath)
{
    if (match == NULL || filePath == NULL || filePath[0] == '\0') return false;

    FILE *f = fopen(filePath, "rb");
    if (!f)
    {
        TraceLog(LOG_ERROR, "[LOAD] Failed to open file for reading: %s", filePath);
        return false;
    }

    char magic[4];
    if (fread(magic, 1, 4, f) != 4 || memcmp(magic, "SCRB", 4) != 0)
    {
        TraceLog(LOG_ERROR, "[LOAD] Invalid file format or magic header in %s", filePath);
        fclose(f);
        return false;
    }

    uint64_t timestamp = 0;
    fread(&timestamp, sizeof(timestamp), 1, f);
    TraceLog(LOG_INFO, "[LOAD] File timestamp: %llu", (unsigned long long)timestamp);

    fread(&match->mode, sizeof(match->mode), 1, f);
    fread(match->dictionaryPath, sizeof(match->dictionaryPath), 1, f);
    fread(&match->specialTilesEnabled, sizeof(match->specialTilesEnabled), 1, f);
    fread(&match->board, sizeof(match->board), 1, f);
    fread(&match->previousBoard, sizeof(match->previousBoard), 1, f);
    fread(match->players, sizeof(match->players), 1, f);
    fread(&match->activePlayerIdx, sizeof(match->activePlayerIdx), 1, f);
    fread(&match->currentTurnNumber, sizeof(match->currentTurnNumber), 1, f);
    fread(&match->tileBag, sizeof(match->tileBag), 1, f);
    fread(&match->tileBagCount, sizeof(match->tileBagCount), 1, f);
    fread(&match->consecutivePassCount, sizeof(match->consecutivePassCount), 1, f);
    fread(&match->isMatchOver, sizeof(match->isMatchOver), 1, f);
    fread(&match->winningPlayerIdx, sizeof(match->winningPlayerIdx), 1, f);

    fclose(f);

    // Re-initialize transient dictionary pointer / data
    memset(&match->dictionary, 0, sizeof(match->dictionary));
    const char *dictPath = (match->dictionaryPath[0] != '\0') ? match->dictionaryPath : "dictionary.txt";
    if (!Load_Dictionary(&match->dictionary, dictPath))
    {
        TraceLog(LOG_WARNING, "[LOAD] Failed to load dictionary file '%s'", dictPath);
    }

    // Reset runtime drag/drop, shuffle, save/exit, and wild overlay states
    match->dragState.isDragging = false;
    match->dragState.draggedTileIdx = -1;
    match->shuffleState.isActive = false;
    SaveAndExitInit(&match->saveExitState);
    WildTileInit(&match->wildTileState);

    TraceLog(LOG_INFO, "[LOAD] Successfully loaded match state from %s", filePath);
    return true;
}

void LoadFromFileInit(LoadFromFileOverlayState *state)
{
    if (state == NULL) return;
    state->isActive = false;
    state->showFileDialog = false;
    memset(&state->fileDialog, 0, sizeof(state->fileDialog));
}

void LoadFromFileUpdate(AppState *appState)
{
    (void)appState;
}

void LoadFromFileDraw(AppState *appState, int screenWidth, int screenHeight, int baseFontSize)
{
    (void)baseFontSize;
    if (appState == NULL || appState->menuLoadState == NULL || !appState->menuLoadState->isActive) return;

    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){10, 15, 22, 215});  // dim background

    if (appState->menuLoadState->showFileDialog)
    {
        GuiWindowFileDialog(&appState->menuLoadState->fileDialog, "Select Saved Match File");

        if (appState->menuLoadState->fileDialog.SelectFilePressed)
        {
            appState->menuLoadState->fileDialog.SelectFilePressed = false;
            appState->menuLoadState->showFileDialog = false;
            appState->menuLoadState->isActive = false;

            char savePath[2048];
            snprintf(savePath, sizeof(savePath), "%s/%s",
                     appState->menuLoadState->fileDialog.dirPathText,
                     appState->menuLoadState->fileDialog.fileNameText);

            if (appState->gamestate != NULL)
            {
                GameCleanUp(appState->gamestate);
                free(appState->gamestate);
                appState->gamestate = NULL;
            }

            appState->gamestate = (GameState *)malloc(sizeof(GameState));
            if (appState->gamestate != NULL)
            {
                memset(appState->gamestate, 0, sizeof(GameState));
                if (LoadMatchFromFile(appState->gamestate, savePath))
                {
                    PlaySoundEffect(SFX_GAME_START);
                    if (appState->pauseState != NULL) InitPauseState(appState->pauseState);
                    if (appState->gameOverState != NULL) InitGameOverState(appState->gameOverState);
                    appState->currentScreen = APP_SCREEN_GAME;
                }
                else
                {
                    PlaySoundEffect(SFX_BACK_NAV);
                    free(appState->gamestate);
                    appState->gamestate = NULL;
                }
            }
            return;
        }
        else if (!appState->menuLoadState->fileDialog.windowActive || appState->menuLoadState->fileDialog.CancelFilePressed)
        {
            appState->menuLoadState->fileDialog.CancelFilePressed = false;
            appState->menuLoadState->showFileDialog = false;
            appState->menuLoadState->isActive = false;
            GuiUnlock();
            PlaySoundEffect(SFX_BACK_NAV);
        }
    }
}
