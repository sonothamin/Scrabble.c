#include "settings.h"
#include "sound.h"
#include "ui.h"
#include "raygui.h"
#include "error_service.h"
#include <stdlib.h>
#include <stdio.h>

SettingsState *InitSettingsState(void)
{
    SettingsState *settings = (SettingsState *)malloc(sizeof(SettingsState));
    if (settings == NULL)
    {
        ReportCriticalError(
            "Memory Allocation Failed",
            "Failed to allocate memory for SettingsState during initialization."
        );
        return NULL;
    }

    settings->bgmVolume = 0.8f;
    settings->sfxVolume = 1.0f;
    settings->sfxEnable = true;
    settings->bgmEnable = true;
    settings->selectedOption = 0;

    SetMusicVolumeLevel(settings->bgmEnable ? settings->bgmVolume : 0.0f);
    SetSfxVolumeLevel(settings->sfxEnable ? settings->sfxVolume : 0.0f);

    return settings;
}

void FreeSettingsState(SettingsState *settings)
{
    if (settings != NULL)
    {
        free(settings);
    }
}

void SettingsUpdate(AppState *state)
{
    if (state == NULL)
    {
        ReportCriticalError(
            "Invalid App State",
            "NULL AppState pointer encountered during SettingsUpdate."
        );
        return;
    }

    if (state->settingsState == NULL)
    {
        ReportCriticalError(
            "Invalid Settings State",
            "NULL SettingsState pointer encountered during SettingsUpdate."
        );
        return;
    }

    if (IsKeyPressed(KEY_B) || IsKeyPressed(KEY_ESCAPE))
    {
        PlaySoundEffect(SFX_BACK_NAV);
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void SettingsDraw(const AppState *state)
{
    if (state == NULL)
    {
        ReportCriticalError(
            "Invalid App State",
            "NULL AppState pointer encountered during SettingsDraw."
        );
        return;
    }

    if (state->settingsState == NULL)
    {
        ReportCriticalError(
            "Invalid Settings State",
            "NULL SettingsState pointer encountered during SettingsDraw."
        );
        return;
    }

    SettingsState *settings = state->settingsState;

    const int boxX = 100;
    const int boxY = 80;
    const int boxWidth = 600;
    const int boxHeight = 440;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(LIGHTGRAY, 0.15f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, GRAY);

    const char *title = "SETTINGS";
    const int titleWidth = MeasureText(title, 32);
    DrawText(title, boxX + (boxWidth - titleWidth) / 2, boxY + 30, 32, DARKGRAY);

    const int startX = boxX + 180;
    int currentY = boxY + 100;
    const int controlWidth = 240;
    const int controlHeight = 30;
    const int spacing = 50;

    // --- Music Volume Slider ---
    DrawText("Music Volume", boxX + 40, currentY + 5, 20, DARKGRAY);

    char musicText[16];
    snprintf(musicText, sizeof(musicText), "%d%%", (int)(settings->bgmVolume * 100.0f));

    float oldMusicVol = settings->bgmVolume;
    GuiSliderBar((Rectangle){(float)startX, (float)currentY, (float)controlWidth, (float)controlHeight},
                 NULL, musicText, &settings->bgmVolume, 0.0f, 1.0f);

    if (oldMusicVol != settings->bgmVolume)
    {
        if (settings->bgmEnable)
        {
            SetMusicVolumeLevel(settings->bgmVolume);
        }
    }

    // --- Music Toggle Button ---
    currentY += spacing;
    DrawText("Music Audio", boxX + 40, currentY + 5, 20, DARKGRAY);

    const char *bgmBtnText = settings->bgmEnable ? "Music Enabled" : "Music Muted";
    if (GuiButton((Rectangle){(float)startX, (float)currentY, (float)controlWidth, (float)controlHeight}, bgmBtnText))
    {
        settings->bgmEnable = !settings->bgmEnable;
        PlaySoundEffect(SFX_BUTTON);
        SetMusicVolumeLevel(settings->bgmEnable ? settings->bgmVolume : 0.0f);
    }

    // --- SFX Volume Slider ---
    currentY += spacing;
    DrawText("SFX Volume", boxX + 40, currentY + 5, 20, DARKGRAY);

    char sfxText[16];
    snprintf(sfxText, sizeof(sfxText), "%d%%", (int)(settings->sfxVolume * 100.0f));

    float oldSfxVol = settings->sfxVolume;
    GuiSliderBar((Rectangle){(float)startX, (float)currentY, (float)controlWidth, (float)controlHeight},
                 NULL, sfxText, &settings->sfxVolume, 0.0f, 1.0f);

    if (oldSfxVol != settings->sfxVolume)
    {
        if (settings->sfxEnable)
        {
            SetSfxVolumeLevel(settings->sfxVolume);
        }
    }

    // --- SFX Toggle Button ---
    currentY += spacing;
    DrawText("SFX Audio", boxX + 40, currentY + 5, 20, DARKGRAY);

    const char *sfxBtnText = settings->sfxEnable ? "SFX Enabled" : "SFX Muted";
    if (GuiButton((Rectangle){(float)startX, (float)currentY, (float)controlWidth, (float)controlHeight}, sfxBtnText))
    {
        settings->sfxEnable = !settings->sfxEnable;
        PlaySoundEffect(SFX_BUTTON);
        SetSfxVolumeLevel(settings->sfxEnable ? settings->sfxVolume : 0.0f);
    }

    // --- Back Button ---
    currentY += spacing + 15;
    const int backWidth = 160;
    const int backX = boxX + (boxWidth - backWidth) / 2;

    if (GuiButton((Rectangle){(float)backX, (float)currentY, (float)backWidth, (float)controlHeight}, "Back to Menu (B)"))
    {
        PlaySoundEffect(SFX_BACK_NAV);
        ((AppState *)state)->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}