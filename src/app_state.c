#include <stddef.h>
#include <stdlib.h>
#include "app_state.h"
#include "ui.h"
#include "menu.h"
#include "settings.h"
#include "game.h"
#include "loading.h"
#include "about.h"
#include "pause.h"
#include "game_over.h"
#include "raylib.h"
#include "raygui.h"
#include "error_service.h"
#include "settings/settings_internal.h"

void InitAppState(AppState *state)
{
    if (state == NULL)
    {
        ReportCriticalError("Initialization Failure", "NULL AppState pointer provided to InitAppState.");
        return;
    }

    state->currentScreen = APP_SCREEN_LOADING;
    state->loadingState = (LoadingState *)malloc(sizeof(LoadingState));
    if (state->loadingState != NULL)
        LoadingInit(state->loadingState);
    else
    {
        ReportCriticalError("Memory Allocation Error", "Failed to allocate memory for LoadingState.");
        return;
    }

    state->aboutState = (AboutState *)malloc(sizeof(AboutState));
    if (state->aboutState != NULL)
        AboutInit(state->aboutState);
    else
    {
        ReportCriticalError("Memory Allocation Error", "Failed to allocate memory for AboutState.");
    }

    state->gamestate = NULL;

    state->settingsState = InitSettingsState();
    if (state->settingsState == NULL)
    {
        ReportCriticalError("Memory Allocation Error", "Failed to initialize SettingsState.");
    }

    state->pauseState = (PauseState *)malloc(sizeof(PauseState));
    if (state->pauseState != NULL)
        InitPauseState(state->pauseState);
    else
    {
        ReportCriticalError("Memory Allocation Error", "Failed to allocate memory for PauseState.");
    }

    state->gameOverState = (GameOverState *)malloc(sizeof(GameOverState));
    if (state->gameOverState != NULL)
        InitGameOverState(state->gameOverState);
    else
    {
        ReportCriticalError("Memory Allocation Error", "Failed to allocate memory for GameOverState.");
    }

    state->menuLoadState = (LoadFromFileOverlayState *)malloc(sizeof(LoadFromFileOverlayState));
    if (state->menuLoadState != NULL)
        LoadFromFileInit(state->menuLoadState);
    else
    {
        ReportCriticalError("Memory Allocation Error", "Failed to allocate memory for MenuLoadState.");
    }

    if (state->settingsState != NULL && FileExists(CONFIG_FILE_PATH))
    {
        if (!LoadSettingsFromFile(state->settingsState, CONFIG_FILE_PATH))
        {
            TraceLog(LOG_WARNING, "APP_STATE: Failed to read %s despite file existing. Using defaults.", CONFIG_FILE_PATH);
            ReportCriticalError("Settings State Error", "Failed to read file despite file existing. Using defaults. See log for path.");
        }
    }

    if (state->settingsState != NULL && !state->settingsState->showLoadingScreen)
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void StartNewGame(AppState *state)
{
    if (state == NULL)
        return;

    if (state->gamestate != NULL)
    {
        GameCleanUp(state->gamestate);
        free(state->gamestate);
        state->gamestate = NULL;
    }

    state->gamestate = (GameState *)malloc(sizeof(GameState));
    if (state->gamestate != NULL)
    {
        GameInit(state->gamestate);
    }

    if (state->pauseState != NULL)
    {
        InitPauseState(state->pauseState);
    }
    if (state->gameOverState != NULL)
    {
        InitGameOverState(state->gameOverState);
    }

    state->currentScreen = APP_SCREEN_GAME;
}

void UpdateAppState(AppState *state)
{
    UpdateErrorService(GetFrameTime());

    if (state == NULL)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered while updating app state.");
        return;
    }

    switch (state->currentScreen)
    {
    case APP_SCREEN_LOADING:
        if (state->loadingState != NULL)
            LoadingUpdate(state, state->loadingState, GetFrameTime());
        break;

    case APP_SCREEN_MAIN_MENU:
        MenuUpdate(state);
        break;

    case APP_SCREEN_SETTINGS:
        SettingsUpdate(state);
        break;

    case APP_SCREEN_GAME:
        if (state->gamestate == NULL)
        {
            state->gamestate = (GameState *)malloc(sizeof(GameState));
            if (state->gamestate != NULL)
                GameInit(state->gamestate);
        }

        if (state->pauseState != NULL)
            UpdatePauseOverlay(state, state->pauseState);

        if (state->gameOverState != NULL)
            UpdateGameOverOverlay(state, state->gameOverState);

        // Only update game board & interactions if not paused and not game over
        if ((state->pauseState == NULL || !state->pauseState->isPaused) &&
            (state->gamestate == NULL || !state->gamestate->isMatchOver))
        {
            GameUpdate(state);
        }
        break;

    case APP_SCREEN_ABOUT:
        if (state->aboutState != NULL)
            AboutUpdate(state, state->aboutState);
        break;

    default:
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        break;
    }
}

void DrawAppState(AppState *state)
{
    if (state == NULL)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered while drawing app state.");
        return;
    }

    ClearBackground(RAYWHITE);

    switch (state->currentScreen)
    {
    case APP_SCREEN_LOADING:
        if (state->loadingState != NULL)
            LoadingDraw(state, state->loadingState);
        break;

    case APP_SCREEN_MAIN_MENU:
        MenuDraw(state);
        break;

    case APP_SCREEN_SETTINGS:
        SettingsDraw(state);
        break;

    case APP_SCREEN_GAME:
        GameDraw(state);

        if (state->pauseState != NULL && state->pauseState->isPaused)
        {
            DrawPauseOverlay(state, state->pauseState);
        }
        else if (state->gamestate != NULL && state->gamestate->isMatchOver && state->gameOverState != NULL)
        {
            DrawGameOverOverlay(state, state->gameOverState);
        }
        break;

    case APP_SCREEN_ABOUT:
        if (state->aboutState != NULL)
            AboutDraw(state, state->aboutState);
        break;

    default:
        MenuDraw(state);
        break;
    }

    DrawErrorServiceOverlay();
}

void CloseAppState(AppState *state)
{
    if (state == NULL)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered when closing app state.");
        return;
    }

    if (state->gamestate != NULL)
    {
        GameCleanUp(state->gamestate);
        free(state->gamestate);
        state->gamestate = NULL;
    }
    if (state->loadingState)
    {
        free(state->loadingState);
        state->loadingState = NULL;
    }
    if (state->aboutState)
    {
        free(state->aboutState);
        state->aboutState = NULL;
    }
    if (state->settingsState)
    {
        FreeSettingsState(state->settingsState);
        state->settingsState = NULL;
    }
    if (state->pauseState)
    {
        free(state->pauseState);
        state->pauseState = NULL;
    }
    if (state->gameOverState)
    {
        free(state->gameOverState);
        state->gameOverState = NULL;
    }
    if (state->menuLoadState)
    {
        free(state->menuLoadState);
        state->menuLoadState = NULL;
    }

    MenuUnloadTextures();
}