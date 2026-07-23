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

#if defined(__cplusplus)
}
#endif