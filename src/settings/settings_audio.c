#include <stdio.h>

#include "raylib.h"
#include "raygui.h"
#include "sound.h"
#include "settings/settings_audio.h"

void DrawSettingsAudioTab(ExtendedSettingsState *extSettings, float contentX, float contentY, float contentWidth, int baseFontSize)
{
    SettingsState *settings = &extSettings->base;

    const float labelWidth = baseFontSize * 7.5f;
    const float muteBtnWidth = 110.0f;
    const float gap = 15.0f;
    const float textPadding = 45.0f;

    float maxAvailableWidth = contentWidth - 70.0f;
    float sliderWidth = maxAvailableWidth - labelWidth - muteBtnWidth - (gap * 2.0f) - textPadding - 30.0f;

    if (sliderWidth < 100.0f)
    {
        sliderWidth = 100.0f;
    }

    const float controlHeight = baseFontSize * 1.4f;
    const float rowSpacing = baseFontSize * 2.2f;

    float currentY = contentY + 10.0f;
    float actionBtnX = contentX + labelWidth + sliderWidth + 30.0f + textPadding + gap;

    // --- Row 1: BGM ---
    GuiLabel((Rectangle){contentX, currentY + 2.0f, labelWidth, controlHeight}, "BGM");

    char musicText[16];
    snprintf(musicText, sizeof(musicText), "%d%%", (int)(settings->bgmVolume * 100.0f));

    float oldMusicVol = settings->bgmVolume;
    GuiSliderBar((Rectangle){contentX + labelWidth + gap, currentY, sliderWidth, controlHeight},
                 NULL, musicText, &settings->bgmVolume, 0.0f, 1.0f);

    if (oldMusicVol != settings->bgmVolume && settings->bgmEnable)
    {
        SetMusicVolumeLevel(settings->bgmVolume);
    }

    const char *bgmBtnText = settings->bgmEnable ? "Mute" : "Unmute";
    if (GuiButton((Rectangle){actionBtnX, currentY, muteBtnWidth, controlHeight}, bgmBtnText))
    {
        settings->bgmEnable = !settings->bgmEnable;
        PlaySoundEffect(SFX_BUTTON);
        SetMusicVolumeLevel(settings->bgmEnable ? settings->bgmVolume : 0.0f);
    }

    // --- Row 2: SFX ---
    currentY += rowSpacing;
    GuiLabel((Rectangle){contentX, currentY + 2.0f, labelWidth, controlHeight}, "Sound Effects");

    char sfxText[16];
    snprintf(sfxText, sizeof(sfxText), "%d%%", (int)(settings->sfxVolume * 100.0f));

    float oldSfxVol = settings->sfxVolume;
    GuiSliderBar((Rectangle){contentX + labelWidth + gap, currentY, sliderWidth, controlHeight},
                 NULL, sfxText, &settings->sfxVolume, 0.0f, 1.0f);

    if (oldSfxVol != settings->sfxVolume && settings->sfxEnable)
    {
        SetSfxVolumeLevel(settings->sfxVolume);
    }

    const char *sfxBtnText = settings->sfxEnable ? "Mute" : "Unmute";
    if (GuiButton((Rectangle){actionBtnX, currentY, muteBtnWidth, controlHeight}, sfxBtnText))
    {
        settings->sfxEnable = !settings->sfxEnable;
        PlaySoundEffect(SFX_BUTTON);
        SetSfxVolumeLevel(settings->sfxEnable ? settings->sfxVolume : 0.0f);
    }
}
