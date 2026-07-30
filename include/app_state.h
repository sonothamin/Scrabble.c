#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "raylib.h"
#include "game.h"

#if defined(__cplusplus)
extern "C" {
#endif

// Forward declarations
typedef struct LoadingState LoadingState;
typedef struct AboutState AboutState;
typedef struct SettingsState SettingsState;
typedef struct PauseState PauseState;
typedef struct GameOverState GameOverState;

typedef enum {
    APP_SCREEN_LOADING = 0,
    APP_SCREEN_MAIN_MENU,
    APP_SCREEN_GAME,
    APP_SCREEN_SETTINGS,
    APP_SCREEN_ABOUT
} AppScreen;

#include "load_from_file.h"

typedef struct AppState
{
    AppScreen currentScreen;
    LoadingState* loadingState;
    AboutState* aboutState;
    GameState* gamestate;
    SettingsState* settingsState;
    PauseState* pauseState;
    GameOverState* gameOverState;
    LoadFromFileOverlayState* menuLoadState;
    bool shouldClose;
} AppState;

void InitAppState(AppState* state);
void UpdateAppState(AppState* state);
void DrawAppState(AppState* state);
void CloseAppState(AppState* state);
void StartNewGame(AppState* state);

#if defined(__cplusplus)
}
#endif