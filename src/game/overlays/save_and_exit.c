#include "save_and_exit.h"
#include "app_state.h"
#include "game.h"
#include "ui.h"
#include "sound.h"
#include "settings/settings_internal.h"
#include "raygui.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

bool SaveMatchToFile(const GameState *match, const char *filePath)
{
    if (match == NULL || filePath == NULL || filePath[0] == '\0') return false;

    FILE *f = fopen(filePath, "wb");
    if (!f)
    {
        TraceLog(LOG_ERROR, "[SAVE] Failed to open file for writing: %s", filePath);
        return false;
    }

    const char magic[4] = {'S', 'C', 'R', 'B'};
    fwrite(magic, 1, 4, f);

    uint64_t timestamp = (uint64_t)time(NULL);
    fwrite(&timestamp, sizeof(timestamp), 1, f);

    fwrite(&match->mode, sizeof(match->mode), 1, f);
    fwrite(match->dictionaryPath, sizeof(match->dictionaryPath), 1, f);
    fwrite(&match->specialTilesEnabled, sizeof(match->specialTilesEnabled), 1, f);
    fwrite(&match->board, sizeof(match->board), 1, f);
    fwrite(&match->previousBoard, sizeof(match->previousBoard), 1, f);
    fwrite(match->players, sizeof(match->players), 1, f);
    fwrite(&match->activePlayerIdx, sizeof(match->activePlayerIdx), 1, f);
    fwrite(&match->currentTurnNumber, sizeof(match->currentTurnNumber), 1, f);
    fwrite(&match->tileBag, sizeof(match->tileBag), 1, f);
    fwrite(&match->tileBagCount, sizeof(match->tileBagCount), 1, f);
    fwrite(&match->consecutivePassCount, sizeof(match->consecutivePassCount), 1, f);
    fwrite(&match->isMatchOver, sizeof(match->isMatchOver), 1, f);
    fwrite(&match->winningPlayerIdx, sizeof(match->winningPlayerIdx), 1, f);

    fclose(f);
    TraceLog(LOG_INFO, "[SAVE] Successfully saved match state to %s (timestamp: %llu)", filePath, (unsigned long long)timestamp);
    return true;
}

void SaveAndExitInit(SaveAndExitOverlayState *state)
{
    if (state == NULL) return;
    state->isActive = false;
    state->showFileDialog = false;
    memset(&state->fileDialog, 0, sizeof(state->fileDialog));
}

void SaveAndExitUpdate(AppState *appState, GameState *match)
{
    (void)appState;
    (void)match;
}

void SaveAndExitDraw(AppState *appState, GameState *match, int screenWidth, int screenHeight, int baseFontSize)
{
    if (appState == NULL || match == NULL || !match->saveExitState.isActive) return;

    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){10, 15, 22, 215});  //dim gui

    if (match->saveExitState.showFileDialog) //file dialog
    {
        GuiWindowFileDialog(&match->saveExitState.fileDialog, "Save Match File");

        if (match->saveExitState.fileDialog.SelectFilePressed)
        {
            match->saveExitState.fileDialog.SelectFilePressed = false;
            match->saveExitState.showFileDialog = false;
            match->saveExitState.isActive = false;

            char savePath[2048];
            snprintf(savePath, sizeof(savePath), "%s/%s",
                     match->saveExitState.fileDialog.dirPathText,
                     match->saveExitState.fileDialog.fileNameText);

            SaveMatchToFile(match, savePath);

            PlaySoundEffect(SFX_BUTTON);
            appState->currentScreen = APP_SCREEN_MAIN_MENU;
            return;
        }
        else if (!match->saveExitState.fileDialog.windowActive || match->saveExitState.fileDialog.CancelFilePressed)
        {
            match->saveExitState.fileDialog.CancelFilePressed = false;
            match->saveExitState.showFileDialog = false;
            GuiUnlock();
            PlaySoundEffect(SFX_BACK_NAV);
        }
        return;
    }

    // Modal Card Layout
    float cardW = fminf(screenWidth * 0.52f, 540.0f);
    if (cardW < 400.0f) cardW = 400.0f;
    float cardH = 340.0f;
    float cardX = (screenWidth - cardW) / 2.0f;
    float cardY = (screenHeight - cardH) / 2.0f;
    Rectangle cardRect = {cardX, cardY, cardW, cardH};

    // Glassmorphism & Shadow Style
    DrawRectangleRounded((Rectangle){cardX + 6, cardY + 8, cardW, cardH}, 0.08f, 6, (Color){0, 0, 0, 150});
    DrawRectangleRounded(cardRect, 0.08f, 6, (Color){20, 28, 36, 255});
    DrawRectangleRoundedLinesEx(cardRect, 0.08f, 6, 2.0f, (Color){54, 72, 90, 255});

    float titleY = cardY + 22.0f;
    const char *titleText = "Are you sure?";
    int titleFontSize = (int)(baseFontSize * 1.6f);
    int titleW = MeasureAppText(titleText, titleFontSize);
    DrawAppText(titleText, cardX + (cardW - titleW) / 2.0f, titleY, titleFontSize, (Color){244, 228, 198, 255});

    DrawLineV((Vector2){cardX + 30.0f, titleY + titleFontSize + 10.0f},
              (Vector2){cardX + cardW - 30.0f, titleY + titleFontSize + 10.0f},
              (Color){46, 202, 113, 255});

    float btnW = cardW - 60.0f;
    float btnH = 42.0f;
    float btnGap = 12.0f;
    float startBtnY = titleY + titleFontSize + 25.0f;

    Rectangle btnSaveExit = {cardX + 30.0f, startBtnY, btnW, btnH};
    Rectangle btnExitNoSave = {cardX + 30.0f, startBtnY + btnH + btnGap, btnW, btnH};
    Rectangle btnCancel = {cardX + 30.0f, startBtnY + (btnH + btnGap) * 2.0f, btnW, btnH};

    bool triggerSaveExit = GuiButton(btnSaveExit, "Save & Exit");
    bool triggerExitNoSave = GuiButton(btnExitNoSave, "Exit Without Saving");
    bool triggerCancel = GuiButton(btnCancel, "Cancel");

    static const HotkeyEntry overlayKeys[] = {
        {"S", "Save & Exit"},
        {"E", "Exit No Save"},
        {"C", "Cancel"},
        {"ESC", "Back"}
    };
    float hkBarH = baseFontSize * 2.0f;
    float modalHkBarY = cardY + cardH - hkBarH - 12.0f;
    DrawHotkeyBar(overlayKeys, 4, cardX + 30.0f, modalHkBarY, btnW, hkBarH, baseFontSize);

    if (triggerSaveExit || IsKeyPressed(KEY_S))
    {
        PlaySoundEffect(SFX_BUTTON);
        const char *cfgPath = GetConfigFilePath();
        const char *saveDirPath = GetDirectoryPath(cfgPath);
        TraceLog(LOG_INFO, "[DEBUG] Config File Path: %s", cfgPath);
        TraceLog(LOG_INFO, "[DEBUG] Save Directory Path: %s", saveDirPath);

        match->saveExitState.fileDialog = InitGuiWindowFileDialog(saveDirPath);
        match->saveExitState.fileDialog.windowActive = true;
        match->saveExitState.fileDialog.saveFileMode = true;
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char defaultName[128];
        if (t != NULL)
        {
            strftime(defaultName, sizeof(defaultName), "scrabble_save_%Y%m%d_%H%M%S.sav", t);
        }
        else
        {
            snprintf(defaultName, sizeof(defaultName), "scrabble_match_%ld.sav", (long)now);
        }

        strcpy(match->saveExitState.fileDialog.fileNameText, defaultName);
        match->saveExitState.showFileDialog = true;
    }
    else if (triggerExitNoSave || IsKeyPressed(KEY_E))
    {
        PlaySoundEffect(SFX_BACK_NAV);
        match->saveExitState.isActive = false;
        appState->currentScreen = APP_SCREEN_MAIN_MENU;
    }
    else if (triggerCancel || IsKeyPressed(KEY_C) || IsKeyPressed(KEY_ESCAPE))
    {
        PlaySoundEffect(SFX_BACK_NAV);
        match->saveExitState.isActive = false;
    }
}
