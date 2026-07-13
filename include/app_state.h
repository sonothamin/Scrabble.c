#pragma once

#include <stddef.h>
#include "raylib.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct LoadingState LoadingState;
typedef struct AboutState AboutState;

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
} AppState;

void InitAppState(AppState* state);
void UpdateAppState(AppState* state);
void DrawAppState(AppState* state);
void CloseAppState(AppState* state);

#if defined(__cplusplus)
}
#endif