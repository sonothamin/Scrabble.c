//Sonoth
#pragma once

#include <stddef.h>
#include "raylib.h"

#if defined(__cplusplus)
extern "C"
{
#endif

    // Forward declaration tells the compiler the type exists
    // without conflicting with the actual structure layout inside loading.h
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
        struct LoadingState *loadingState; // Changed to a pointer to cleanly resolve sizing rules
        struct GameState *gameState;
    } AppState;

    void InitAppState(AppState *state);
    void UpdateAppState(AppState *state);
    void DrawAppState(AppState *state);

#if defined(__cplusplus)
}
#endif