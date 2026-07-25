#include "ui.h"
#include <math.h>

#define RAYGUI_IMPLEMENTATION
#define FONT "PTSerif-Regular.ttf"
#include "raygui.h"

#define APP_FONT_BASE_SIZE 64
static Font g_appFont = { 0 };
static bool g_appFontLoaded = false;

static Color GetThemeColor(int control, int property) { return GetColor(GuiGetStyle(control, property)); }

void InitAppFont(void)
{
    g_appFont = LoadFontEx(FONT, APP_FONT_BASE_SIZE, NULL, 0);
    g_appFontLoaded = (g_appFont.texture.id != 0);
    if (!g_appFontLoaded) g_appFont = GetFontDefault();
    else SetTextureFilter(g_appFont.texture, TEXTURE_FILTER_BILINEAR);
    GuiSetFont(g_appFont);
}

void UnloadAppFont(void) { if (g_appFontLoaded) { UnloadFont(g_appFont); g_appFontLoaded = false; } }
Font GetAppFont(void) { return g_appFont; }

void DrawAppText(const char *text, float x, float y, float fontSize, Color color)
{
    if (text && fontSize > 0.0f) DrawTextEx(g_appFont, text, (Vector2){ x, y }, fontSize, fontSize / 16.0f, color);
}

int MeasureAppText(const char *text, float fontSize)
{
    if (!text || fontSize <= 0.0f) return 0;
    return (int)ceilf(MeasureTextEx(g_appFont, text, fontSize, fontSize / 16.0f).x);
}

void DrawScreenBox(const char *title, const char *instructions)
{
    DrawRectangle(100, 100, 600, 400, Fade(LIGHTGRAY, 0.55f));
    DrawRectangleLines(100, 100, 600, 400, GRAY);
    if (title) DrawText(title, 100 + (600 - MeasureText(title, 40)) / 2, 160, 40, DARKGRAY);
    if (instructions) DrawText(instructions, 100 + (600 - MeasureText(instructions, 20)) / 2, 300, 20, GRAY);
}

void ApplyScrabbleTheme(int baseFontSize)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize - 2);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 16);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x182026FF);
    GuiSetStyle(DEFAULT, LINE_COLOR, 0xC4B589FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xAACF9BFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0x3F4D59FF);

    GuiSetStyle(STATUSBAR, TEXT_PADDING, 14);
    GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, 0x182026FF);
    GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, 0xC4B589FF);

    GuiSetStyle(BUTTON, TEXT_PADDING, 12);
    GuiSetStyle(BUTTON, COMBO_BUTTON_SPACING, 24);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x242C34FF);    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x364452FF);    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x2ECA7115);   GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x2ECA71AA);   GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0x2ECA71FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x2ECA7130);   GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0x2ECA71FF);   GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0x8FF2B8FF);
    GuiSetStyle(BUTTON, BASE_COLOR_DISABLED, 0x12171BFF);  GuiSetStyle(BUTTON, BORDER_COLOR_DISABLED, 0x1F272EFF);  GuiSetStyle(BUTTON, TEXT_COLOR_DISABLED, 0x3F4D59FF);

    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x242C34FF);   GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x364452FF);   GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    GuiSetStyle(CHECKBOX, BASE_COLOR_PRESSED, 0x2ECA71FF);  GuiSetStyle(CHECKBOX, BORDER_COLOR_PRESSED, 0x8FF2B8FF);
    GuiSetStyle(CHECKBOX, BASE_COLOR_DISABLED, 0x12171BFF); GuiSetStyle(CHECKBOX, BORDER_COLOR_DISABLED, 0x1F272EFF); GuiSetStyle(CHECKBOX, TEXT_COLOR_DISABLED, 0x3F4D59FF);
}

void ApplyAltTheme(int baseFontSize)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 8);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x0F172AFF);
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x38BDF8FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0x475569FF);

    GuiSetStyle(BUTTON, TEXT_PADDING, 6);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x1E293BFF);    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x334155FF);    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x334155FF);   GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x38BDF8FF);   GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0x38BDF8FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x0284C7FF);   GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0x38BDF8FF);   GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0xFFFFFFFF);
    GuiSetStyle(BUTTON, BASE_COLOR_DISABLED, 0x090D16FF);  GuiSetStyle(BUTTON, BORDER_COLOR_DISABLED, 0x1E293BFF);  GuiSetStyle(BUTTON, TEXT_COLOR_DISABLED, 0x475569FF);

    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, 0x1E293BFF);   GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, 0x334155FF);   GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(TEXTBOX, BASE_COLOR_DISABLED, 0x090D16FF);  GuiSetStyle(TEXTBOX, BORDER_COLOR_DISABLED, 0x1E293BFF);  GuiSetStyle(TEXTBOX, TEXT_COLOR_DISABLED, 0x475569FF);

    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x1E293BFF);  GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x334155FF);  GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(CHECKBOX, BASE_COLOR_PRESSED, 0x38BDF8FF); GuiSetStyle(CHECKBOX, BORDER_COLOR_PRESSED, 0x7DD3FCFF);
    GuiSetStyle(CHECKBOX, BASE_COLOR_DISABLED, 0x090D16FF);GuiSetStyle(CHECKBOX, BORDER_COLOR_DISABLED, 0x1E293BFF); GuiSetStyle(CHECKBOX, TEXT_COLOR_DISABLED, 0x475569FF);

    GuiSetStyle(STATUSBAR, TEXT_PADDING, 8);
    GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, 0x0F172AFF);
    GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, 0x94A3B8FF);
}


bool DrawTabButton(const char *text, Rectangle bounds, bool isActive, int fontSize, bool isModalActive)
{
    bool disabled = (GuiGetState() == STATE_DISABLED);
    bool hovered = !disabled && !isModalActive && CheckCollisionPointRec(GetMousePosition(), bounds);

    int bgProp = disabled ? BASE_COLOR_DISABLED : (isActive ? BASE_COLOR_PRESSED : (hovered ? BASE_COLOR_FOCUSED : DEFAULT));
    int txtProp = disabled ? TEXT_COLOR_DISABLED : (isActive ? TEXT_COLOR_PRESSED : (hovered ? TEXT_COLOR_FOCUSED : DEFAULT));

    DrawRectangleRec(bounds, bgProp == DEFAULT ? GetThemeColor(DEFAULT, BACKGROUND_COLOR) : GetThemeColor(BUTTON, bgProp));
    DrawRectangleLinesEx(bounds, 1.5f, GetThemeColor(DEFAULT, LINE_COLOR));
    DrawAppText(text, bounds.x + 15.0f, bounds.y + (bounds.height - fontSize) / 2.0f, (float)fontSize, 
                txtProp == DEFAULT ? GetThemeColor(DEFAULT, TEXT_COLOR_NORMAL) : GetThemeColor(BUTTON, txtProp));

    return (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}

void DrawHotkeyBar(const HotkeyEntry *entries, int count, float x, float y, float width, float height, float fontSize)
{
    if (!entries || count <= 0) return;
    if (count > 32) count = 32;

    DrawRectangleRounded((Rectangle){ x, y, width, height }, 0.35f, 6, (Color){ 12, 17, 22, 220 });
    DrawRectangleRoundedLines((Rectangle){ x, y, width, height }, 0.35f, 6, (Color){ 70, 85, 100, 180 });

    float keyFontSize = fontSize * 0.72f, keyPadX = keyFontSize * 0.55f, keyPadY = keyFontSize * 0.22f;
    float gapKeyLabel = keyFontSize * 0.50f, gapBetween = keyFontSize * 1.10f, totalWidth = 0.0f;

    for (int i = 0; i < count; i++) {
        totalWidth += MeasureAppText(entries[i].key, keyFontSize) + keyPadX * 2.0f + gapKeyLabel + MeasureAppText(entries[i].label, keyFontSize);
        if (i < count - 1) totalWidth += gapBetween;
    }

    float curX = x + (width - totalWidth) / 2.0f, centerY = y + height / 2.0f;

    for (int i = 0; i < count; i++) {
        float kw = MeasureAppText(entries[i].key, keyFontSize) + keyPadX * 2.0f, kh = keyFontSize + keyPadY * 2.0f;
        Rectangle cap = { curX, centerY - kh / 2.0f, kw, kh };

        DrawRectangleRounded((Rectangle){ cap.x + 1.5f, cap.y + 2.5f, cap.width, cap.height }, 0.30f, 4, (Color){ 8, 12, 18, 200 });
        DrawRectangleRounded(cap, 0.30f, 4, (Color){ 38, 48, 60, 255 });
        DrawRectangleRounded((Rectangle){ cap.x, cap.y, cap.width, cap.height * 0.30f }, 0.30f, 4, (Color){ 72, 92, 112, 200 });
        DrawRectangleRoundedLines(cap, 0.30f, 4, (Color){ 55, 75, 95, 255 });

        DrawAppText(entries[i].key, cap.x + (kw - MeasureAppText(entries[i].key, keyFontSize)) / 2.0f, cap.y + keyPadY, keyFontSize, (Color){ 210, 230, 255, 255 });
        curX += kw + gapKeyLabel;

        DrawAppText(entries[i].label, curX, centerY - keyFontSize / 2.0f, keyFontSize, GetThemeColor(DEFAULT, TEXT_COLOR_NORMAL));
        curX += MeasureAppText(entries[i].label, keyFontSize);

        if (i < count - 1) {
            DrawLineEx((Vector2){ curX + gapBetween / 2.0f, centerY - height * 0.225f }, (Vector2){ curX + gapBetween / 2.0f, centerY + height * 0.225f }, 1.0f, (Color){ 60, 80, 100, 140 });
            curX += gapBetween;
        }
    }
}

bool DrawLegendButton(Texture2D image, const char *text, Rectangle bounds, int fontSize, bool isModalActive)
{
    bool disabled = (GuiGetState() == STATE_DISABLED);
    bool hovered = !disabled && !isModalActive && CheckCollisionPointRec(GetMousePosition(), bounds);
    bool pressed = hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    int p = disabled ? BASE_COLOR_DISABLED : (pressed ? BASE_COLOR_PRESSED : (hovered ? BASE_COLOR_FOCUSED : BASE_COLOR_NORMAL));
    int b = disabled ? BORDER_COLOR_DISABLED : (pressed ? BORDER_COLOR_PRESSED : (hovered ? BORDER_COLOR_FOCUSED : BORDER_COLOR_NORMAL));
    int t = disabled ? TEXT_COLOR_DISABLED : (pressed ? TEXT_COLOR_PRESSED : (hovered ? TEXT_COLOR_FOCUSED : TEXT_COLOR_NORMAL));

    DrawRectangleRec(bounds, GetThemeColor(BUTTON, p));
    DrawRectangleLinesEx(bounds, 1.5f, GetThemeColor(BUTTON, b));

    float overflow = bounds.height * 0.18f, size = (bounds.height + overflow * 2.0f) * (hovered ? 1.15f : 1.0f);
    float legendX = bounds.x + (bounds.height * 0.25f) - ((size - (bounds.height + overflow * 2.0f)) / 2.0f);

    if (image.id > 0)
        DrawTexturePro(image, (Rectangle){ 0, 0, (float)image.width, (float)image.height }, 
                       (Rectangle){ legendX, bounds.y + bounds.height / 2.0f - size / 2.0f, size, size }, 
                       (Vector2){ 0, 0 }, 0.0f, disabled ? GRAY : WHITE);

    if (text) DrawAppText(text, bounds.x + bounds.height * 1.25f + overflow * 2.0f + 18.0f, 
                          bounds.y + (bounds.height - fontSize) / 2.0f, (float)fontSize, GetThemeColor(BUTTON, t));

    return (hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
}