#include <stdio.h>

#include "raylib.h"
#include "settings/settings_io.h"

bool SaveSettingsToFile(const SettingsState *settings, const char *filePath)
{
    if (settings == NULL || filePath == NULL)
    {
        return false;
    }

    FILE *file = fopen(filePath, "wb");
    if (file == NULL)
    {
        TraceLog(LOG_WARNING, "SETTINGS: Failed to open %s for writing.", filePath);
        return false;
    }

    size_t written = fwrite(settings, sizeof(SettingsState), 1, file);
    fclose(file);

    if (written == 1)
    {
        TraceLog(LOG_INFO, "SETTINGS: Configuration saved successfully to %s", filePath);
        return true;
    }

    return false;
}

bool LoadSettingsFromFile(SettingsState *settings, const char *filePath)
{
    if (settings == NULL || filePath == NULL)
    {
        return false;
    }

    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
    {
        TraceLog(LOG_INFO, "SETTINGS: No existing %s found, using defaults.", filePath);
        return false;
    }

    size_t readCount = fread(settings, sizeof(SettingsState), 1, file);
    fclose(file);

    if (readCount == 1)
    {
        settings->dictionaryEditMode = false;
        settings->boardLayoutEditMode = false;
        settings->tileMapEditMode = false;
        settings->selectedOption = SETTINGS_TAB_GAME;

        TraceLog(LOG_INFO, "SETTINGS: Configuration loaded successfully from %s", filePath);
        return true;
    }

    return false;
}
