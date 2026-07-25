#pragma once

#include "raylib.h"
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

void DrawScreenBox(const char* title, const char* instructions);
void ApplyScrabbleTheme(int baseFontSize);
void ApplyAltTheme(int baseFontSize);

void InitAppFont(void);
void UnloadAppFont(void);
Font GetAppFont(void);

void DrawAppText(const char* text, float x, float y, float fontSize, Color color);
int  MeasureAppText(const char* text, float fontSize);

bool DrawTabButton(const char *text, Rectangle bounds, bool isActive, int fontSize, bool isModalActive);

typedef struct {
    const char *key;
    const char *label;
} HotkeyEntry;

void DrawHotkeyBar(const HotkeyEntry *entries, int count,
                   float x, float y, float width, float height,
                   float fontSize);

bool DrawLegendButton(Texture2D image, const char *text, Rectangle bounds, int fontSize, bool isModalActive);


#if defined(__cplusplus)
}
#endif