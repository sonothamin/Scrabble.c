#include "raylib.h"
#include "raygui.h"
#include "error_service.h"

#include "menu.h"
#include "ui.h"
#include "sound.h"
#include "settings.h"

#include <math.h>

// -----------------------------------------------------------------------------
// Constants & Configuration
// -----------------------------------------------------------------------------
static const Color MENU_BG_COLOR         = { 24,  32,  38,  255 };
static const Color TILE_SHADOW_COLOR     = { 10,  14,  18,  160 };
static const Color TILE_FILL_COLOR       = { 244, 228, 198, 255 };
static const Color TILE_INNER_LINE_COLOR = { 255, 248, 230, 255 };
static const Color TILE_OUTER_LINE_COLOR = { 194, 169, 126, 255 };
static const Color TILE_TEXT_COLOR       = { 38,  28,  16,  255 };
static const Color TILE_SCORE_COLOR      = { 120, 95,  68,  255 };

typedef struct {
    Texture2D background;
    Texture2D newGame;
    Texture2D network;
    Texture2D loadSave;
    bool isLoaded;
} MenuTextures;

static MenuTextures g_menuTextures = { 0 };

static const HotkeyEntry MENU_HOTKEYS[] = {
    { "N",   "New Game"   },
    { "L",   "Load Saved" },
    { "M",   "Mute"       },
    { "S",   "Settings"   },
    { "F11", "Fullscreen" },
    { "Q",   "Quit"       },
};
static const int MENU_HOTKEYS_COUNT = sizeof(MENU_HOTKEYS) / sizeof(MENU_HOTKEYS[0]);

// -----------------------------------------------------------------------------
// Helper Functions
// -----------------------------------------------------------------------------
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

static void MenuInitTextures(void)
{
    if (g_menuTextures.isLoaded) return;

    g_menuTextures.background = LoadTexture("images/menu_bg.png");
    g_menuTextures.newGame    = LoadTexture("images/new.png");
    g_menuTextures.network    = LoadTexture("images/network.png");
    g_menuTextures.loadSave   = LoadTexture("images/saved.png");
    g_menuTextures.isLoaded   = true;
}

void MenuUnloadTextures(void)
{
    if (!g_menuTextures.isLoaded) return;

    UnloadTexture(g_menuTextures.background);
    UnloadTexture(g_menuTextures.newGame);
    UnloadTexture(g_menuTextures.network);
    UnloadTexture(g_menuTextures.loadSave);
    
    g_menuTextures = (MenuTextures){ 0 };
}

static void ToggleAudioMute(SettingsState* settings)
{
    if (!settings) return;

    bool enable = !(settings->sfxEnable || settings->bgmEnable);
    settings->sfxEnable = enable;
    settings->bgmEnable = enable;

    SetMusicVolumeLevel(enable ? settings->bgmVolume : 0.0f);
    SetSfxVolumeLevel(enable ? settings->sfxVolume : 0.0f);

    PlaySoundEffect(SFX_BUTTON);
}

// -----------------------------------------------------------------------------
// Drawing Subroutines
// -----------------------------------------------------------------------------
static void DrawHeaderBackground(Rectangle headerArea)
{
    if (g_menuTextures.background.id > 0)
    {
        Rectangle srcRec = { 0, 0, (float)g_menuTextures.background.width, (float)g_menuTextures.background.height };
        DrawTexturePro(g_menuTextures.background, srcRec, headerArea, (Vector2){ 0 }, 0.0f, Fade(WHITE, 0.35f));
    }

    DrawRectangleGradientV(0, headerArea.height * 0.4f, headerArea.width, headerArea.height * 0.6f, Fade(MENU_BG_COLOR, 0.0f), MENU_BG_COLOR);
    DrawRectangleGradientH(0, 0, headerArea.width * 0.15f, headerArea.height, MENU_BG_COLOR, Fade(MENU_BG_COLOR, 0.0f));
    DrawRectangleGradientH(headerArea.width * 0.85f, 0, headerArea.width * 0.15f, headerArea.height, Fade(MENU_BG_COLOR, 0.0f), MENU_BG_COLOR);
}

static void DrawTitleBanner(float padding, float tileSize)
{
    const char* titleText = "SCRABBLE.C";
    const float tileSpacing = tileSize * 0.10f;
    const int tileFontSize = tileSize * 0.65f; 
    const int scoreFontSize = tileSize * 0.22f;

    for (int i = 0; titleText[i] != '\0'; i++)
    {
        Rectangle tileRect = { padding + i * (tileSize + tileSpacing), padding, tileSize, tileSize };

        DrawRectangleRounded((Rectangle){ tileRect.x + 5.0f, tileRect.y + 6.0f, tileSize, tileSize }, 0.18f, 4, TILE_SHADOW_COLOR);
        DrawRectangleRounded(tileRect, 0.18f, 4, TILE_FILL_COLOR);
        DrawRectangleRoundedLines((Rectangle){ tileRect.x + 4.0f, tileRect.y + 4.0f, tileSize - 8.0f, tileSize - 8.0f }, 0.15f, 4, TILE_INNER_LINE_COLOR);
        DrawRectangleRoundedLines(tileRect, 0.18f, 4, TILE_OUTER_LINE_COLOR);

        char letterStr[2] = { titleText[i], '\0' };
        float textX = tileRect.x + (tileSize - MeasureAppText(letterStr, tileFontSize)) / 2.0f;
        float textY = tileRect.y + (tileSize - tileFontSize) / 2.0f - 4.0f;
        DrawAppText(letterStr, textX, textY, tileFontSize, TILE_TEXT_COLOR);

        if (titleText[i] != '.')
        {
            float scoreX = tileRect.x + tileSize - scoreFontSize - 6.0f;
            float scoreY = tileRect.y + tileSize - scoreFontSize - 6.0f;
            DrawAppText(GetMenuScrabbleScore(titleText[i]), scoreX, scoreY, scoreFontSize, TILE_SCORE_COLOR);
        }
    }
}

static void DrawStartGamePanel(AppState* state, Rectangle panelRect, float rowHeight, float targetBtnHeight, int baseFontSize)
{
    GuiGroupBox(panelRect, "START A GAME");

    const float btnWidth = fmaxf(panelRect.width * 0.38f, 180.0f);
    const float legendBtnX = panelRect.x + 45.0f;
    const float legendBtnW = btnWidth - 20.0f;
    const float labelX = panelRect.x + btnWidth + 40.0f;
    const float labelW = panelRect.width - btnWidth - 60.0f;

    const Texture2D* textures[] = { &g_menuTextures.newGame, &g_menuTextures.network, &g_menuTextures.loadSave };
    const char* labels[] = { "New Local Game", "New Network Game", "Load Saved Game" };
    const char* descs[]  = { "Play on this device turn-by-turn", "Play with friends within the LAN", "Load a previous saved game file" };

    for (int i = 0; i < 3; i++)
    {
        float rowY = panelRect.y + rowHeight * (0.8f + i);
        if (DrawLegendButton(*textures[i], labels[i], (Rectangle){ legendBtnX, rowY, legendBtnW, targetBtnHeight }, baseFontSize, false)) {
            PlaySoundEffect(i == 2 ? SFX_BUTTON : SFX_GAME_START);
            StartNewGame(state);
        }
        GuiLabel((Rectangle){ labelX, rowY, labelW, targetBtnHeight }, descs[i]);
    }

    GuiLine((Rectangle){ panelRect.x + 25.0f, panelRect.y + (rowHeight * 4.6f), panelRect.width - 50.0f, 8.0f }, NULL);
    DrawHotkeyBar(MENU_HOTKEYS, MENU_HOTKEYS_COUNT, panelRect.x + 25.0f, panelRect.y + (rowHeight * 4.85f), panelRect.width - 50.0f, rowHeight * 0.40f, baseFontSize);
}

static void DrawSettingsPanel(AppState* state, Rectangle panelRect, float targetBtnHeight, int baseFontSize)
{
    GuiGroupBox(panelRect, "SETTINGS & OPTIONS");

    const float marginX = 30.0f;
    const float soundBoxWidth = panelRect.width - (marginX * 2.0f);
    const float soundBoxHeight = panelRect.height * 0.38f;
    
    Rectangle soundBoxRect = { panelRect.x + marginX, panelRect.y + 50.0f, soundBoxWidth, soundBoxHeight };
    GuiGroupBox(soundBoxRect, "Audio Mixer");
    
    const float checkboxHeight = fminf(fmaxf((float)baseFontSize, 16.0f), 22.0f);

    if (state->settingsState)
    {
        SettingsState* settings = state->settingsState;
        bool prevSfx = settings->sfxEnable;
        bool prevBgm = settings->bgmEnable;

        GuiCheckBox((Rectangle){ soundBoxRect.x + 15.0f, soundBoxRect.y + (soundBoxHeight * 0.28f), checkboxHeight, checkboxHeight }, "Sound Effects (SFX)", &settings->sfxEnable);
        GuiCheckBox((Rectangle){ soundBoxRect.x + 15.0f, soundBoxRect.y + (soundBoxHeight * 0.62f), checkboxHeight, checkboxHeight }, "Background Music (BGM)", &settings->bgmEnable);

        if (prevSfx != settings->sfxEnable)
        {
            PlaySoundEffect(SFX_BUTTON);
            SetSfxVolumeLevel(settings->sfxEnable ? settings->sfxVolume : 0.0f);
        }
        if (prevBgm != settings->bgmEnable)
        {
            PlaySoundEffect(SFX_BUTTON);
            SetMusicVolumeLevel(settings->bgmEnable ? settings->bgmVolume : 0.0f);
        }
    }

    const float navBtnWidth = (panelRect.width - 56.0f) / 2.0f;
    const float navBtnY = panelRect.y + panelRect.height - targetBtnHeight - 20.0f;
    
    if (GuiButton((Rectangle){ panelRect.x + 20.0f, navBtnY, navBtnWidth, targetBtnHeight }, "About")) {
        PlaySoundEffect(SFX_BUTTON);
        state->currentScreen = APP_SCREEN_ABOUT;
    }
    if (GuiButton((Rectangle){ panelRect.x + 36.0f + navBtnWidth, navBtnY, navBtnWidth, targetBtnHeight }, "Settings")) {
        PlaySoundEffect(SFX_BUTTON);
        state->currentScreen = APP_SCREEN_SETTINGS;
    }
}

void MenuUpdate(AppState* state)
{
    if (!state)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered while updating Menu.");
        return;
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_N))
    {
        PlaySoundEffect(SFX_GAME_START);
        StartNewGame(state);
    }
    else if (IsKeyPressed(KEY_S))
    {
        PlaySoundEffect(SFX_BUTTON);
        state->currentScreen = APP_SCREEN_SETTINGS;
    }
    else if (IsKeyPressed(KEY_Q))
    {
        state->shouldClose = true;
    }
    else if (IsKeyPressed(KEY_F11))
    {
        ToggleFullscreen();
    }
    else if (IsKeyPressed(KEY_M))
    {
        ToggleAudioMute(state->settingsState);
    }
}

void MenuDraw(AppState* state)
{
    if (!state)
    {
        ReportCriticalError("Invalid App State", "NULL AppState pointer encountered while drawing Menu.");
        return;
    }

    MenuInitTextures();

    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();

    ClearBackground(MENU_BG_COLOR);

    int baseFontSize = fmaxf(screenHeight / 38.0f, 16.0f); 
    ApplyScrabbleTheme(baseFontSize);

    const float padding = screenWidth / 25.0f;
    const float panelGap = screenWidth / 35.0f;
    const float menuTileSize = baseFontSize * 3.0f;
    const float subtitleY = padding + menuTileSize + 16.0f;
    const float subtitleHeight = baseFontSize * 1.2f;
    const float contentTop = subtitleY + subtitleHeight + 45.0f;

    DrawHeaderBackground((Rectangle){ 0, 0, screenWidth, contentTop - 15.0f });
    DrawTitleBanner(padding, menuTileSize);
    
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 0); 
    GuiLabel((Rectangle){ padding, subtitleY, screenWidth - (2.0f * padding), subtitleHeight }, "Be aware adventurer! Here every letter counts!");

    const float optionPanelWidth = fmaxf(screenWidth / 4.0f, 260.0f);
    const float mainPanelWidth = screenWidth - (2.0f * padding) - panelGap - optionPanelWidth;
    const float mainPanelHeight = screenHeight - contentTop - padding;

    const float rowHeight = mainPanelHeight / 6.0f;
    const float targetBtnHeight = rowHeight * 0.65f;

    DrawStartGamePanel(state, (Rectangle){ padding, contentTop, mainPanelWidth, mainPanelHeight }, rowHeight, targetBtnHeight, baseFontSize);
    DrawSettingsPanel(state, (Rectangle){ screenWidth - padding - optionPanelWidth, contentTop, optionPanelWidth, mainPanelHeight }, targetBtnHeight, baseFontSize);
}