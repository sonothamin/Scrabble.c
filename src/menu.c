#include "menu.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static bool newLocalGameButtonPressed = false;
static bool newNetworkGameButtonPressed = false;
static bool loadGameFromFileButtonPressed = false;
static bool sfxCheckboxChecked = true;  
static bool bgmCheckboxChecked = true;  
static bool aboutNavButtonPressed = false;
static bool settingsNavButtonPressed = false;

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

static void ApplyScrabbleTheme(int baseFontSize)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x182026FF); 
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x2ECA7140); 
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x697D8CFF);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x242C34FF);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x364452FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x2ECA7115);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x2ECA71AA);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0x2ECA71FF);
    
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x2ECA7130);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0x2ECA71FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0x8FF2B8FF);

    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x242C34FF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x364452FF);
    GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    GuiSetStyle(CHECKBOX, BASE_COLOR_PRESSED, 0x2ECA71FF); 
    GuiSetStyle(CHECKBOX, BORDER_COLOR_PRESSED, 0x8FF2B8FF);
}

void MenuUpdate(AppState* state)
{
    if (state == NULL) return;

    if (newLocalGameButtonPressed || newNetworkGameButtonPressed || loadGameFromFileButtonPressed)
        state->currentScreen = APP_SCREEN_GAME;
    
    if (settingsNavButtonPressed)
        state->currentScreen = APP_SCREEN_SETTINGS;

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        state->currentScreen = APP_SCREEN_GAME;
    else if (IsKeyPressed(KEY_S))
        state->currentScreen = APP_SCREEN_SETTINGS;
    else if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
        CloseWindow();
        
    if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_L))
        state->currentScreen = APP_SCREEN_GAME;

    if (IsKeyPressed(KEY_M))
    {
        bool toggled = !(sfxCheckboxChecked || bgmCheckboxChecked);
        sfxCheckboxChecked = toggled;
        bgmCheckboxChecked = toggled;
    }
}

void MenuDraw(AppState* state)
{
    if (state == NULL) return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){ 24, 32, 38, 255 });

    int baseFontSize = screenHeight / 42; 
    if (baseFontSize < 16) baseFontSize = 16;
    ApplyScrabbleTheme(baseFontSize);

    const int padding = screenWidth / 25;
    const int panelGap = screenWidth / 40;
    
    // ----------------------------------------------------
    // HEADER: Giant Wood Scrabble Board Tiles 
    // ----------------------------------------------------
    const char* titleText = "SCRABBLE.C";
    int titleLength = 10;
    
    // Significantly increased tile proportions relative to global font scaling
    float menuTileSize = baseFontSize * 2.4f; 
    float menuTileSpacing = menuTileSize * 0.10f;
    float titleY = (float)padding;
    int tileFontSize = (int)(menuTileSize * 0.65f); // Large bold typography stamp
    
    for (int i = 0; i < titleLength; i++)
    {
        float currentX = (float)padding + i * (menuTileSize + menuTileSpacing);
        Rectangle tileRect = { currentX, titleY, menuTileSize, menuTileSize };

        DrawRectangleRounded((Rectangle){ tileRect.x + 4, tileRect.y + 5, tileRect.width, tileRect.height }, 0.18f, 4, (Color){ 10, 14, 18, 160 });
        DrawRectangleRounded(tileRect, 0.18f, 4, (Color){ 244, 228, 198, 255 });
        
        Rectangle innerBevel = { tileRect.x + 3, tileRect.y + 3, tileRect.width - 6, tileRect.height - 6 };
        DrawRectangleRoundedLines(innerBevel, 0.15f, 4, (Color){ 255, 248, 230, 255 });
        DrawRectangleRoundedLines(tileRect, 0.18f, 4, (Color){ 194, 169, 126, 255 });

        char letterStr[2] = { titleText[i], '\0' };
        int textWidth = MeasureText(letterStr, tileFontSize);
        DrawText(
            letterStr, 
            tileRect.x + (menuTileSize - textWidth) / 2.0f, 
            tileRect.y + (menuTileSize - tileFontSize) / 2.0f - 2.0f, 
            tileFontSize, 
            (Color){ 38, 28, 16, 255 }
        );

        if (titleText[i] != '.')
        {
            const char* score = GetMenuScrabbleScore(titleText[i]);
            int scoreFontSize = (int)(menuTileSize * 0.22f);
            DrawText(
                score, 
                tileRect.x + tileRect.width - (scoreFontSize * 1.3f), 
                tileRect.y + tileRect.height - (scoreFontSize * 1.4f), 
                scoreFontSize, 
                (Color){ 120, 95, 68, 255 }
            );
        }
    }
    
    float subtitleY = titleY + menuTileSize + 14.0f;
    float subtitleHeight = (float)baseFontSize * 1.2f;
    GuiLabel((Rectangle){ (float)padding, subtitleY, (float)screenWidth - (2 * padding), subtitleHeight }, "Be aware adventurer! Here every letter counts!");

    // Layout Containers Dimensions Parsing
    const int contentTop = (int)(subtitleY + subtitleHeight + 25);
    const int optionPanelWidth = screenWidth / 4 < 280 ? 280 : screenWidth / 4;
    const int mainPanelWidth = screenWidth - (2 * padding) - panelGap - optionPanelWidth;
    const int mainPanelHeight = screenHeight - contentTop - padding;

    const float mainPanelX = (float)padding;
    const float mainPanelY = (float)contentTop;
    const float mainPanelW = (float)mainPanelWidth;
    const float mainPanelH = (float)mainPanelHeight;

    const float optionPanelX = (float)(screenWidth - padding - optionPanelWidth);
    const float optionPanelY = (float)contentTop;
    const float optionPanelW = (float)optionPanelWidth;
    const float optionPanelH = (float)mainPanelHeight;

    // ----------------------------------------------------
    // MAIN WIDGET PANEL: Compact & Framed Game Selection
    // ----------------------------------------------------
    GuiGroupBox((Rectangle){ mainPanelX, mainPanelY, mainPanelW, mainPanelH }, "START A GAME");
    
    // Hardfixed absolute item scaling prevents massive vertical stretching on tall viewports
    float itemRowHeight = baseFontSize * 2.2f; 
    float innerSpacingY = baseFontSize * 0.4f;
    float menuContentStartY = mainPanelY + (baseFontSize * 1.5f); // Tightened top container deadspace

    float btnWidth = mainPanelW * 0.35f;
    if (btnWidth < 180) btnWidth = 180;
    float labelX = mainPanelX + btnWidth + 40;
    float labelW = mainPanelW - btnWidth - 60; 

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 0); 

    // Action Row 1: Local
    float currentY = menuContentStartY;
    newLocalGameButtonPressed = GuiButton((Rectangle){ mainPanelX + 25, currentY, btnWidth, itemRowHeight }, "New Local Game");
    GuiLabel((Rectangle){ labelX, currentY, labelW, itemRowHeight }, "Play on this device turn-by-turn");

    // Action Row 2: Networking
    currentY += itemRowHeight + innerSpacingY;
    newNetworkGameButtonPressed = GuiButton((Rectangle){ mainPanelX + 25, currentY, btnWidth, itemRowHeight }, "New Network Game");
    GuiLabel((Rectangle){ labelX, currentY, labelW, itemRowHeight }, "Play with friends within the LAN");

    // Action Row 3: Load File
    currentY += itemRowHeight + innerSpacingY;
    loadGameFromFileButtonPressed = GuiButton((Rectangle){ mainPanelX + 25, currentY, btnWidth, itemRowHeight }, "Load Saved Game");
    GuiLabel((Rectangle){ labelX, currentY, labelW, itemRowHeight }, "Load a previous saved game file");

    // Shortcut Footer anchored tightly to the base of the main panel instead of floating loose
    float footerH = baseFontSize * 1.5f;
    float lineY = mainPanelY + mainPanelH - footerH - 16.0f;
    GuiLine((Rectangle){ mainPanelX + 25, lineY, mainPanelW - 50, 8 }, NULL);
    GuiLabel((Rectangle){ mainPanelX + 25, lineY + 12.0f, mainPanelW - 50, footerH }, "Q: Quit   M: Mute Everything   L: Quick Load   N: Quick Local   F11: Fullscreen");

    // ----------------------------------------------------
    // RIGHT SIDEBAR PANEL: Compact Mixer & Bottom Anchored Navigation
    // ----------------------------------------------------
    GuiGroupBox((Rectangle){ optionPanelX, optionPanelY, optionPanelW, optionPanelH }, "SETTINGS & OPTIONS");

    float sideMarginX = 16.0f;
    float sideTopY = optionPanelY + (baseFontSize * 1.5f); 
    float audioBoxHeight = baseFontSize * 5.5f; // Absolute sizing tightly bounds checkboxes
    
    GuiGroupBox((Rectangle){ optionPanelX + sideMarginX, sideTopY, optionPanelW - (sideMarginX * 2), audioBoxHeight }, "Audio Mixer");
    
    float checkboxHeight = baseFontSize * 1.1f;
    float chkMarginX = sideMarginX + 16.0f;
    float sfxCheckboxY = sideTopY + (baseFontSize * 1.4f);
    float bgmCheckboxY = sfxCheckboxY + checkboxHeight + (baseFontSize * 0.8f);

    GuiCheckBox((Rectangle){ optionPanelX + chkMarginX, sfxCheckboxY, checkboxHeight, checkboxHeight }, "Sound Effects (SFX)", &sfxCheckboxChecked);
    GuiCheckBox((Rectangle){ optionPanelX + chkMarginX, bgmCheckboxY, checkboxHeight, checkboxHeight }, "Background Music (BGM)", &bgmCheckboxChecked);

    // Meta Actions anchored directly into the bottom corner limits
    float navBtnWidth = (optionPanelW - (sideMarginX * 2) - 16.0f) / 2.0f;
    float navBtnHeight = baseFontSize * 1.8f;
    float navY = optionPanelY + optionPanelH - navBtnHeight - 20.0f;
    
    aboutNavButtonPressed = GuiButton((Rectangle){ optionPanelX + sideMarginX, navY, navBtnWidth, navBtnHeight }, "About");
    settingsNavButtonPressed = GuiButton((Rectangle){ optionPanelX + sideMarginX + navBtnWidth + 16.0f, navY, navBtnWidth, navBtnHeight }, "Settings");
}