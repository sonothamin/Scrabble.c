#include "settings.h"
#include "sound.h"
#include "ui.h"
#include "raygui.h"
#include <stdlib.h>
#include <stdio.h>

SettingsState* SettingsInit(void)
{
    SettingsState* settings = (SettingsState*)malloc(sizeof(SettingsState));
    if (settings == NULL)
    {
        return NULL;
    }

    settings->musicVolume = 0.8f;
    settings->sfxVolume = 1.0f;
    settings->muteAll = false;
    settings->selectedOption = 0;

    SetMusicVolumeLevel(settings->musicVolume);
    SetSfxVolumeLevel(settings->sfxVolume);

    return settings;
}

void SettingsFree(SettingsState* settings)
{
    if (settings != NULL)
    {
        free(settings);
    }
}

void SettingsUpdate(AppState* state)
{
    if (state == NULL || state->settingsState == NULL)
    {
        return;
    }

    if (IsKeyPressed(KEY_B) || IsKeyPressed(KEY_ESCAPE))
    {
        PlaySoundEffect(SFX_BACK_NAV);
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void SettingsDraw(const AppState* state)
{
    if (state == NULL || state->settingsState == NULL)
    {
        return;
    }

    SettingsState* settings = state->settingsState;

    const int boxX = 100;
    const int boxY = 80;
    const int boxWidth = 600;
    const int boxHeight = 440;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(LIGHTGRAY, 0.15f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, GRAY);

    const char* title = "SETTINGS";
    const int titleWidth = MeasureText(title, 32);
    DrawText(title, boxX + (boxWidth - titleWidth) / 2, boxY + 30, 32, DARKGRAY);

    const int startX = boxX + 180;
    int currentY = boxY + 110;
    const int controlWidth = 240;
    const int controlHeight = 30;
    const int spacing = 50;

    DrawText("Music Volume", boxX + 40, currentY + 5, 20, DARKGRAY);
    
    char musicText[16];
    snprintf(musicText, sizeof(musicText), "%d%%", (int)(settings->musicVolume * 100.0f));
    
    float oldMusicVol = settings->musicVolume;
    GuiSliderBar((Rectangle){ (float)startX, (float)currentY, (float)controlWidth, (float)controlHeight }, 
                 NULL, musicText, &settings->musicVolume, 0.0f, 1.0f);

    if (oldMusicVol != settings->musicVolume)
    {
        SetMusicVolumeLevel(settings->musicVolume);
    }

    currentY += spacing;
    DrawText("SFX Volume", boxX + 40, currentY + 5, 20, DARKGRAY);

    char sfxText[16];
    snprintf(sfxText, sizeof(sfxText), "%d%%", (int)(settings->sfxVolume * 100.0f));

    float oldSfxVol = settings->sfxVolume;
    GuiSliderBar((Rectangle){ (float)startX, (float)currentY, (float)controlWidth, (float)controlHeight }, 
                 NULL, sfxText, &settings->sfxVolume, 0.0f, 1.0f);

    if (oldSfxVol != settings->sfxVolume)
    {
        SetSfxVolumeLevel(settings->sfxVolume);
    }

    currentY += spacing;
    DrawText("Audio Output", boxX + 40, currentY + 5, 20, DARKGRAY);

    const char* muteBtnText = settings->muteAll ? "Muted (Click to Unmute)" : "Audio Active (Click to Mute)";
    if (GuiButton((Rectangle){ (float)startX, (float)currentY, (float)controlWidth, (float)controlHeight }, muteBtnText))
    {
        settings->muteAll = !settings->muteAll;
        PlaySoundEffect(SFX_BUTTON);

        if (settings->muteAll)
        {
            SetMusicVolumeLevel(0.0f);
            SetSfxVolumeLevel(0.0f);
        }
        else
        {
            SetMusicVolumeLevel(settings->musicVolume);
            SetSfxVolumeLevel(settings->sfxVolume);
        }
    }

    currentY += spacing + 20;
    const int backWidth = 160;
    const int backX = boxX + (boxWidth - backWidth) / 2;

    if (GuiButton((Rectangle){ (float)backX, (float)currentY, (float)backWidth, (float)controlHeight }, "Back to Menu (B)"))
    {
        PlaySoundEffect(SFX_BACK_NAV);
        ((AppState*)state)->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}