#ifndef LOAD_FROM_FILE_H
#define LOAD_FROM_FILE_H

#include "raylib.h"
#include "gui_window_file_dialog.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct AppState AppState;
typedef struct GameState GameState;

typedef struct LoadFromFileOverlayState {
    bool isActive;
    bool showFileDialog;
    GuiWindowFileDialogState fileDialog;
} LoadFromFileOverlayState;

void LoadFromFileInit(LoadFromFileOverlayState *state);
void LoadFromFileUpdate(AppState *appState);
void LoadFromFileDraw(AppState *appState, int screenWidth, int screenHeight, int baseFontSize);
bool LoadMatchFromFile(GameState *match, const char *filePath);

#if defined(__cplusplus)
}
#endif

#endif // LOAD_FROM_FILE_H
