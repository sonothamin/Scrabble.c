#include <stdio.h>

#include "raylib.h"
#include "raygui.h"
#include "sound.h"
#include "settings/settings_game.h"

void DrawSettingsGameTab(ExtendedSettingsState *extSettings, float contentX, float contentY, float contentWidth, int baseFontSize)
{
    SettingsState *settings = &extSettings->base;

    const float labelWidth = baseFontSize * 7.0f;
    const float pickerBtnWidth = baseFontSize * 2.8f;
    const float inputHeight = baseFontSize * 1.5f;
    const float rowSpacing = baseFontSize * 2.3f;
    const float inputWidth = contentWidth - labelWidth - pickerBtnWidth - 90.0f;

    float rowY = contentY + 10.0f;

    // --- Row 1: Dictionary ---
    GuiLabel((Rectangle){contentX, rowY, labelWidth, inputHeight}, "Dictionary");

    GuiTextBox((Rectangle){contentX + labelWidth, rowY, inputWidth, inputHeight}, settings->dictionaryPath, 256, false);

    if (GuiButton((Rectangle){contentX + labelWidth + inputWidth + 10.0f, rowY, pickerBtnWidth, inputHeight}, "#141# Open"))
    {
        PlaySoundEffect(SFX_BUTTON);
        extSettings->activeTarget = FILE_PICKER_DICTIONARY;
        extSettings->fileDialogState.windowActive = true;
        snprintf(extSettings->fileDialogState.filterExt, sizeof(extSettings->fileDialogState.filterExt), ".txt");
    }

    // --- Row 2: Board Layout ---
    rowY += rowSpacing;
    GuiLabel((Rectangle){contentX, rowY, labelWidth, inputHeight}, "Board Layout");

    GuiTextBox((Rectangle){contentX + labelWidth, rowY, inputWidth, inputHeight}, settings->boardLayoutPath, 256, false);

    if (GuiButton((Rectangle){contentX + labelWidth + inputWidth + 10.0f, rowY, pickerBtnWidth, inputHeight}, "#141# Open"))
    {
        PlaySoundEffect(SFX_BUTTON);
        extSettings->activeTarget = FILE_PICKER_BOARD_LAYOUT;
        extSettings->fileDialogState.windowActive = true;
        snprintf(extSettings->fileDialogState.filterExt, sizeof(extSettings->fileDialogState.filterExt), ".txt");
    }

    // --- Row 3: Tile Letter Scoring Map ---
    rowY += rowSpacing;
    GuiLabel((Rectangle){contentX, rowY, labelWidth, inputHeight}, "Tile Map");

    GuiTextBox((Rectangle){contentX + labelWidth, rowY, inputWidth, inputHeight}, settings->tileMapPath, 256, false);

    if (GuiButton((Rectangle){contentX + labelWidth + inputWidth + 10.0f, rowY, pickerBtnWidth, inputHeight}, "#141# Open"))
    {
        PlaySoundEffect(SFX_BUTTON);
        extSettings->activeTarget = FILE_PICKER_TILE_MAP;
        extSettings->fileDialogState.windowActive = true;
        snprintf(extSettings->fileDialogState.filterExt, sizeof(extSettings->fileDialogState.filterExt), ".txt");
    }

    // --- Divider Line ---
    rowY += rowSpacing + 5.0f;
    GuiLine((Rectangle){contentX, rowY, contentWidth - 60.0f, 2.0f}, NULL);

    // --- Row 4: Luxury Tiles Toggle ---
    rowY += 20.0f;
    float checkboxSize = inputHeight * 0.85f;

    bool prevLuxury = settings->luxuryTilesEnabled;
    GuiCheckBox((Rectangle){contentX, rowY, checkboxSize, checkboxSize}, "Enable Luxury Rendered Tiles (High-DPI Textures & Bevels)", &settings->luxuryTilesEnabled);

    if (prevLuxury != settings->luxuryTilesEnabled)
    {
        PlaySoundEffect(SFX_BUTTON);
    }
}
