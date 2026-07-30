#ifndef SAVE_AND_EXIT_H
#define SAVE_AND_EXIT_H

#include "raylib.h"
#include "gui_window_file_dialog.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct AppState AppState;
typedef struct GameState GameState;

typedef struct SaveAndExitOverlayState {
    bool isActive;
    bool showFileDialog;
    GuiWindowFileDialogState fileDialog;
} SaveAndExitOverlayState;

void SaveAndExitInit(SaveAndExitOverlayState *state);
void SaveAndExitUpdate(AppState *appState, GameState *match);
void SaveAndExitDraw(AppState *appState, GameState *match, int screenWidth, int screenHeight, int baseFontSize);
bool SaveMatchToFile(const GameState *match, const char *filePath);

#if defined(__cplusplus)
}
#endif

#endif // SAVE_AND_EXIT_H
