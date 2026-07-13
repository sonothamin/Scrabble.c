#include "menu.h"
#include "ui.h"
#include "raylib.h"
#include "raygui.h"

static bool sfxCheckboxChecked = true;  
static bool bgmCheckboxChecked = true;  

static const char* GetMenuScrabbleScore(char c) 
{
    switch (c) {
        case 'D': case 'G': return "2";
        case 'B': case 'C': case 'M': case 'P': return "3";
        case 'F': case 'H': case 'V': case 'W': case 'Y': return "4";
        case 'K': return "5";
        case 'J': case 'X': return "8";
        case 'Q': case 'Z': return "10";
        default: return "1"; 
    }
}

void MenuUpdate(AppState* state)
{
    if (!state) return;

    // Combined hotkeys for quick-starting or loading a match
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_N) || IsKeyPressed(KEY_L))
    {
        StartNewGame(state);
    }
    else if (IsKeyPressed(KEY_S))
    {
        state->currentScreen = APP_SCREEN_SETTINGS;
    }
    else if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
    {
        state->shouldClose = true; // Safe main loop exit trigger
    }
    else if (IsKeyPressed(KEY_F11))
    {
        ToggleFullscreen();
    }
    else if (IsKeyPressed(KEY_M))
    {
        // Smart toggle: if either is on, mute both. If both are off, restore both.
        bool toggled = !(sfxCheckboxChecked || bgmCheckboxChecked);
        sfxCheckboxChecked = toggled;
        bgmCheckboxChecked = toggled;
    }
}

void MenuDraw(AppState* state)
{
    if (!state) return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){ 24, 32, 38, 255 });

    int baseFontSize = screenHeight / 38; 
    if (baseFontSize < 16) baseFontSize = 16;
    ApplyScrabbleTheme(baseFontSize);

    const float padding = screenWidth / 25.0f;
    const float panelGap = screenWidth / 35.0f;

    const char* titleText = "SCRABBLE.C";
    const int titleLength = 10;
    
    const float menuTileSize = baseFontSize * 3.0f;
    const float menuTileSpacing = menuTileSize * 0.10f;
    const int tileFontSize = (int)(menuTileSize * 0.65f); 
    const int scoreFontSize = (int)(menuTileSize * 0.22f);

    const Color shadowColor = { 10, 14, 18, 160 };
    const Color tileColor = { 244, 228, 198, 255 };
    const Color innerLineColor = { 255, 248, 230, 255 };
    const Color outerLineColor = { 194, 169, 126, 255 };
    const Color textColor = { 38, 28, 16, 255 };
    const Color scoreColor = { 120, 95, 68, 255 };
    
    // Render Decorative Header Tiles
    for (int i = 0; i < titleLength; i++)
    {
        float currentX = padding + i * (menuTileSize + menuTileSpacing);
        Rectangle tileRect = { currentX, padding, menuTileSize, menuTileSize };

        DrawRectangleRounded((Rectangle){ tileRect.x + 5.0f, tileRect.y + 6.0f, tileRect.width, tileRect.height }, 0.18f, 4, shadowColor);
        DrawRectangleRounded(tileRect, 0.18f, 4, tileColor);
        DrawRectangleRoundedLines((Rectangle){ tileRect.x + 4.0f, tileRect.y + 4.0f, tileRect.width - 8.0f, tileRect.height - 8.0f }, 0.15f, 4, innerLineColor);
        DrawRectangleRoundedLines(tileRect, 0.18f, 4, outerLineColor);

        char letterStr[2] = { titleText[i], '\0' };
        DrawText(
            letterStr, 
            tileRect.x + (menuTileSize - MeasureText(letterStr, tileFontSize)) / 2.0f, 
            tileRect.y + (menuTileSize - tileFontSize) / 2.0f - 4.0f, 
            tileFontSize, 
            textColor
        );

        if (titleText[i] != '.')
        {
            DrawText(
                GetMenuScrabbleScore(titleText[i]), 
                tileRect.x + tileRect.width - scoreFontSize - 6.0f, 
                tileRect.y + tileRect.height - scoreFontSize - 6.0f, 
                scoreFontSize, 
                scoreColor
            );
        }
    }
    
    float subtitleY = padding + menuTileSize + 16.0f;
    float subtitleHeight = baseFontSize * 1.2f;
    GuiLabel((Rectangle){ padding, subtitleY, screenWidth - (2.0f * padding), subtitleHeight }, "Be aware adventurer! Here every letter counts!");

    // Layout Calculations
    const float contentTop = subtitleY + subtitleHeight + 45.0f;
    const float optionPanelWidth = (screenWidth / 4 < 260) ? 260.0f : (float)(screenWidth / 4);
    const float mainPanelWidth = screenWidth - (2.0f * padding) - panelGap - optionPanelWidth;
    const float mainPanelHeight = screenHeight - contentTop - padding;

    float rowHeight = mainPanelHeight / 6.0f;
    float btnWidth = (mainPanelWidth * 0.38f < 180.0f) ? 180.0f : mainPanelWidth * 0.38f;
    float targetBtnHeight = rowHeight * 0.65f;
    float labelX = padding + btnWidth + 40.0f;
    float labelW = mainPanelWidth - btnWidth - 60.0f; 

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 0); 

    // --- MAIN ACTION PANEL ---
    GuiGroupBox((Rectangle){ padding, contentTop, mainPanelWidth, mainPanelHeight }, "START A GAME");

    if (GuiButton((Rectangle){ padding + 25.0f, contentTop + rowHeight * 0.8f, btnWidth, targetBtnHeight }, "New Local Game")) {
        StartNewGame(state);
    }
    GuiLabel((Rectangle){ labelX, contentTop + rowHeight * 0.8f, labelW, targetBtnHeight }, "Play on this device turn-by-turn");

    if (GuiButton((Rectangle){ padding + 25.0f, contentTop + (rowHeight * 1.8f), btnWidth, targetBtnHeight }, "New Network Game")) {
        StartNewGame(state);
    }
    GuiLabel((Rectangle){ labelX, contentTop + (rowHeight * 1.8f), labelW, targetBtnHeight }, "Play with friends within the LAN");

    if (GuiButton((Rectangle){ padding + 25.0f, contentTop + (rowHeight * 2.8f), btnWidth, targetBtnHeight }, "Load Saved Game")) {
        StartNewGame(state);
    }
    GuiLabel((Rectangle){ labelX, contentTop + (rowHeight * 2.8f), labelW, targetBtnHeight }, "Load a previous saved game file");

    // Bottom Quick HUD Info
    GuiLine((Rectangle){ padding + 25.0f, contentTop + (rowHeight * 4.6f), mainPanelWidth - 50.0f, 8.0f }, NULL);
    GuiLabel((Rectangle){ padding + 25.0f, contentTop + (rowHeight * 4.9f), mainPanelWidth - 50.0f, rowHeight * 0.5f }, "Q: Quit   M: Mute Everything   L: Quick Load   N: Quick Local   F11: Fullscreen");

    // --- SIDE PANEL: CONFIGURATION ---
    float optionPanelX = screenWidth - padding - optionPanelWidth;
    GuiGroupBox((Rectangle){ optionPanelX, contentTop, optionPanelWidth, mainPanelHeight }, "SETTINGS & OPTIONS");

    float soundGroupMarginX = 30.0f; // Tighter padding for cleaner text formatting
    float soundGroupMarginY = 50.0f;
    float soundBoxWidth = optionPanelWidth - (soundGroupMarginX * 2.0f);
    float soundBoxHeight = mainPanelHeight * 0.38f;
    
    GuiGroupBox((Rectangle){ optionPanelX + soundGroupMarginX, contentTop + soundGroupMarginY, soundBoxWidth, soundBoxHeight }, "Audio Mixer");
    
    float checkboxHeight = (baseFontSize < 16) ? 16.0f : ((baseFontSize > 22) ? 22.0f : (float)baseFontSize);
    float sfxCheckboxY = contentTop + soundGroupMarginY + (soundBoxHeight * 0.28f);
    float bgmCheckboxY = contentTop + soundGroupMarginY + (soundBoxHeight * 0.62f);

    GuiCheckBox((Rectangle){ optionPanelX + soundGroupMarginX + 15.0f, sfxCheckboxY, checkboxHeight, checkboxHeight }, "Sound Effects (SFX)", &sfxCheckboxChecked);
    GuiCheckBox((Rectangle){ optionPanelX + soundGroupMarginX + 15.0f, bgmCheckboxY, checkboxHeight, checkboxHeight }, "Background Music (BGM)", &bgmCheckboxChecked);

    // Navigation Footers
    float navBtnWidth = (optionPanelWidth - 56.0f) / 2.0f;
    float navBtnY = contentTop + mainPanelHeight - targetBtnHeight - 20.0f;
    
    if (GuiButton((Rectangle){ optionPanelX + 20.0f, navBtnY, navBtnWidth, targetBtnHeight }, "About")) {
        state->currentScreen = APP_SCREEN_ABOUT;
    }
    if (GuiButton((Rectangle){ optionPanelX + 36.0f + navBtnWidth, navBtnY, navBtnWidth, targetBtnHeight }, "Settings")) {
        state->currentScreen = APP_SCREEN_SETTINGS;
    }
}