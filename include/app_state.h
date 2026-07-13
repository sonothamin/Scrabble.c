#pragma once

#include <stddef.h>
#include "raylib.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct LoadingState;
struct GameState;

typedef enum AppScreen
{
    APP_SCREEN_LOADING = 0,
    APP_SCREEN_MAIN_MENU,
    APP_SCREEN_SETTINGS,
    APP_SCREEN_GAME,
} AppScreen;

typedef struct AppState
{
    AppScreen currentScreen;
    struct LoadingState* loadingState;
    struct GameState* gamestate;
} AppState;

void InitAppState(AppState* state);
void UpdateAppState(AppState* state);
void DrawAppState(AppState* state);

#if defined(__cplusplus)
}
#endif