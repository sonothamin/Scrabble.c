#include "menu.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static bool newLocalGameButtonPressed = false;
static bool newNetworkGameButtonPressed = false;
static bool loadGameFromFileButtonPressed = false;
static bool sfxCheckboxChecked = false;
static bool bgmCheckboxChecked = false;
static bool aboutNavButtonPressed = false;
static bool settingsNavButtonPressed = false;

void MenuUpdate(AppState* state)
{
    if (state == NULL)
        return;

    // Key binds (F11 check removed here to be handled globally in the main engine loop)
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        state->currentScreen = APP_SCREEN_GAME;
    else if (IsKeyPressed(KEY_S))
        state->currentScreen = APP_SCREEN_SETTINGS;
    else if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
        CloseWindow();
        
    if (IsKeyPressed(KEY_N))
        state->currentScreen = APP_SCREEN_GAME;
    if (IsKeyPressed(KEY_L))
        state->currentScreen = APP_SCREEN_GAME;
    if (IsKeyPressed(KEY_M))
    {
        if (sfxCheckboxChecked || bgmCheckboxChecked)
        {
            sfxCheckboxChecked = false;
            bgmCheckboxChecked = false;
        }
        else
        {
            sfxCheckboxChecked = true;
            bgmCheckboxChecked = true;
        }
    }
}

void MenuDraw(AppState* state)
{
    if (state == NULL)
        return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    int baseFontSize = screenHeight / 40; 
    if (baseFontSize < 16) baseFontSize = 16;
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize);

    // Dynamic layout measurements
    const int padding = screenWidth / 25;
    const int panelGap = screenWidth / 30;
    
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize * 2.5);
    float titleHeight = baseFontSize * 3.0f;
    GuiLabel((Rectangle){ (float)padding, (float)padding, (float)screenWidth - (2 * padding), titleHeight }, "Scrabble.c");
    
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize);
    float subtitleY = (float)padding + titleHeight - (baseFontSize * 0.4f); 
    float subtitleHeight = (float)baseFontSize * 1.5f;
    GuiLabel((Rectangle){ (float)padding, subtitleY, (float)screenWidth - (2 * padding), subtitleHeight }, "Be aware adventurer! Here every letter counts!");

    float bottomPadding = baseFontSize * 1.5f; 
    const int contentTop = (int)(subtitleY + subtitleHeight + bottomPadding);
    
    const int optionPanelWidth = screenWidth / 4 < 240 ? 240 : screenWidth / 4;
    const int mainPanelWidth = screenWidth - (2 * padding) - panelGap - optionPanelWidth;
    const int mainPanelHeight = screenHeight - contentTop - padding;
    const int optionPanelHeight = mainPanelHeight;

    const float mainPanelX = (float)padding;
    const float mainPanelY = (float)contentTop;
    const float mainPanelW = (float)mainPanelWidth;
    const float mainPanelH = (float)mainPanelHeight;

    const float optionPanelX = (float)(screenWidth - padding - optionPanelWidth);
    const float optionPanelY = (float)contentTop;
    const float optionPanelW = (float)optionPanelWidth;
    const float optionPanelH = (float)optionPanelHeight;

    // ("Start a Game") ---
    GuiGroupBox((Rectangle){ mainPanelX, mainPanelY, mainPanelW, mainPanelH }, "Start a Game");
    
    float rowHeight = mainPanelH / 6.0f;
    float btnWidth = mainPanelW * 0.35f;
    if (btnWidth < 160) btnWidth = 160;
    
    float labelX = mainPanelX + btnWidth + 30;
    float labelW = mainPanelW - btnWidth - 50; 

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 0); 

    newLocalGameButtonPressed = GuiButton((Rectangle){ mainPanelX + 20, mainPanelY + rowHeight, btnWidth, rowHeight * 0.6f }, "New Local Game");
    GuiLabel((Rectangle){ labelX, mainPanelY + rowHeight, labelW, rowHeight * 0.6f }, "Play on this device turn-by-turn");

    newNetworkGameButtonPressed = GuiButton((Rectangle){ mainPanelX + 20, mainPanelY + (rowHeight * 2), btnWidth, rowHeight * 0.6f }, "New Network Game");
    GuiLabel((Rectangle){ labelX, mainPanelY + (rowHeight * 2), labelW, rowHeight * 0.6f }, "Play with friends within the LAN");

    loadGameFromFileButtonPressed = GuiButton((Rectangle){ mainPanelX + 20, mainPanelY + (rowHeight * 3), btnWidth, rowHeight * 0.6f }, "Load Saved Game");
    GuiLabel((Rectangle){ labelX, mainPanelY + (rowHeight * 3), labelW, rowHeight * 0.6f }, "Load a previous saved game file");

    // Hotkey Info Footer (Keeping the visual instruction)
    GuiLine((Rectangle){ mainPanelX + 20, mainPanelY + (rowHeight * 4.5f), mainPanelW - 40, 12 }, NULL);
    GuiLabel((Rectangle){ mainPanelX + 20, mainPanelY + (rowHeight * 4.8f), mainPanelW - 40, rowHeight * 0.6f }, "Q: Quit  M: Mute  L: Load  N: New  F11: Fullscreen");

    // Options Panel ---
    GuiGroupBox((Rectangle){ optionPanelX, optionPanelY, optionPanelW, optionPanelH }, "Options");

    float soundGroupPaddingX = 16.0f;
    float soundGroupPaddingY = 32.0f;
    float soundInnerPaddingX = 20.0f;
    
    float soundBoxHeight = optionPanelH * 0.38f;
    GuiGroupBox((Rectangle){ optionPanelX + soundGroupPaddingX, optionPanelY + soundGroupPaddingY, optionPanelW - (soundGroupPaddingX * 2), soundBoxHeight }, "Sound");
    
    float checkboxHeight = baseFontSize * 1.1f;
    if (checkboxHeight > 22.0f) checkboxHeight = 22.0f;
    if (checkboxHeight < 16.0f) checkboxHeight = 16.0f;

    float sfxCheckboxY = optionPanelY + soundGroupPaddingY + (soundBoxHeight * 0.25f);
    float bgmCheckboxY = optionPanelY + soundGroupPaddingY + (soundBoxHeight * 0.60f);

    GuiCheckBox((Rectangle){ optionPanelX + soundGroupPaddingX + soundInnerPaddingX, sfxCheckboxY, checkboxHeight, checkboxHeight }, "Sound Effects", &sfxCheckboxChecked);
    GuiCheckBox((Rectangle){ optionPanelX + soundGroupPaddingX + soundInnerPaddingX, bgmCheckboxY, checkboxHeight, checkboxHeight }, "Background", &bgmCheckboxChecked);

    float navBtnWidth = (optionPanelW - 48) / 2.0f;
    float navBtnHeight = rowHeight * 0.6f;
    if (navBtnHeight > 40) navBtnHeight = 40;
    
    aboutNavButtonPressed = GuiButton((Rectangle){ optionPanelX + 16, optionPanelY + optionPanelH - navBtnHeight - 16, navBtnWidth, navBtnHeight }, "About");
    settingsNavButtonPressed = GuiButton((Rectangle){ optionPanelX + 16 + navBtnWidth + 16, optionPanelY + optionPanelH - navBtnHeight - 16, navBtnWidth, navBtnHeight }, "Settings");

    // State Management (UI Interactions) ---
    if (newLocalGameButtonPressed || newNetworkGameButtonPressed || loadGameFromFileButtonPressed)
    {
        state->currentScreen = APP_SCREEN_GAME;
    }

    if (settingsNavButtonPressed)
    {
        state->currentScreen = APP_SCREEN_SETTINGS;
    }

    if (aboutNavButtonPressed)
    {
        // Reserved for future content.
    }
}