#include "pause.h"
#include "ui.h"
#include "raylib.h"
#include "raygui.h"
#include "sound.h"
#include "settings.h"
#include "settings/settings_internal.h"
#include "error_service.h"
#include <math.h>

#include "app_state.h"

static void ExitPauseMenu(AppState *state, PauseState *pause)
{
    pause->isPaused = false;
    if (state && state->settingsState)
    {
        SaveSettingsToFile(state->settingsState, CONFIG_FILE_PATH);
    }
}

void InitPauseState(PauseState *pause)
{
    if (!pause) return;
    pause->isPaused = false;
}

void UpdatePauseOverlay(AppState *state, PauseState *pause)
{
    if (!state || !pause) return;

    if (!pause->isPaused)
    {
        if (IsKeyPressed(KEY_P))
        {
            pause->isPaused = true;
            PlaySoundEffect(SFX_BUTTON);
        }
        return;
    }

    // Active pause menu hotkeys
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_R))
    {
        ExitPauseMenu(state, pause);
        PlaySoundEffect(SFX_BUTTON);
        return;
    }

    if (IsKeyPressed(KEY_T))
    {
        ExitPauseMenu(state, pause);
        StartNewGame(state);
        PlaySoundEffect(SFX_GAME_START);
        return;
    }

    if (IsKeyPressed(KEY_Q))
    {
        ExitPauseMenu(state, pause);
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        PlaySoundEffect(SFX_BACK_NAV);
        return;
    }
}

void DrawPauseOverlay(AppState *state, PauseState *pause)
{
    if (!state || !pause || !pause->isPaused) return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Soft dim overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 10, 15, 22, 210 });

    int baseFontSize = fmaxf(16, screenHeight / 38);
    ApplyScrabbleTheme(baseFontSize);

    // Clean, centered modal card
    float cardW = fminf(screenWidth * 0.55f, 560.0f);
    if (cardW < 420.0f) cardW = 420.0f;

    float cardH = 540.0f;
    float cardX = (screenWidth - cardW) / 2.0f;
    float cardY = (screenHeight - cardH) / 2.0f;

    Rectangle cardRect = { cardX, cardY, cardW, cardH };

    // Card background shadow & border
    DrawRectangleRounded((Rectangle){ cardX + 4, cardY + 6, cardW, cardH }, 0.08f, 6, (Color){ 0, 0, 0, 140 });
    DrawRectangleRounded(cardRect, 0.08f, 6, (Color){ 20, 28, 36, 255 });
    DrawRectangleRoundedLinesEx(cardRect, 0.08f, 6, 2.0f, (Color){ 54, 72, 90, 255 });

    // Clean Header
    float titleY = cardY + 25.0f;
    const char *titleText = "PAUSED";
    int titleFontSize = (int)(baseFontSize * 2.0f);
    int titleW = MeasureAppText(titleText, titleFontSize);
    DrawAppText(titleText, cardX + (cardW - titleW) / 2.0f, titleY, titleFontSize, (Color){ 244, 228, 198, 255 });

    // Thin accent line below title
    DrawLineV((Vector2){ cardX + 40.0f, titleY + titleFontSize + 12.0f },
              (Vector2){ cardX + cardW - 40.0f, titleY + titleFontSize + 12.0f },
              (Color){ 46, 202, 113, 255 });

    // Audio & Sound Quick Options Box
    float soundBoxY = titleY + titleFontSize + 25.0f;
    float soundBoxW = cardW - 60.0f;
    float soundBoxH = 175.0f;
    Rectangle soundBoxRect = { cardX + 30.0f, soundBoxY, soundBoxW, soundBoxH };

    DrawRectangleRounded(soundBoxRect, 0.06f, 4, (Color){ 14, 20, 27, 255 });
    DrawRectangleRoundedLines(soundBoxRect, 0.06f, 4, (Color){ 36, 48, 62, 255 });

    if (state->settingsState)
    {
        SettingsState *settings = state->settingsState;

        float ctrlY = soundBoxY + 16.0f;
        bool sfx = settings->sfxEnable;
        bool bgm = settings->bgmEnable;

        if (GuiCheckBox((Rectangle){ soundBoxRect.x + 20.0f, ctrlY, 22.0f, 22.0f }, "Sound Effects (SFX)", &settings->sfxEnable))
        {
            if (sfx != settings->sfxEnable)
            {
                SetSfxVolumeLevel(settings->sfxEnable ? settings->sfxVolume : 0.0f);
                PlaySoundEffect(SFX_BUTTON);
            }
        }

        ctrlY += 32.0f;
        float labelFontSize = baseFontSize * 0.75f;
        DrawAppText("Volume:", soundBoxRect.x + 35.0f, ctrlY + (20.0f - labelFontSize) / 2.0f, labelFontSize, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        float prevSfxVol = settings->sfxVolume;
        GuiSlider((Rectangle){ soundBoxRect.x + 165.0f, ctrlY, soundBoxW - 200.0f, 20.0f }, "", "", &settings->sfxVolume, 0.0f, 1.0f);
        if (fabsf(prevSfxVol - settings->sfxVolume) > 0.01f && settings->sfxEnable)
        {
            SetSfxVolumeLevel(settings->sfxVolume);
        }

        ctrlY += 40.0f;
        if (GuiCheckBox((Rectangle){ soundBoxRect.x + 20.0f, ctrlY, 22.0f, 22.0f }, "Background Music (BGM)", &settings->bgmEnable))
        {
            if (bgm != settings->bgmEnable)
            {
                SetMusicVolumeLevel(settings->bgmEnable ? settings->bgmVolume : 0.0f);
                PlaySoundEffect(SFX_BUTTON);
            }
        }

        ctrlY += 32.0f;
        DrawAppText("Volume:", soundBoxRect.x + 35.0f, ctrlY + (20.0f - labelFontSize) / 2.0f, labelFontSize, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        float prevBgmVol = settings->bgmVolume;
        GuiSlider((Rectangle){ soundBoxRect.x + 165.0f, ctrlY, soundBoxW - 200.0f, 20.0f }, "", "", &settings->bgmVolume, 0.0f, 1.0f);
        if (fabsf(prevBgmVol - settings->bgmVolume) > 0.01f && settings->bgmEnable)
        {
            SetMusicVolumeLevel(settings->bgmVolume);
        }
    }

    // Main Menu Navigation Action Buttons
    float btnY = soundBoxY + soundBoxH + 18.0f;
    float btnW = cardW - 60.0f;
    float btnH = 38.0f;
    float btnGap = 8.0f;

    if (GuiButton((Rectangle){ cardX + 30.0f, btnY, btnW, btnH }, "Resume Game"))
    {
        ExitPauseMenu(state, pause);
        PlaySoundEffect(SFX_BUTTON);
    }

    if (GuiButton((Rectangle){ cardX + 30.0f, btnY + (btnH + btnGap), btnW, btnH }, "Restart Match"))
    {
        ExitPauseMenu(state, pause);
        StartNewGame(state);
        PlaySoundEffect(SFX_GAME_START);
    }

    if (GuiButton((Rectangle){ cardX + 30.0f, btnY + 2.0f * (btnH + btnGap), btnW, btnH }, "Exit to Main Menu"))
    {
        ExitPauseMenu(state, pause);
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        PlaySoundEffect(SFX_BACK_NAV);
    }

    // Hotkey bar at bottom of pause menu modal card
    float hkBarH = (float)(int)(baseFontSize * 0.72f) * 2.0f + 6.0f;
    float hkBarY = cardY + cardH - 12.0f - hkBarH;
    float hkBarX = cardX + 30.0f;
    float hkBarW = cardW - 60.0f;

    static const HotkeyEntry pauseKeys[] = {
        { "ESC/R", "Resume"  },
        { "T",     "Restart" },
        { "Q",     "Quit"    },
    };
    DrawHotkeyBar(pauseKeys, 3, hkBarX, hkBarY, hkBarW, hkBarH, (float)baseFontSize);
}
