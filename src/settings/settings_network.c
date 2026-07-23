#include "raylib.h"
#include "raygui.h"
#include "ui.h"
#include "settings/settings_network.h"

void DrawSettingsNetworkTab(ExtendedSettingsState *extSettings, float contentX, float contentY, float contentWidth, int baseFontSize)
{
    (void)extSettings;
    (void)contentWidth;
    DrawAppText("LAN Settings Coming Soon!!", contentX, contentY, baseFontSize * 1.1f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
}
