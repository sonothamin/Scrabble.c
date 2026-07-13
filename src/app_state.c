#include <stddef.h>
#include <stdlib.h>
#include "app_state.h"
#include "ui.h"
#include "menu.h"
#include "settings.h"
#include "game.h"
#include "loading.h"
#include "about.h"
#include "raylib.h"

void InitAppState(AppState *state)
{
    if (state == NULL)
        return;

    state->currentScreen = APP_SCREEN_LOADING;

    // Allocate Loading State memory
    state->loadingState = (LoadingState *)malloc(sizeof(LoadingState));
    if (state->loadingState != NULL)
        LoadingInit(state->loadingState);

    state->aboutState = (AboutState *)malloc(sizeof(AboutState));
    if (state->aboutState != NULL)
    {
        // AboutInit(state->aboutState);
    }
}

void UpdateAppState(AppState *state)
{
    if (state == NULL)
        return;

    switch (state->currentScreen)
    {
    case APP_SCREEN_LOADING:
        if (state->loadingState != NULL)
        {
            LoadingUpdate(state, state->loadingState, GetFrameTime());
        }
        break;

    case APP_SCREEN_MAIN_MENU:
        MenuUpdate(state);
        break;

    case APP_SCREEN_SETTINGS:
        SettingsUpdate(state);
        break;

    case APP_SCREEN_GAME:
        GameUpdate(state);
        break;

    case APP_SCREEN_ABOUT:
        if (state->aboutState != NULL)
        {
            AboutUpdate(state, state->aboutState);
        }
        break;

    default:
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        break;
    }
}

void DrawAppState(AppState *state)
{
    if (state == NULL)
        return;

    ClearBackground(RAYWHITE);

    switch (state->currentScreen)
    {
    case APP_SCREEN_LOADING:
        if (state->loadingState != NULL)
        {
            LoadingDraw(state, state->loadingState);
        }
        break;

    case APP_SCREEN_MAIN_MENU:
        MenuDraw(state);
        break;

    case APP_SCREEN_SETTINGS:
        SettingsDraw();
        break;

    case APP_SCREEN_GAME:
        GameDraw();
        break;

    case APP_SCREEN_ABOUT:
        if (state->aboutState != NULL)
            AboutDraw(state, state->aboutState);
        break;

    default:
        MenuDraw(state);
        break;
    }
}

void CloseAppState(AppState *state)
{
    if (state == NULL)
        return;
    if (state->loadingState)
        free(state->loadingState);
    if (state->aboutState)
        free(state->aboutState);
}