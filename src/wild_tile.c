#include "wild_tile.h"
#include "ui.h"
#include "sound.h"
#include "raygui.h"
#include <math.h>
#include <string.h>

void WildTileInit(WildTileOverlayState *state)
{
    if (!state) return;
    state->isActive = false;
    state->targetGridX = -1;
    state->targetGridY = -1;
    state->selectedLetter = 'A';
    state->hoverIndex = 0;
}

void WildTileOpen(WildTileOverlayState *state, int gridX, int gridY)
{
    if (!state) return;
    state->isActive = true;
    state->targetGridX = gridX;
    state->targetGridY = gridY;
    state->selectedLetter = 'A';
    state->hoverIndex = 0;
    PlaySoundEffect(SFX_BUTTON);
}

void WildTileCancel(WildTileOverlayState *state)
{
    if (!state) return;
    state->isActive = false;
    state->targetGridX = -1;
    state->targetGridY = -1;
    state->selectedLetter = '\0';
    PlaySoundEffect(SFX_BACK_NAV);
}

void WildTileUpdate(WildTileOverlayState *state)
{
    if (!state || !state->isActive) return;

    // Direct key presses A-Z
    for (int key = KEY_A; key <= KEY_Z; key++)
    {
        if (IsKeyPressed(key))
        {
            state->selectedLetter = 'A' + (key - KEY_A);
            state->hoverIndex = key - KEY_A;
            PlaySoundEffect(SFX_BUTTON);
            return;
        }
    }

    // Keyboard navigation (Arrow keys / TAB)
    if (IsKeyPressed(KEY_RIGHT))
    {
        state->hoverIndex = (state->hoverIndex + 1) % 26;
        state->selectedLetter = 'A' + state->hoverIndex;
        PlaySoundEffect(SFX_BUTTON);
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        state->hoverIndex = (state->hoverIndex + 25) % 26;
        state->selectedLetter = 'A' + state->hoverIndex;
        PlaySoundEffect(SFX_BUTTON);
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        state->hoverIndex = (state->hoverIndex + 7) % 26;
        state->selectedLetter = 'A' + state->hoverIndex;
        PlaySoundEffect(SFX_BUTTON);
    }
    else if (IsKeyPressed(KEY_UP))
    {
        state->hoverIndex = (state->hoverIndex + 19) % 26;
        state->selectedLetter = 'A' + state->hoverIndex;
        PlaySoundEffect(SFX_BUTTON);
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        // Selected letter stays active, overlay deactivates
        state->isActive = false;
        PlaySoundEffect(SFX_TILE_PLACE);
    }
    else if (IsKeyPressed(KEY_ESCAPE))
    {
        WildTileCancel(state);
    }
}

void WildTileDraw(WildTileOverlayState *state, int screenWidth, int screenHeight, int baseFontSize)
{
    if (!state || !state->isActive) return;

    // Dark transparent backdrop
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){10, 15, 22, 215});

    // Card dimensions
    float cardW = fminf(screenWidth * 0.55f, 560.0f);
    if (cardW < 420.0f) cardW = 420.0f;
    float cardH = 380.0f;
    float cardX = (screenWidth - cardW) / 2.0f;
    float cardY = (screenHeight - cardH) / 2.0f;
    Rectangle cardRect = {cardX, cardY, cardW, cardH};

    // Glassmorphism background & border
    DrawRectangleRounded((Rectangle){cardX + 6, cardY + 8, cardW, cardH}, 0.08f, 6, (Color){0, 0, 0, 150});
    DrawRectangleRounded(cardRect, 0.08f, 6, (Color){20, 28, 36, 255});
    DrawRectangleRoundedLinesEx(cardRect, 0.08f, 6, 2.0f, (Color){54, 72, 90, 255});

    // Title
    float titleY = cardY + 20.0f;
    const char *titleText = "CHOOSE WILDCARD LETTER";
    int titleFontSize = (int)(baseFontSize * 1.4f);
    int titleW = MeasureAppText(titleText, titleFontSize);
    DrawAppText(titleText, cardX + (cardW - titleW) / 2.0f, titleY, titleFontSize, (Color){244, 228, 198, 255});

    // Accent line
    DrawLineV((Vector2){cardX + 30.0f, titleY + titleFontSize + 10.0f},
              (Vector2){cardX + cardW - 30.0f, titleY + titleFontSize + 10.0f},
              (Color){46, 202, 113, 255});

    // Grid of 26 letter buttons (7 columns per row)
    float gridTopY = titleY + titleFontSize + 25.0f;
    float tileW = 45.0f;
    float tileH = 45.0f;
    float gapX = 10.0f;
    float gapY = 8.0f;
    int cols = 7;

    float gridTotalW = cols * tileW + (cols - 1) * gapX;
    float gridStartX = cardX + (cardW - gridTotalW) / 2.0f;

    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < 26; i++)
    {
        int row = i / cols;
        int col = i % cols;

        float bx = gridStartX + col * (tileW + gapX);
        float by = gridTopY + row * (tileH + gapY);
        Rectangle bRect = {bx, by, tileW, tileH};

        char lStr[2] = {'A' + i, '\0'};
        bool isSelected = (state->selectedLetter == ('A' + i));
        bool isHovered = CheckCollisionPointRec(mousePos, bRect);

        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            state->selectedLetter = 'A' + i;
            state->hoverIndex = i;
            PlaySoundEffect(SFX_BUTTON);
        }

        Color fillCol = isSelected ? (Color){46, 202, 113, 255} : (isHovered ? (Color){54, 72, 90, 255} : (Color){33, 43, 51, 255});
        Color lineCol = isSelected ? (Color){120, 255, 170, 255} : (Color){70, 90, 110, 255};
        Color textCol = isSelected ? (Color){10, 25, 15, 255} : (Color){220, 230, 240, 255};

        DrawRectangleRounded(bRect, 0.2f, 4, fillCol);
        DrawRectangleRoundedLinesEx(bRect, 0.2f, 4, 1.5f, lineCol);

        int lFontSz = (int)(baseFontSize * 1.2f);
        int lW = MeasureAppText(lStr, lFontSz);
        DrawAppText(lStr, bx + (tileW - lW) / 2.0f, by + (tileH - lFontSz) / 2.0f, lFontSz, textCol);
    }

    // Action buttons (Confirm / Cancel)
    float btnW = (cardW - 80.0f) / 2.0f;
    float btnH = 38.0f;
    float btnY = cardY + cardH - btnH - 45.0f;

    Rectangle confirmBtn = {cardX + 30.0f, btnY, btnW, btnH};
    Rectangle cancelBtn = {cardX + 50.0f + btnW, btnY, btnW, btnH};

    if (GuiButton(confirmBtn, "[ENTER] Confirm"))
    {
        // Selected letter stays active, overlay deactivates
        state->isActive = false;
        PlaySoundEffect(SFX_TILE_PLACE);
    }

    if (GuiButton(cancelBtn, "[ESC] Cancel"))
    {
        WildTileCancel(state);
    }

    // Nav hotkey bar
    static const HotkeyEntry wildKeys[] = {
        {"A-Z", "Select Letter"},
        {"ENTER", "Confirm"},
        {"ESC", "Cancel"}
    };
    float hkBarH = baseFontSize * 1.6f;
    float modalHkBarY = cardY + cardH - hkBarH - 8.0f;
    DrawHotkeyBar(wildKeys, 3, cardX + 30.0f, modalHkBarY, cardW - 60.0f, hkBarH, baseFontSize);
}
