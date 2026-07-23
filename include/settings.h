#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct AppState AppState;

typedef enum SettingsTab
{
    SETTINGS_TAB_GAME = 0,
    SETTINGS_TAB_AUDIO,
    SETTINGS_TAB_NETWORK,
    SETTINGS_TAB_ADVANCED,
    SETTINGS_TAB_COUNT
} SettingsTab;

typedef struct SettingsState
{
    SettingsTab selectedOption;

    float bgmVolume; // 0.0f ~ 1.0f
    float sfxVolume; // 0.0f ~ 1.0f
    bool bgmEnable;
    bool sfxEnable;

    char dictionaryPath[256];
    char boardLayoutPath[256];
    char tileMapPath[256];

    bool dictionaryEditMode;
    bool boardLayoutEditMode;
    bool tileMapEditMode;

    bool luxuryTilesEnabled;

    bool showLoadingScreen;
} SettingsState;

// --- Lifecycle Management ---
SettingsState *InitSettingsState(void);
void FreeSettingsState(SettingsState *settings);
bool SaveSettingsToFile(const SettingsState *settings, const char *filePath);
bool LoadSettingsFromFile(SettingsState *settings, const char *filePath);

// --- UI Handlers ---
void SettingsUpdate(AppState *state);
void SettingsDraw(const AppState *state);

#if defined(__cplusplus)
}
#endif

#endif // SETTINGS_H