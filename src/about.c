#include "about.h"
#include "ui.h"
#include "raylib.h"
#include "raygui.h"
#include "error_service.h"
#include "sound.h"

static void DrawProceduralHeart(float x, float y, float size, Color color)
{
    float radius = size / 4.0f;

    DrawCircleV((Vector2){x + radius, y + radius}, radius, color);
    DrawCircleV((Vector2){x + radius * 3.0f, y + radius}, radius, color);

    Vector2 v1 = {x + size / 2.0f, y + size};
    Vector2 v2 = {x + size, y + radius * 1.2f};
    Vector2 v3 = {x, y + radius * 1.2f};

    DrawTriangle(v1, v2, v3, color);
}

void AboutInit(AboutState *state)
{
    if (!state)
    {
        ReportCriticalError("Invalid App State", "NULL AboutState pointer encountered while initializing About.");
        return;
    }
    state->activeTab = ABOUT_TAB_OVERVIEW;
    state->scrollOffset = 0.0f;
    state->EasterState = 0;
}

void AboutUpdate(AppState *appState, AboutState *aboutState)
{
    if (!appState || !aboutState)
        return;

    if (IsKeyPressed(KEY_ESCAPE))
    {
        appState->currentScreen = APP_SCREEN_MAIN_MENU;
        PlaySoundEffect(SFX_BACK_NAV);
        return;
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        aboutState->activeTab = (aboutState->activeTab + 1) % 3;
        aboutState->scrollOffset = 0.0f;
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        aboutState->activeTab = (aboutState->activeTab - 1 + 3) % 3;
        aboutState->scrollOffset = 0.0f;
    }

    if (IsKeyPressed(KEY_ONE))
        aboutState->activeTab = ABOUT_TAB_OVERVIEW;
    if (IsKeyPressed(KEY_TWO))
        aboutState->activeTab = ABOUT_TAB_TEAM;
    if (IsKeyPressed(KEY_THREE))
        aboutState->activeTab = ABOUT_TAB_TECH;

    float mouseWheel = GetMouseWheelMove();
    if (mouseWheel != 0)
    {
        aboutState->scrollOffset += mouseWheel * 35.0f;
    }
}

static bool DrawTabButton(const char *text, Rectangle bounds, bool isActive, int fontSize)
{
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);

    Color bg = isActive ? GetColor(GuiGetStyle(BUTTON, BASE_COLOR_PRESSED)) : (isHovered ? GetColor(GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED)) : GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    Color textCol = isActive ? GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)) : (isHovered ? GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_FOCUSED)) : GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    DrawRectangleRec(bounds, bg);
    DrawRectangleLinesEx(bounds, 1.5f, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
    DrawAppText(text, bounds.x + 15.0f, bounds.y + (bounds.height - fontSize) / 2.0f, fontSize, textCol);

    return (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}

void AboutDraw(AppState *appState, AboutState *aboutState)
{
    if (!appState || !aboutState)
    {
        ReportCriticalError("Invalid About/App State", "NULL About/AppState pointer encountered while drawing About.");
        return;
    }

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){24, 32, 38, 255});

    int baseFontSize = screenHeight / 38;
    if (baseFontSize < 16)
        baseFontSize = 16;
    ApplyScrabbleTheme(baseFontSize);

    const float padding = screenWidth / 25.0f;
    const float headerLineY = padding + (baseFontSize * 1.5f) + 15.0f;

    DrawAppText("SCRABBLE.C", padding, padding, baseFontSize * 1.7f, WHITE);
    DrawLineEx((Vector2){padding, headerLineY}, (Vector2){screenWidth - padding, headerLineY}, 2, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

    float sidebarX = padding;
    float sidebarY = headerLineY + 25.0f;
    float tabWidth = baseFontSize * 10.0f;
    float tabHeight = baseFontSize * 2.2f;
    float tabSpacing = 8.0f;

    if (DrawTabButton("[1] Overview", (Rectangle){sidebarX, sidebarY, tabWidth, tabHeight}, aboutState->activeTab == ABOUT_TAB_OVERVIEW, baseFontSize * 0.85f))
    {
        aboutState->activeTab = ABOUT_TAB_OVERVIEW;
        aboutState->scrollOffset = 0.0f;
    }
    if (DrawTabButton("[2] Team", (Rectangle){sidebarX, sidebarY + tabHeight + tabSpacing, tabWidth, tabHeight}, aboutState->activeTab == ABOUT_TAB_TEAM, baseFontSize * 0.85f))
    {
        aboutState->activeTab = ABOUT_TAB_TEAM;
        aboutState->scrollOffset = 0.0f;
    }
    if (DrawTabButton("[3] Stack", (Rectangle){sidebarX, sidebarY + (tabHeight + tabSpacing) * 2.0f, tabWidth, tabHeight}, aboutState->activeTab == ABOUT_TAB_TECH, baseFontSize * 0.85f))
    {
        aboutState->activeTab = ABOUT_TAB_TECH;
        aboutState->scrollOffset = 0.0f;
    }

    float contentLeft = sidebarX + tabWidth + 40.0f;
    float contentTop = sidebarY + 30.0f;
    float contentWidth = screenWidth - contentLeft - padding;
    float contentHeight = screenHeight - contentTop - padding - 35.0f;

    Rectangle contentBox = {contentLeft, contentTop, contentWidth, contentHeight};
    GuiGroupBox(contentBox, " Terminal ");

    float contentX = contentBox.x + 40.0f;
    float stepGap = baseFontSize * 1.6f;
    int totalLines = 0;

    switch (aboutState->activeTab)
    {
    case ABOUT_TAB_OVERVIEW:
        totalLines = 3;
        break;
    case ABOUT_TAB_TEAM:
        totalLines = 3;
        break;
    case ABOUT_TAB_TECH:
        totalLines = 4;
        break;
    }

    float innerHeight = contentBox.height - 70.0f;
    float totalContentHeight = totalLines * stepGap;
    float minScrollOffset = 0.0f;

    if (totalContentHeight > innerHeight)
    {
        minScrollOffset = innerHeight - totalContentHeight;
    }

    if (aboutState->scrollOffset > 0.0f)
        aboutState->scrollOffset = 0.0f;
    if (aboutState->scrollOffset < minScrollOffset)
        aboutState->scrollOffset = minScrollOffset;

    float contentY = contentBox.y + 45.0f + aboutState->scrollOffset;

    // Inner scissor boundary box masking context
    BeginScissorMode((int)contentBox.x + 5, (int)contentBox.y + 5, (int)contentBox.width - 10, (int)contentBox.height - 10);

    switch (aboutState->activeTab)
    {
    case ABOUT_TAB_OVERVIEW:
        DrawAppText("This went out of hand quick", contentX, contentY, baseFontSize * 1.1f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        DrawAppText("First it was just a simple game of Scrabble.", contentX, contentY + stepGap, baseFontSize * 0.9f, WHITE);
        DrawAppText("Then we added animation, sound effects, UI niceties and even Network Play! May God have mercy on us :)", contentX, contentY + (stepGap * 2), baseFontSize * 0.9f, WHITE);
        DrawAppText("Version 1.0.0-RC // Still in closed Beta. But we will get there!", contentX, contentY + (stepGap * 3), baseFontSize * 0.76f, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_DISABLED)));
        break;

    case ABOUT_TAB_TEAM:
        DrawAppText("The Dev and Friends", contentX, contentY, baseFontSize * 1.1f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        DrawAppText("> Lead, UI, Menu, AppShell, Docs : Sonoth Amin", contentX, contentY + stepGap, baseFontSize * 0.9f, WHITE);
        DrawAppText("> Game Logic, Board : Tanvir Ahmed", contentX, contentY + (stepGap * 2), baseFontSize * 0.9f, WHITE);
        break;

    case ABOUT_TAB_TECH:
        DrawAppText("Tech and Stack", contentX, contentY, baseFontSize * 1.1f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        DrawAppText("> Graphics Library : Raylib", contentX, contentY + stepGap, baseFontSize * 0.9f, WHITE);
        DrawAppText("> UI Library : RayGUI", contentX, contentY + (stepGap * 2), baseFontSize * 0.9f, WHITE);
        DrawAppText("> Written in C and somehow we are still sane. SEND HELP!!", contentX, contentY + (stepGap * 3), baseFontSize * 0.9f, WHITE);
        break;
    }

    EndScissorMode();

    float footerY = screenHeight - padding;
    DrawAppText("Navigate: [W/S] or Keys [1-3]  |  [ESC] Main Menu", padding, footerY, baseFontSize * 0.75f, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_DISABLED)));

    if(IsKeyPressed(KEY_SPACE))
    {
        aboutState->EasterState = (aboutState->EasterState ? 0 : 1);
        if(aboutState->EasterState) PlaySoundEffect(SFX_ABOUT);
    }

    if (aboutState->EasterState)
    {
        const char *prefix = "Made with ";
        const char *suffix = " by Sonoth Amin";
        int textFontSize = baseFontSize * 0.85f;
        float totalWidth = MeasureAppText(prefix, textFontSize) + textFontSize + 10.0f + MeasureAppText(suffix, textFontSize);
        float eggX = screenWidth - totalWidth - padding;

        DrawAppText(prefix, eggX, footerY - 2.0f, textFontSize, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        DrawProceduralHeart(eggX + MeasureAppText(prefix, textFontSize) + 5.0f, (footerY - 2.0f) + 1.0f, textFontSize * 0.85f, RED);
        DrawAppText(suffix, eggX + MeasureAppText(prefix, textFontSize) + (textFontSize * 0.85f) + 10.0f, footerY - 2.0f, textFontSize, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
    }
    else
    {
        const char *prompt = "[Space] Try?";
        DrawAppText(prompt, screenWidth - MeasureAppText(prompt, baseFontSize * 0.75f) - padding, footerY, baseFontSize * 0.75f, ColorAlpha(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_DISABLED)), 0.3f));
    }
}