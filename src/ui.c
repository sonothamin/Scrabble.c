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

bool DrawTabButton(const char *text, Rectangle bounds, bool isActive, int fontSize, bool isModalActive)
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

void DrawHotkeyBar(const HotkeyEntry *entries, int count,
                   float x, float y, float width, float height,
                   float fontSize)
{
    if (!entries || count <= 0) return;

    // ---- Background container ----
    const Color bgColor    = { 12, 17, 22, 220 };
    const Color borderColor= { 70, 85, 100, 180 };

    DrawRectangleRounded((Rectangle){ x, y, width, height }, 0.35f, 6, bgColor);
    DrawRectangleRoundedLines((Rectangle){ x, y, width, height }, 0.35f, 6, borderColor);

    // ---- Sizing & layout ----
    float keyFontSize   = fontSize * 0.72f;
    float labelFontSize = fontSize * 0.72f;

    // Measure all items so we can pack them tightly with equal gaps
    float itemWidths[32]; // max 32 hotkeys
    if (count > 32) count = 32;

    float keyPadX  = keyFontSize * 0.55f;  // horizontal padding inside keycap
    float keyPadY  = keyFontSize * 0.22f;  // vertical padding inside keycap
    float gapKeyLabel = keyFontSize * 0.50f; // space between keycap and its label
    float gapBetween  = keyFontSize * 1.10f; // space between entries

    float totalContentWidth = 0.0f;
    for (int i = 0; i < count; i++)
    {
        float kw = MeasureAppText(entries[i].key,   keyFontSize)   + keyPadX * 2.0f;
        float lw = MeasureAppText(entries[i].label, labelFontSize);
        itemWidths[i] = kw + gapKeyLabel + lw;
        totalContentWidth += itemWidths[i];
        if (i < count - 1) totalContentWidth += gapBetween;
    }

    // Center the content block inside the bar
    float startX = x + (width - totalContentWidth) / 2.0f;
    float centerY = y + height / 2.0f;

    // ---- Colors (adapted to active theme) ----
    // Keycap face
    const Color capFace  = { 38,  48,  60, 255 };
    const Color capTop   = { 72,  92, 112, 200 }; // highlight bevel (top-left)
    const Color capBot   = {  8,  12,  18, 200 }; // shadow bevel   (bottom-right)
    const Color capBorder= { 55,  75,  95, 255 };
    const Color keyText  = {210, 230, 255, 255 };
    const Color sepColor = { 60,  80, 100, 140 };

    float capRound = 0.30f;

    float curX = startX;
    for (int i = 0; i < count; i++)
    {
        // --- Keycap geometry ---
        float kw       = MeasureAppText(entries[i].key, keyFontSize) + keyPadX * 2.0f;
        float kh       = keyFontSize + keyPadY * 2.0f;
        float capY     = centerY - kh / 2.0f;

        Rectangle capRect = { curX, capY, kw, kh };

        // Shadow offset for the "key press depth" look
        Rectangle shadowRect = { capRect.x + 1.5f, capRect.y + 2.5f, capRect.width, capRect.height };
        DrawRectangleRounded(shadowRect, capRound, 4, capBot);

        // Main face
        DrawRectangleRounded(capRect, capRound, 4, capFace);

        // Top-left highlight sliver (simulated bevel)
        DrawRectangleRounded((Rectangle){ capRect.x, capRect.y, capRect.width, capRect.height * 0.30f }, capRound, 4, capTop);

        // Border
        DrawRectangleRoundedLines(capRect, capRound, 4, capBorder);

        // Key letter(s) – horizontally centered inside cap
        float keyTextW = MeasureAppText(entries[i].key, keyFontSize);
        float keyTextX = capRect.x + (kw - keyTextW) / 2.0f;
        float keyTextY = capY + (kh - keyFontSize) / 2.0f;
        DrawAppText(entries[i].key, keyTextX, keyTextY, keyFontSize, keyText);

        curX += kw + gapKeyLabel;

        // --- Label ---
        float labelY = centerY - labelFontSize / 2.0f;
        Color labelColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
        DrawAppText(entries[i].label, curX, labelY, labelFontSize, labelColor);

        float lw = MeasureAppText(entries[i].label, labelFontSize);
        curX += lw;

        // --- Thin separator between entries (not after last) ---
        if (i < count - 1)
        {
            float sepX = curX + gapBetween / 2.0f;
            float sepH = height * 0.45f;
            float sepY = centerY - sepH / 2.0f;
            DrawLineEx((Vector2){ sepX, sepY }, (Vector2){ sepX, sepY + sepH }, 1.0f, sepColor);
            curX += gapBetween;
        }
    }
}