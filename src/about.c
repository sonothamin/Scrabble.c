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
    DrawTriangle((Vector2){x + size / 2.0f, y + size},
                 (Vector2){x + size, y + radius * 1.2f},
                 (Vector2){x, y + radius * 1.2f}, color);
}

static float DrawCenteredText(const char *text, float centerX, float y, float fontSize, Color color)
{
    float width = MeasureAppText(text, fontSize);
    DrawAppText(text, centerX - (width / 2.0f), y, fontSize, color);
    return fontSize * 1.5f; // Returns vertical spacing step
}

static float DrawCenteredLink(const char *text, const char *url, float centerX, float y, float fontSize, Color color)
{
    float width = MeasureAppText(text, fontSize);
    Rectangle bounds = { centerX - (width / 2.0f), y, width, fontSize * 1.2f };

    // Hover effect
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, bounds);
    Color drawColor = hovered ? GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_FOCUSED)) : color;

    DrawAppText(text, bounds.x, bounds.y, fontSize, drawColor);

    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        OpenURL(url);
    }

    return fontSize * 1.5f;
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
    state->logoTexture = (Texture2D){ 0 };
    state->logoLoaded = false;
}

void AboutUpdate(AppState *appState, AboutState *aboutState)
{
    if (!appState || !aboutState) return;

    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (aboutState->logoLoaded)
        {
            UnloadTexture(aboutState->logoTexture);
            aboutState->logoTexture = (Texture2D){ 0 };
            aboutState->logoLoaded = false;
        }
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
        aboutState->activeTab = (aboutState->activeTab + 2) % 3;
        aboutState->scrollOffset = 0.0f;
    }

    if (IsKeyPressed(KEY_ONE))   aboutState->activeTab = ABOUT_TAB_OVERVIEW;
    if (IsKeyPressed(KEY_TWO))   aboutState->activeTab = ABOUT_TAB_TEAM;
    if (IsKeyPressed(KEY_THREE)) aboutState->activeTab = ABOUT_TAB_TECH;

    float mouseWheel = GetMouseWheelMove();
    if (mouseWheel != 0)
    {
        aboutState->scrollOffset += mouseWheel * 35.0f;
    }
}

void AboutDraw(AppState *appState, AboutState *aboutState)
{
    if (!appState || !aboutState)
    {
        ReportCriticalError("Invalid About/App State", "NULL About/AppState pointer encountered while drawing About.");
        return;
    }

    if (!aboutState->logoLoaded)
    {
        aboutState->logoTexture = LoadTexture("images/icon.png");
        aboutState->logoLoaded = true;
    }

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){24, 32, 38, 255});

    int baseFontSize = (screenHeight / 38 < 16) ? 16 : screenHeight / 38;
    ApplyScrabbleTheme(baseFontSize);

    const float padding = screenWidth / 25.0f;
    const float headerLineY = padding + (baseFontSize * 1.5f) + 15.0f;

    DrawAppText("SCRABBLE.C", padding, padding, baseFontSize * 1.7f, WHITE);
    DrawLineEx((Vector2){padding, headerLineY}, (Vector2){screenWidth - padding, headerLineY}, 2, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

    float sidebarY = headerLineY + 25.0f;
    float tabWidth = baseFontSize * 10.0f;
    float tabHeight = baseFontSize * 2.2f;
    float tabSpacing = 8.0f;

    const char *tabs[] = { "Overview", "Team", "Stack" };
    for (int i = 0; i < 3; i++)
    {
        if (DrawTabButton(tabs[i], (Rectangle){padding, sidebarY + (tabHeight + tabSpacing) * i, tabWidth, tabHeight}, aboutState->activeTab == i, baseFontSize * 0.85f, false))
        {
            aboutState->activeTab = i;
            aboutState->scrollOffset = 0.0f;
        }
    }

    Rectangle contentBox = {
        padding + tabWidth + 40.0f,
        sidebarY + 30.0f,
        screenWidth - (padding + tabWidth + 40.0f) - padding,
        screenHeight - (sidebarY + 30.0f) - padding - 35.0f
    };

    GuiGroupBox(contentBox, " Terminal ");

    if (aboutState->scrollOffset > 0.0f) aboutState->scrollOffset = 0.0f;

    float currentY = contentBox.y + 35.0f + aboutState->scrollOffset;
    float centerX = contentBox.x + (contentBox.width / 2.0f);

    BeginScissorMode((int)contentBox.x + 5, (int)contentBox.y + 5, (int)contentBox.width - 10, (int)contentBox.height - 10);

    switch (aboutState->activeTab)
    {
    case ABOUT_TAB_OVERVIEW:
    {
        if (aboutState->logoTexture.id > 0)
        {
            float logoSize = baseFontSize * 10.0f;
            Rectangle srcRec = { 0, 0, (float)aboutState->logoTexture.width, (float)aboutState->logoTexture.height };
            Rectangle destRec = { centerX - (logoSize / 2.0f), currentY, logoSize, logoSize };
            DrawTexturePro(aboutState->logoTexture, srcRec, destRec, (Vector2){ 0, 0 }, 0.0f, WHITE);
            currentY += logoSize + 15.0f;
        }

        currentY += DrawCenteredText("SCRABBLE.C", centerX, currentY, baseFontSize * 1.4f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        currentY += DrawCenteredText("A Modern C Implementation of the Classic Word Game", centerX, currentY, baseFontSize * 0.95f, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
        currentY += DrawCenteredText("This is what happens when a C programming project goes haywire", centerX, currentY, baseFontSize * 0.85f, WHITE);

        currentY += baseFontSize * 0.5f;
        currentY += DrawCenteredText("Website & Docs:", centerX, currentY, baseFontSize * 0.9f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_FOCUSED)));
        
        currentY += DrawCenteredLink("Find more about Scrabble.c at sonothamin.dev/Scrabble.c", "https://sonothamin.dev/Scrabble.c", centerX, currentY, baseFontSize * 0.85f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        currentY += DrawCenteredLink("And About Me Here", "https://sonothamin.dev", centerX, currentY, baseFontSize * 0.85f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        
        currentY += baseFontSize * 0.5f;
        DrawCenteredText("Version 1.0.0-RC // Closed Beta", centerX, currentY, baseFontSize * 0.75f, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_DISABLED)));
        break;
    }

    case ABOUT_TAB_TEAM:
    {
        float textX = contentBox.x + 40.0f;
        float stepGap = baseFontSize * 1.5f;
        DrawAppText("The Dev and Friends", textX, currentY, baseFontSize * 1.1f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        DrawAppText("> Lead, UI, Menu, AppShell, Docs : Sonoth Amin", textX, currentY + stepGap, baseFontSize * 0.9f, WHITE);
        DrawAppText("> Game Logic, Board : Tanvir Ahmed", textX, currentY + (stepGap * 2), baseFontSize * 0.9f, WHITE);
        break;
    }

    case ABOUT_TAB_TECH:
    {
        float textX = contentBox.x + 40.0f;
        float stepGap = baseFontSize * 1.5f;
        DrawAppText("Tech and Stack", textX, currentY, baseFontSize * 1.1f, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        DrawAppText("> Graphics Library : Raylib", textX, currentY + stepGap, baseFontSize * 0.9f, WHITE);
        DrawAppText("> UI Library : RayGUI", textX, currentY + (stepGap * 2), baseFontSize * 0.9f, WHITE);
        DrawAppText("> Written in C and somehow we are still sane. SEND HELP!!", textX, currentY + (stepGap * 3), baseFontSize * 0.9f, WHITE);
        break;
    }
    }

    EndScissorMode();

    static const HotkeyEntry aboutKeys[] = {
        { "1",   "Overview" },
        { "2",   "Team"     },
        { "3",   "Stack"    },
        { "W/S", "Navigate" },
        { "ESC", "Main Menu"},
    };
    float hkH = baseFontSize * 1.35f;
    float hkY = screenHeight - padding - hkH;
    DrawHotkeyBar(aboutKeys, 5, padding, hkY + padding * 0.30f, screenWidth - padding * 2.0f, hkH + 25, baseFontSize);

    if (IsKeyPressed(KEY_SPACE))
    {
        aboutState->EasterState = !aboutState->EasterState;
        if (aboutState->EasterState) PlaySoundEffect(SFX_ABOUT);
    }

    if (aboutState->EasterState)
    {
        const char *prefix = "Made with ";
        const char *suffix = " by Sonoth Amin";
        float textSize = baseFontSize * 0.85f;
        float totalWidth = MeasureAppText(prefix, textSize) + textSize + 10.0f + MeasureAppText(suffix, textSize);
        float eggX = screenWidth - totalWidth - padding * 1.75f;
        float eggY = hkY - textSize - 4.0f - padding * 0.25f;

        DrawAppText(prefix, eggX, eggY, textSize, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
        DrawProceduralHeart(eggX + MeasureAppText(prefix, textSize) + 5.0f, eggY + 1.0f, textSize * 0.85f, RED);
        DrawAppText(suffix, eggX + MeasureAppText(prefix, textSize) + (textSize * 0.85f) + 10.0f, eggY, textSize, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)));
    }
    else
    {
        const char *prompt = "Be a silly goose and press Space";
        float textSize = baseFontSize * 0.75f;
        float promptX = screenWidth - MeasureAppText(prompt, textSize) - padding * 1.75f;
        float promptY = hkY - textSize - 4.0f - padding * 0.25f;

        DrawAppText(prompt, promptX, promptY, textSize, ColorAlpha(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_DISABLED)), 0.3f));
    }
}