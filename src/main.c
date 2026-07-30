#include "raylib.h"
#include "resource_dir.h"
#include "app_state.h"
#include "settings.h"
#include "error_service.h"
#include "ui.h"
#include "sound.h"

#if defined(_WIN32) //Windows Dark Mode Hack
    __declspec(dllimport) void* __stdcall LoadLibraryA(const char* lpLibFileName);
    __declspec(dllimport) void* __stdcall GetProcAddress(void* hModule, const char* lpProcName);
    __declspec(dllimport) int   __stdcall FreeLibrary(void* hModule);

    typedef long (__stdcall *pfnDwmSetWindowAttribute)(void* hwnd, unsigned long dwAttribute, const void* pvAttribute, unsigned long cbAttribute);

    static void EnableImmersiveDarkMode(void* hwnd) 
    {
        void* dwmModule = LoadLibraryA("dwmapi.dll");
        if (dwmModule) 
        {
            pfnDwmSetWindowAttribute setWindowAttribute = 
                (pfnDwmSetWindowAttribute)GetProcAddress(dwmModule, "DwmSetWindowAttribute");
            
            if (setWindowAttribute) 
            {
                int darkMode = 1;
                // DWMWA_USE_IMMERSIVE_DARK_MODE = 20
                setWindowAttribute(hwnd, 20, &darkMode, sizeof(darkMode));
            }
            FreeLibrary(dwmModule);
        }
    }
#endif

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    InitWindow(1024, 768, "Scrabble.c");

#if defined(_WIN32)
    void* hwnd = GetWindowHandle();
    if (hwnd) 
    {
        EnableImmersiveDarkMode(hwnd);
    }
#endif

    SetExitKey(KEY_F4);
    MaximizeWindow();
    SetWindowMinSize(1380, 820);

    SearchAndSetResourceDir("resources");
    InitAppFont();

    AppState appState = {0};

    InitAppState(&appState);
    SoundSysInit();

    if (appState.settingsState != NULL)
    {
        ApplySoundSettings(
            appState.settingsState->bgmVolume,
            appState.settingsState->bgmEnable,
            appState.settingsState->sfxVolume,
            appState.settingsState->sfxEnable
        );
    }

    appState.shouldClose = false;

    if (appState.settingsState != NULL && appState.settingsState->showLoadingScreen)
    {
        appState.currentScreen = APP_SCREEN_LOADING;
    }
    else
    {
        appState.currentScreen = APP_SCREEN_MAIN_MENU;
    }

    ClearGlobalError();

    while (!WindowShouldClose() && !appState.shouldClose)
    {
        SoundSysUpdate();
        
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        if (!HasGlobalError())
        {
            UpdateAppState(&appState);
        }

        BeginDrawing();
        DrawAppState(&appState);
        if (HasGlobalError() && ShowErrorDialog())
        {
            appState.shouldClose = true;
        }
        EndDrawing();
    }
    
    SoundSysShutdown();
    CloseAppState(&appState);
    UnloadAppFont();
    CloseWindow();

    return 0;
}