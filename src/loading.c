#include "loading.h"
#include "ui.h"      // Included to access ApplyScrabbleTheme
#include "raylib.h"
#include "raygui.h"  // Needed for GuiGetStyle metrics
#include <math.h>

#define TILE_COUNT 10
static const char* LOADING_TEXT = "LOADING...";

void LoadingInit(LoadingState* lState)
{
    if (lState == NULL) return;
    
    lState->progress = 0.0f;
    lState->vfxRotation = 0.0f;
    lState->particlePulse = 0.0f;
    
    // Stagger the drop timing of the Scrabble tiles
    for (int i = 0; i < TILE_COUNT; i++)
    {
        lState->tileTimers[i] = -0.15f * i; // Negative value sets a staggered delay
        lState->tileYOffsets[i] = -200.0f;  // Start high above their target positions
    }
}

void LoadingUpdate(AppState* state, LoadingState* lState, float deltaTime)
{
    if (state == NULL || lState == NULL) return;

    // 1. Progress Bar Logic
    lState->progress += deltaTime * 0.4f; 
    if (lState->progress >= 1.0f)
    {
        lState->progress = 1.0f;
        state->currentScreen = APP_SCREEN_MAIN_MENU; 
    }

    // 2. Continuous Ambient VFX Animation
    lState->vfxRotation += deltaTime * 45.0f; 
    lState->particlePulse += deltaTime * 3.0f; 

    // 3. Tile Bounce Physics Simulation
    for (int i = 0; i < TILE_COUNT; i++)
    {
        lState->tileTimers[i] += deltaTime;
        if (lState->tileTimers[i] > 0.0f)
        {
            float time = lState->tileTimers[i] * 5.0f; 
            if (time < 3.14159f * 2.5f) 
            {
                lState->tileYOffsets[i] = -200.0f * (expf(-time * 0.5f) * cosf(time * 2.0f));
            }
            else
            {
                lState->tileYOffsets[i] = 0.0f; 
            }
        }
    }
}

void LoadingDraw(AppState* state, LoadingState* lState)
{
    if (state == NULL || lState == NULL) return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    
    int baseFontSize = screenHeight / 25;
    if (baseFontSize < 20) baseFontSize = 20;

    // Hook directly into your unified visual styling layout
    ApplyScrabbleTheme(baseFontSize);

    // Extract dynamic colors safely out of your RayGui runtime theme state configuration
    Color bgSlate      = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
    Color accentEmerald = GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED)); // 0x8FF2B8FF
    Color darkCharcoal  = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL));   // 0x242C34FF
    Color mutedText     = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));   // 0x697D8CFF

    // Use your theme's background slate color
    ClearBackground(bgSlate);

    // ----------------------------------------------------
    // VFX Level 1: Background Ambient Glow Rings
    // ----------------------------------------------------
    Vector2 center = { screenWidth / 2.0f, screenHeight / 2.0f };
    float pulseRadius = (screenHeight * 0.25f) + (sinf(lState->particlePulse) * 15.0f);
    
    DrawCircleLinesV(center, pulseRadius, (Color){ 45, 54, 72, 180 });
    DrawCircleLinesV(center, pulseRadius + 30.0f, (Color){ 45, 54, 72, 90 });
    
    DrawRectanglePro(
        (Rectangle){ center.x, center.y, 4.0f, (float)screenHeight * 0.6f },
        (Vector2){ 2.0f, (screenHeight * 0.6f) / 2.0f },
        lState->vfxRotation,
        (Color){ 60, 75, 100, 40 }
    );
    DrawRectanglePro(
        (Rectangle){ center.x, center.y, 4.0f, (float)screenHeight * 0.6f },
        (Vector2){ 2.0f, (screenHeight * 0.6f) / 2.0f },
        lState->vfxRotation + 90.0f,
        (Color){ 60, 75, 100, 40 }
    );

    // ----------------------------------------------------
    // VFX Level 2: Interactive Scrabble Letters
    // ----------------------------------------------------
    float tileSize = baseFontSize * 1.8f;
    float tileSpacing = tileSize * 0.15f;
    float totalWidth = (TILE_COUNT * tileSize) + ((TILE_COUNT - 1) * tileSpacing);
    float startX = (screenWidth - totalWidth) / 2.0f;
    float startY = (screenHeight / 2.0f) - (tileSize / 2.0f) - 40.0f;

    for (int i = 0; i < TILE_COUNT; i++)
    {
        float currentY = startY + lState->tileYOffsets[i];
        if (lState->tileTimers[i] < 0.0f) continue;

        Rectangle tileRect = { startX + i * (tileSize + tileSpacing), currentY, tileSize, tileSize };

        DrawRectangleRounded((Rectangle){ tileRect.x + 4, tileRect.y + 6, tileRect.width, tileRect.height }, 0.2f, 4, (Color){ 10, 12, 16, 180 });
        DrawRectangleRounded(tileRect, 0.2f, 4, (Color){ 238, 219, 184, 255 });
        DrawRectangleRoundedLines(tileRect, 0.2f, 4, (Color){ 204, 179, 136, 255 });

        char letterStr[2] = { LOADING_TEXT[i], '\0' };
        int textWidth = MeasureText(letterStr, baseFontSize * 1.2);
        DrawText(
            letterStr, 
            tileRect.x + (tileSize - textWidth) / 2.0f, 
            tileRect.y + (tileSize - (baseFontSize * 1.2f)) / 2.0f - 2.0f, 
            baseFontSize * 1.2f, 
            (Color){ 44, 34, 20, 255 }
        );

        const char* score = (LOADING_TEXT[i] == 'G') ? "2" : "1"; 
        DrawText(
            score, 
            tileRect.x + tileRect.width - (baseFontSize * 0.45f), 
            tileRect.y + tileRect.height - (baseFontSize * 0.55f), 
            baseFontSize * 0.4f, 
            (Color){ 110, 90, 65, 255 }
        );
    }

    // ----------------------------------------------------
    // VFX Level 3: Modern Progress Loading Bar
    // ----------------------------------------------------
    float barWidth = screenWidth * 0.5f;
    float barHeight = 10.0f;
    float barX = (screenWidth - barWidth) / 2.0f;
    float barY = (screenHeight / 2.0f) + 60.0f;

    // Track frame border background matching our themed charcoal settings
    DrawRectangleRounded((Rectangle){ barX, barY, barWidth, barHeight }, 0.5f, 4, darkCharcoal);
    
    // Filled progress slider matching our themed emerald settings
    float filledWidth = barWidth * lState->progress;
    if (filledWidth > 0.0f)
    {
        DrawRectangleRounded((Rectangle){ barX, barY, filledWidth, barHeight }, 0.5f, 4, accentEmerald);
        
        float particleRadius = 4.0f + (sinf(lState->particlePulse * 2.0f) * 2.0f);
        DrawCircle(barX + filledWidth, barY + (barHeight / 2.0f), particleRadius, (Color){ 255, 255, 255, 255 });
    }

    // Percentage Counter Text display using themed muted text colors
    int percentValue = (int)(lState->progress * 100.0f);
    const char* percentText = TextFormat("%d%% COMPLETE", percentValue);
    int pTextWidth = MeasureText(percentText, baseFontSize * 0.6f);
    DrawText(
        percentText, 
        (screenWidth - pTextWidth) / 2.0f, 
        barY + barHeight + 16.0f, 
        baseFontSize * 0.6f, 
        mutedText
    );
}