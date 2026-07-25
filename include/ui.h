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

// ---------------------------------------------------------------------------
// Hotkey Bar  (professional game-style keycap footer)
// ---------------------------------------------------------------------------

typedef struct {
    const char *key;    // e.g. "Q", "ESC", "F11"
    const char *label;  // e.g. "Quit", "Fullscreen"
} HotkeyEntry;

// Draws a row of keycap badges + labels inside a dark rounded container.
// x,y,w,h define the bounding rectangle; fontSize is the base size for text.
void DrawHotkeyBar(const HotkeyEntry *entries, int count,
                   float x, float y, float width, float height,
                   float fontSize);

#if defined(__cplusplus)
}
#endif