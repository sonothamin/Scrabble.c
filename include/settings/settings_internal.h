#ifndef SETTINGS_INTERNAL_H
#define SETTINGS_INTERNAL_H

#ifndef RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
#define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 30
#endif

#include "raylib.h"
#include "raygui.h"
#include "gui_window_file_dialog.h"
#include "settings.h"

#define CONFIG_FILE_PATH "config.dat"

typedef enum
{
    FILE_PICKER_NONE = 0,
    FILE_PICKER_DICTIONARY,
    FILE_PICKER_BOARD_LAYOUT,
    FILE_PICKER_TILE_MAP
} FilePickerTarget;

typedef struct
{
    SettingsState base;
    GuiWindowFileDialogState fileDialogState;
    FilePickerTarget activeTarget;
} ExtendedSettingsState;

// Shared Helper UI Functions
bool DrawSettingsTabButton(const char *text, Rectangle bounds, bool isActive, int fontSize, bool isModalActive);
void HandleFileSelection(ExtendedSettingsState *extSettings);

#endif // SETTINGS_INTERNAL_H
