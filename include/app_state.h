#pragma once

#include <stddef.h>
#include "raylib.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum AppScreen
{
    APP_SCREEN_MAIN_MENU = 0,
    APP_SCREEN_SETTINGS,
    APP_SCREEN_GAME,
} AppScreen;

typedef struct AppState
{
    AppScreen currentScreen;
} AppState;

void InitAppState(AppState* state);
void UpdateAppState(AppState* state);
void DrawAppState(const AppState* state);

#if defined(__cplusplus)
}
#endif
