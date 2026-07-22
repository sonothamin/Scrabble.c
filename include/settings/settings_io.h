#ifndef SETTINGS_IO_H
#define SETTINGS_IO_H

#include "settings.h"

bool SaveSettingsToFile(const SettingsState *settings, const char *filePath);
bool LoadSettingsFromFile(SettingsState *settings, const char *filePath);

#endif // SETTINGS_IO_H
