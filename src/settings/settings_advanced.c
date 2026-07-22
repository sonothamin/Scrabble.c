#include "raylib.h"
#include "raygui.h"
#include "sound.h"
#include "settings/settings_advanced.h"

void DrawSettingsAdvancedTab(ExtendedSettingsState *extSettings, float contentX, float contentY, float contentWidth, int baseFontSize)
{
    (void)contentWidth;
    SettingsState *settings = &extSettings->base;

    const float inputHeight = baseFontSize * 1.5f;
    float rowY = contentY + 10.0f;
    float checkboxSize = inputHeight * 0.85f;

    bool prevLoadingScreen = settings->showLoadingScreen;
    GuiCheckBox((Rectangle){contentX, rowY, checkboxSize, checkboxSize}, "Enable Loading Screen Transitions", &settings->showLoadingScreen);

    if (prevLoadingScreen != settings->showLoadingScreen)
    {
        PlaySoundEffect(SFX_BUTTON);
    }
}
