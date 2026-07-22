#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "raygui.h"

#include "settings.h"
#include "settings/settings_internal.h"

#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"
#undef GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "settings/settings_io.h"
#include "settings/settings_game.h"
#include "settings/settings_audio.h"
#include "settings/settings_network.h"
#include "settings/settings_advanced.h"
#include "sound.h"
#include "ui.h"
#include "error_service.h"
#include "app_state.h"

// =============================================================================
// LIFECYCLE MANAGEMENT
// =============================================================================

SettingsState *InitSettingsState(void)
{
    ExtendedSettingsState *extSettings = (ExtendedSettingsState *)calloc(1, sizeof(ExtendedSettingsState));
    if (extSettings == NULL)
    {
        ReportCriticalError("Memory Allocation Failed", "Failed to allocate memory for SettingsState during initialization.");
        return NULL;
    }

    SettingsState *settings = &extSettings->base;

    // Default Fallback Values
    settings->selectedOption = SETTINGS_TAB_GAME;
    settings->bgmVolume = 0.8f;
    settings->sfxVolume = 1.0f;
    settings->bgmEnable = true;
    settings->sfxEnable = true;

    snprintf(settings->dictionaryPath, sizeof(settings->dictionaryPath), "resources/dictionary.txt");
    snprintf(settings->boardLayoutPath, sizeof(settings->boardLayoutPath), "resources/board_layout.txt");
    snprintf(settings->tileMapPath, sizeof(settings->tileMapPath), "resources/scores/letters.txt");

    settings->dictionaryEditMode = false;
    settings->boardLayoutEditMode = false;
    settings->tileMapEditMode = false;
    settings->luxuryTilesEnabled = true;
    settings->showLoadingScreen = true;

    LoadSettingsFromFile(settings, CONFIG_FILE_PATH);

    extSettings->fileDialogState = InitGuiWindowFileDialog(GetWorkingDirectory());
    extSettings->activeTarget = FILE_PICKER_NONE;

    extSettings->fileDialogState.windowBounds = (Rectangle){
        (GetScreenWidth() - 640.0f) / 2.0f,
        (GetScreenHeight() - 460.0f) / 2.0f,
        640.0f,
        460.0f};

    SetMusicVolumeLevel(settings->bgmEnable ? settings->bgmVolume : 0.0f);
    SetSfxVolumeLevel(settings->sfxEnable ? settings->sfxVolume : 0.0f);

    return settings;
}

void FreeSettingsState(SettingsState *settings)
{
    if (settings != NULL)
    {
        free(settings);
    }
}

// =============================================================================
// UPDATE LOGIC & DIALOG HELPERS
// =============================================================================

void HandleFileSelection(ExtendedSettingsState *extSettings)
{
    if (!extSettings->fileDialogState.SelectFilePressed)
        return;

    // Allocate buffer for path string
    char selectedPath[2048] = {0};
    snprintf(selectedPath, sizeof(selectedPath), "%s" PATH_SEPERATOR "%s",
             extSettings->fileDialogState.dirPathText,
             extSettings->fileDialogState.fileNameText);

    SettingsState *settings = &extSettings->base;

    switch (extSettings->activeTarget)
    {
    case FILE_PICKER_DICTIONARY:
        strncpy(settings->dictionaryPath, selectedPath, sizeof(settings->dictionaryPath) - 1);
        settings->dictionaryPath[sizeof(settings->dictionaryPath) - 1] = '\0';
        break;
    case FILE_PICKER_BOARD_LAYOUT:
        strncpy(settings->boardLayoutPath, selectedPath, sizeof(settings->boardLayoutPath) - 1);
        settings->boardLayoutPath[sizeof(settings->boardLayoutPath) - 1] = '\0';
        break;
    case FILE_PICKER_TILE_MAP:
        strncpy(settings->tileMapPath, selectedPath, sizeof(settings->tileMapPath) - 1);
        settings->tileMapPath[sizeof(settings->tileMapPath) - 1] = '\0';
        break;
    default:
        break;
    }

    extSettings->fileDialogState.SelectFilePressed = false;
    extSettings->activeTarget = FILE_PICKER_NONE;
}

void SettingsUpdate(AppState *state)
{
    if (state == NULL || state->settingsState == NULL)
    {
        ReportCriticalError(
            "Invalid State Pointer",
            "NULL pointer encountered during SettingsUpdate.");
        return;
    }

    ExtendedSettingsState *extSettings = (ExtendedSettingsState *)state->settingsState;
    SettingsState *settings = &extSettings->base;

    // Handle completed selection from dialog
    HandleFileSelection(extSettings);

    bool isEditingText = settings->dictionaryEditMode ||
                         settings->boardLayoutEditMode ||
                         settings->tileMapEditMode;

    bool isDialogOpen = extSettings->fileDialogState.windowActive;

    // Save configuration and return to main menu
    if ((IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_B)) && !isEditingText && !isDialogOpen)
    {
        PlaySoundEffect(SFX_BACK_NAV);
        SaveSettingsToFile(settings, CONFIG_FILE_PATH);
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        return;
    }

    // Navigation shortcuts (blocked when modal dialog is active)
    if (!isEditingText && !isDialogOpen)
    {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_D))
        {
            settings->selectedOption = (SettingsTab)((settings->selectedOption + 1) % SETTINGS_TAB_COUNT);
            PlaySoundEffect(SFX_BUTTON);
        }
        else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_A))
        {
            settings->selectedOption = (SettingsTab)((settings->selectedOption - 1 + SETTINGS_TAB_COUNT) % SETTINGS_TAB_COUNT);
            PlaySoundEffect(SFX_BUTTON);
        }

        if (IsKeyPressed(KEY_ONE))
        {
            settings->selectedOption = SETTINGS_TAB_GAME;
            PlaySoundEffect(SFX_BUTTON);
        }
        if (IsKeyPressed(KEY_TWO))
        {
            settings->selectedOption = SETTINGS_TAB_AUDIO;
            PlaySoundEffect(SFX_BUTTON);
        }
        if (IsKeyPressed(KEY_THREE))
        {
            settings->selectedOption = SETTINGS_TAB_NETWORK;
            PlaySoundEffect(SFX_BUTTON);
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            settings->selectedOption = SETTINGS_TAB_ADVANCED;
            PlaySoundEffect(SFX_BUTTON);
        }
    }
}

// =============================================================================
// RENDERING LOGIC
// =============================================================================

bool DrawSettingsTabButton(const char *text, Rectangle bounds, bool isActive, int fontSize, bool isModalActive)
{
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), bounds) && !isModalActive;

    Color bg = isActive ? GetColor(GuiGetStyle(BUTTON, BASE_COLOR_PRESSED))
                        : (isHovered ? GetColor(GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED))
                                     : GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    Color textCol = isActive ? GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED))
                             : (isHovered ? GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_FOCUSED))
                                          : GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    DrawRectangleRec(bounds, bg);
    DrawRectangleLinesEx(bounds, 1.5f, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
    DrawAppText(text, bounds.x + 15.0f, bounds.y + (bounds.height - fontSize) / 2.0f, fontSize, textCol);

    return (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}

void SettingsDraw(const AppState *state)
{
    if (state == NULL || state->settingsState == NULL)
    {
        ReportCriticalError(
            "Invalid State Pointer",
            "NULL pointer encountered during SettingsDraw.");
        return;
    }

    AppState *mutableState = (AppState *)state;
    ExtendedSettingsState *extSettings = (ExtendedSettingsState *)state->settingsState;
    SettingsState *settings = &extSettings->base;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){24, 32, 38, 255});

    int baseFontSize = screenHeight / 38;
    if (baseFontSize < 16)
    {
        baseFontSize = 16;
    }
    ApplyScrabbleTheme(baseFontSize);

    const float padding = screenWidth / 25.0f;
    const float headerLineY = padding + (baseFontSize * 1.5f) + 15.0f;
    const bool isDialogOpen = extSettings->fileDialogState.windowActive;

    if (isDialogOpen)
    {
        GuiLock();
    }

    // --- Header ---
    DrawAppText("SETTINGS & CONFIGURATION", padding, padding, baseFontSize * 1.7f, WHITE);
    DrawLineEx((Vector2){padding, headerLineY}, (Vector2){screenWidth - padding, headerLineY}, 2, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

    // --- Sidebar Navigation ---
    float sidebarX = padding;
    float sidebarY = headerLineY + 25.0f;
    float tabWidth = baseFontSize * 11.0f;
    float tabHeight = baseFontSize * 2.2f;
    float tabSpacing = 8.0f;

    if (DrawSettingsTabButton("[1] Game Rules", (Rectangle){sidebarX, sidebarY, tabWidth, tabHeight}, settings->selectedOption == SETTINGS_TAB_GAME, baseFontSize * 0.85f, isDialogOpen))
    {
        settings->selectedOption = SETTINGS_TAB_GAME;
        PlaySoundEffect(SFX_BUTTON);
    }
    if (DrawSettingsTabButton("[2] Audio Mixer", (Rectangle){sidebarX, sidebarY + tabHeight + tabSpacing, tabWidth, tabHeight}, settings->selectedOption == SETTINGS_TAB_AUDIO, baseFontSize * 0.85f, isDialogOpen))
    {
        settings->selectedOption = SETTINGS_TAB_AUDIO;
        PlaySoundEffect(SFX_BUTTON);
    }
    if (DrawSettingsTabButton("[3] Network & LAN", (Rectangle){sidebarX, sidebarY + (tabHeight + tabSpacing) * 2.0f, tabWidth, tabHeight}, settings->selectedOption == SETTINGS_TAB_NETWORK, baseFontSize * 0.85f, isDialogOpen))
    {
        settings->selectedOption = SETTINGS_TAB_NETWORK;
        PlaySoundEffect(SFX_BUTTON);
    }
    if (DrawSettingsTabButton("[4] Advanced", (Rectangle){sidebarX, sidebarY + (tabHeight + tabSpacing) * 3.0f, tabWidth, tabHeight}, settings->selectedOption == SETTINGS_TAB_ADVANCED, baseFontSize * 0.85f, isDialogOpen))
    {
        settings->selectedOption = SETTINGS_TAB_ADVANCED;
        PlaySoundEffect(SFX_BUTTON);
    }

    // --- Main Panel Container ---
    float contentLeft = sidebarX + tabWidth + 40.0f;
    float contentTop = sidebarY + 30.0f;
    float contentWidth = screenWidth - contentLeft - padding;
    float contentHeight = screenHeight - contentTop - padding - 35.0f;

    Rectangle contentBox = {contentLeft, contentTop, contentWidth, contentHeight};

    switch (settings->selectedOption)
    {
    case SETTINGS_TAB_GAME:
        GuiGroupBox(contentBox, "--< Board & Asset Configuration >--");
        break;
    case SETTINGS_TAB_AUDIO:
        GuiGroupBox(contentBox, "--< Audio & Sound Mixer >--");
        break;
    case SETTINGS_TAB_NETWORK:
        GuiGroupBox(contentBox, "--< Network & Multiplayer >--");
        break;
    case SETTINGS_TAB_ADVANCED:
        GuiGroupBox(contentBox, "--< Advanced System Options >--");
        break;
    default:
        break;
    }

    float contentX = contentBox.x + 30.0f;
    float contentY = contentBox.y + 35.0f;

    BeginScissorMode((int)contentBox.x + 5, (int)contentBox.y + 5, (int)contentBox.width - 10, (int)contentBox.height - 10);

    switch (settings->selectedOption)
    {
    case SETTINGS_TAB_GAME:
        DrawSettingsGameTab(extSettings, contentX, contentY, contentWidth, baseFontSize);
        break;
    case SETTINGS_TAB_AUDIO:
        DrawSettingsAudioTab(extSettings, contentX, contentY, contentWidth, baseFontSize);
        break;
    case SETTINGS_TAB_NETWORK:
        DrawSettingsNetworkTab(extSettings, contentX, contentY, contentWidth, baseFontSize);
        break;
    case SETTINGS_TAB_ADVANCED:
        DrawSettingsAdvancedTab(extSettings, contentX, contentY, contentWidth, baseFontSize);
        break;
    default:
        break;
    }

    EndScissorMode();

    // --- Footer Controls ---
    float footerY = screenHeight - padding;
    DrawAppText("Navigate: [A/D] or Keys [1-4]  |  [B/ESC] Save & Return", padding, footerY, baseFontSize * 0.75f, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_DISABLED)));

    float backBtnWidth = baseFontSize * 10.5f;
    float backBtnHeight = baseFontSize * 1.6f;
    float backBtnX = screenWidth - padding - backBtnWidth;
    float backBtnY = footerY - (backBtnHeight * 0.3f);

    if (GuiButton((Rectangle){backBtnX, backBtnY, backBtnWidth, backBtnHeight}, "Save and Back (B)"))
    {
        PlaySoundEffect(SFX_BACK_NAV);
        SaveSettingsToFile(settings, CONFIG_FILE_PATH);
        mutableState->currentScreen = APP_SCREEN_MAIN_MENU;
    }

    // --- Unlock GUI controls if locked ---
    if (isDialogOpen)
    {
        GuiUnlock();
    }

    // =========================================================================
    // MODAL DIALOG DRAWING OVERLAY
    // =========================================================================
    if (extSettings->fileDialogState.windowActive)
    {
        const char *dialogTitle = "Select File";
        switch (extSettings->activeTarget)
        {
        case FILE_PICKER_DICTIONARY:
            dialogTitle = "Select Dictionary File";
            break;
        case FILE_PICKER_BOARD_LAYOUT:
            dialogTitle = "Select Board Layout File";
            break;
        case FILE_PICKER_TILE_MAP:
            dialogTitle = "Select Tile Map File";
            break;
        default:
            break;
        }

        ApplyAltTheme(18);
        GuiWindowFileDialog(&extSettings->fileDialogState, (char *)dialogTitle);
        ApplyScrabbleTheme(baseFontSize);
    }
}
