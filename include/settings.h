#pragma once

#include <stdbool.h>
#include "app_state.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct AppState AppState;

typedef struct SettingsState {
    float musicVolume;
    float sfxVolume;
    bool  muteAll;
    int selectedOption;
} SettingsState;


//Life Cycle
SettingsState* InitSettingsState(void);
void FreeSettingsState(SettingsState* settings);

//UI
void SettingsUpdate(AppState* state);
void SettingsDraw(const AppState* state);

#if defined(__cplusplus)
}
#endif