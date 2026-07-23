#include "ui.h"
#include <math.h>

#define RAYGUI_IMPLEMENTATION
#define FONT "PTSerif-Regular.ttf"
#include "raygui.h"

void DrawScreenBox(const char *title, const char *instructions)
{
    const int boxX = 100;
    const int boxY = 100;
    const int boxWidth = 600;
    const int boxHeight = 400;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(LIGHTGRAY, 0.55f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, GRAY);

    const int titleWidth = MeasureText(title, 40);
    DrawText(title, boxX + (boxWidth - titleWidth) / 2, boxY + 60, 40, DARKGRAY);

    const int instructionsWidth = MeasureText(instructions, 20);
    DrawText(instructions, boxX + (boxWidth - instructionsWidth) / 2, boxY + 200, 20, GRAY);
}

void ApplyScrabbleTheme(int baseFontSize)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize - 2);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 16);

    GuiSetStyle(STATUSBAR, TEXT_PADDING, 14);

    GuiSetStyle(BUTTON, TEXT_PADDING, 12);
    GuiSetStyle(BUTTON, COMBO_BUTTON_SPACING, 24);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x182026FF);
    GuiSetStyle(DEFAULT, LINE_COLOR, 0xC4B589FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xAACF9BFF);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x242C34FF);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x364452FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0x8F8FA0FF);

    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x2ECA7115);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x2ECA71AA);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0x2ECA71FF);

    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x2ECA7130);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0x2ECA71FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0x8FF2B8FF);

    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x242C34FF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x364452FF);
    GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0x8F8FA0FF);

    GuiSetStyle(CHECKBOX, BASE_COLOR_PRESSED, 0x2ECA71FF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_PRESSED, 0x8FF2B8FF);

    GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, 0x182026FF);
    GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, 0xC4B589FF);
}

void ApplyAltTheme(int baseFontSize)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 8);

    GuiSetStyle(BUTTON, TEXT_PADDING, 6);
    GuiSetStyle(STATUSBAR, TEXT_PADDING, 8);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x0F172AFF);
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x38BDF8FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x1E293BFF);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x334155FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x334155FF);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x38BDF8FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0x38BDF8FF);

    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x0284C7FF);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0x38BDF8FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0xFFFFFFFF);

    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, 0x1E293BFF);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, 0x334155FF);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x1E293BFF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x334155FF);
    GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(CHECKBOX, BASE_COLOR_PRESSED, 0x38BDF8FF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_PRESSED, 0x7DD3FCFF);

    GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, 0x0F172AFF);
    GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, 0x94A3B8FF);
}

// ---------------------------------------------------------------------------
// Font Management
// ---------------------------------------------------------------------------

#define APP_FONT_BASE_SIZE 64

static Font g_appFont = { 0 };
static bool g_appFontLoaded = false;

void InitAppFont(void)
{
    g_appFont = LoadFontEx(FONT, APP_FONT_BASE_SIZE, NULL, 0);

    if (g_appFont.texture.id == 0)
    {
        TraceLog(LOG_WARNING, "[UI] FONT could not be loaded. Using default font.");
        g_appFont = GetFontDefault();
        g_appFontLoaded = false;
    }
    else
    {
        SetTextureFilter(g_appFont.texture, TEXTURE_FILTER_BILINEAR);
        g_appFontLoaded = true;
        TraceLog(LOG_INFO, "[UI] FONT loaded successfully (base size: %d).", APP_FONT_BASE_SIZE);
    }

    GuiSetFont(g_appFont);
}

void UnloadAppFont(void)
{
    if (g_appFontLoaded)
    {
        UnloadFont(g_appFont);
        g_appFontLoaded = false;
    }
}

Font GetAppFont(void)
{
    return g_appFont;
}

void DrawAppText(const char *text, float x, float y, float fontSize, Color color)
{
    if (text == NULL) return;
    
    float spacing = fontSize / 16.0f;
    DrawTextEx(g_appFont, text, (Vector2){ x, y }, fontSize, spacing, color);
}

int MeasureAppText(const char *text, float fontSize)
{
    if (text == NULL) return 0;

    float spacing = fontSize / 16.0f;
    Vector2 size = MeasureTextEx(g_appFont, text, fontSize, spacing);
    return (int)ceilf(size.x);
}