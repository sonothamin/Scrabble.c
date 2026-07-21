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

typedef enum {
    APP_SCREEN_LOADING = 0,
    APP_SCREEN_MAIN_MENU,
    APP_SCREEN_GAME,
    APP_SCREEN_SETTINGS,
    APP_SCREEN_ABOUT
} AppScreen;

typedef struct AppState
{
    AppScreen currentScreen;
    LoadingState* loadingState;
    AboutState* aboutState;
    GameState* gamestate;
    SettingsState* settingsState;
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