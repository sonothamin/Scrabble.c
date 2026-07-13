#include "menu.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static bool sfxCheckboxChecked = true;  
static bool bgmCheckboxChecked = true;  

// Internal helper for Scrabble Title Card layout point values
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

// Applies custom themes to RayGui controls matching the dark loading screen palette
static void ApplyScrabbleTheme(int baseFontSize)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize);
    
    // Background Slate Panel Styling
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x182026FF); 
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x2ECA7140); 
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x697D8CFF);

    // Button & Control Styling (Rich Charcoal + Sharp Emerald Accents)
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x242C34FF);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x364452FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x2ECA7115);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x2ECA71AA);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0x2ECA71FF);
    
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x2ECA7130);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0x2ECA71FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0x8FF2B8FF);

    // Checkbox Theme overrides using standard control properties
    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x242C34FF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x364452FF);
    GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    
    GuiSetStyle(CHECKBOX, BASE_COLOR_PRESSED, 0x2ECA71FF); 
    GuiSetStyle(CHECKBOX, BORDER_COLOR_PRESSED, 0x8FF2B8FF);
}

void MenuUpdate(AppState* state)
{
    if (state == NULL) return;

    // Global Key Bind Controllers
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_N) || IsKeyPressed(KEY_L))
        state->currentScreen = APP_SCREEN_GAME;
    else if (IsKeyPressed(KEY_S))
        state->currentScreen = APP_SCREEN_SETTINGS;
    else if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
        CloseWindow();

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

    int baseFontSize = screenHeight / 38; 
    if (baseFontSize < 16) baseFontSize = 16;
    
    ApplyScrabbleTheme(baseFontSize);

    const int padding = screenWidth / 25;
    const int panelGap = screenWidth / 35;
    
    // ----------------------------------------------------
    // TITLE: Procedural Wood Scrabble Board Tiles (BIGGG)
    // ----------------------------------------------------
    const char* titleText = "SCRABBLE.C";
    const int titleLength = 10;
    
    float menuTileSize = baseFontSize * 3.0f;
    float menuTileSpacing = menuTileSize * 0.10f;
    float titleY = (float)padding;
    
    for (int i = 0; i < titleLength; i++)
    {
        float currentX = (float)padding + i * (menuTileSize + menuTileSpacing);
        Rectangle tileRect = { currentX, titleY, menuTileSize, menuTileSize };

        // 3D Drop Shadow effect
        DrawRectangleRounded((Rectangle){ tileRect.x + 5, tileRect.y + 6, tileRect.width, tileRect.height }, 0.18f, 4, (Color){ 10, 14, 18, 160 });

        // Birch Wood Ivory Body Base
        DrawRectangleRounded(tileRect, 0.18f, 4, (Color){ 244, 228, 198, 255 });
        
        // Inner Beveling Frame
        Rectangle innerBevel = { tileRect.x + 4, tileRect.y + 4, tileRect.width - 8, tileRect.height - 8 };
        DrawRectangleRoundedLines(innerBevel, 0.15f, 4, (Color){ 255, 248, 230, 255 });
        DrawRectangleRoundedLines(tileRect, 0.18f, 4, (Color){ 194, 169, 126, 255 });

        // Character Core Stamp scaled proportionally
        char letterStr[2] = { titleText[i], '\0' };
        int tileFontSize = (int)(menuTileSize * 0.65f); 
        int textWidth = MeasureText(letterStr, tileFontSize);
        DrawText(
            letterStr, 
            tileRect.x + (menuTileSize - textWidth) / 2.0f, 
            tileRect.y + (menuTileSize - tileFontSize) / 2.0f - 4.0f, 
            tileFontSize, 
            (Color){ 38, 28, 16, 255 }
        );

        // Score Subscript Indexing scaled proportionally
        if (titleText[i] != '.')
        {
            const char* score = GetMenuScrabbleScore(titleText[i]);
            int scoreFontSize = (int)(menuTileSize * 0.22f);
            DrawText(
                score, 
                tileRect.x + tileRect.width - scoreFontSize - 6.0f, 
                tileRect.y + tileRect.height - scoreFontSize - 6.0f, 
                scoreFontSize, 
                (Color){ 120, 95, 68, 255 }
            );
        }
    }
    
    float subtitleY = titleY + menuTileSize + 16.0f;
    float subtitleHeight = (float)baseFontSize * 1.2f;
    GuiLabel((Rectangle){ (float)padding, subtitleY, (float)screenWidth - (2 * padding), subtitleHeight }, "Be aware adventurer! Here every letter counts!");

    // Layout Containers Dimensions Parsing
    const int contentTop = (int)(subtitleY + subtitleHeight + 45);
    const int optionPanelWidth = screenWidth / 4 < 260 ? 260 : screenWidth / 4;
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
    // MAIN WIDGET PANEL: Game Management Group
    // ----------------------------------------------------
    GuiGroupBox((Rectangle){ mainPanelX, mainPanelY, mainPanelW, mainPanelH }, "START A GAME");
    
    float rowHeight = mainPanelH / 6.0f;
    float btnWidth = mainPanelW * 0.38f;
    if (btnWidth < 180) btnWidth = 180;
    
    float labelX = mainPanelX + btnWidth + 40;
    float labelW = mainPanelW - btnWidth - 60; 

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 0); 

    // Action Row 1: Local Setup
    if (GuiButton((Rectangle){ mainPanelX + 25, mainPanelY + rowHeight * 0.8f, btnWidth, rowHeight * 0.65f }, "New Local Game")) {
        state->currentScreen = APP_SCREEN_GAME;
    }
    GuiLabel((Rectangle){ labelX, mainPanelY + rowHeight * 0.8f, labelW, rowHeight * 0.65f }, "Play on this device turn-by-turn");

    // Action Row 2: Networking Infrastructure Setup
    if (GuiButton((Rectangle){ mainPanelX + 25, mainPanelY + (rowHeight * 1.8f), btnWidth, rowHeight * 0.65f }, "New Network Game")) {
        state->currentScreen = APP_SCREEN_GAME;
    }
    GuiLabel((Rectangle){ labelX, mainPanelY + (rowHeight * 1.8f), labelW, rowHeight * 0.65f }, "Play with friends within the LAN");

    // Action Row 3: Disk I/O Parser
    if (GuiButton((Rectangle){ mainPanelX + 25, mainPanelY + (rowHeight * 2.8f), btnWidth, rowHeight * 0.65f }, "Load Saved Game")) {
        state->currentScreen = APP_SCREEN_GAME;
    }
    GuiLabel((Rectangle){ labelX, mainPanelY + (rowHeight * 2.8f), labelW, rowHeight * 0.65f }, "Load a previous saved game file");

    // Shortcut Command Keys Footer Bar
    GuiLine((Rectangle){ mainPanelX + 25, mainPanelY + (rowHeight * 4.6f), mainPanelW - 50, 8 }, NULL);
    GuiLabel((Rectangle){ mainPanelX + 25, mainPanelY + (rowHeight * 4.9f), mainPanelW - 50, rowHeight * 0.5f }, "Q: Quit   M: Mute Everything   L: Quick Load   N: Quick Local   F11: Fullscreen");

    // ----------------------------------------------------
    // RIGHT SIDEBAR PANEL: Options & Meta Controllers
    // ----------------------------------------------------
    GuiGroupBox((Rectangle){ optionPanelX, optionPanelY, optionPanelW, optionPanelH }, "SETTINGS & OPTIONS");

    float soundGroupPaddingX = 16.0f;
    float soundGroupPaddingY = 24.0f;
    float soundInnerPaddingX = 20.0f;
    
    float soundBoxHeight = optionPanelH * 0.38f;
    GuiGroupBox((Rectangle){ optionPanelX + soundGroupPaddingX, optionPanelY + soundGroupPaddingY, optionPanelW - (soundGroupPaddingX * 2), soundBoxHeight }, "Audio Mixer");
    
    float checkboxHeight = baseFontSize * 1.0f;
    if (checkboxHeight > 22.0f) checkboxHeight = 22.0f;
    if (checkboxHeight < 16.0f) checkboxHeight = 16.0f;

    float sfxCheckboxY = optionPanelY + soundGroupPaddingY + (soundBoxHeight * 0.28f);
    float bgmCheckboxY = optionPanelY + soundGroupPaddingY + (soundBoxHeight * 0.62f);

    GuiCheckBox((Rectangle){ optionPanelX + soundGroupPaddingX + soundInnerPaddingX, sfxCheckboxY, checkboxHeight, checkboxHeight }, "Sound Effects (SFX)", &sfxCheckboxChecked);
    GuiCheckBox((Rectangle){ optionPanelX + soundGroupPaddingX + soundInnerPaddingX, bgmCheckboxY, checkboxHeight, checkboxHeight }, "Background Music (BGM)", &bgmCheckboxChecked);

    // Meta Control Footer Actions
    float navBtnWidth = (optionPanelW - 48) / 2.0f;
    float navBtnHeight = rowHeight * 0.6f;
    if (navBtnHeight > 42) navBtnHeight = 42;
    
    if (GuiButton((Rectangle){ optionPanelX + 16, optionPanelY + optionPanelH - navBtnHeight - 20, navBtnWidth, navBtnHeight }, "About")) {
        // Handle "About" screen routing if state enum exists later
    }
    
    if (GuiButton((Rectangle){ optionPanelX + 16 + navBtnWidth + 16, optionPanelY + optionPanelH - navBtnHeight - 20, navBtnWidth, navBtnHeight }, "Settings")) {
        state->currentScreen = APP_SCREEN_SETTINGS;
    }
}