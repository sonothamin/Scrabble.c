#include "loading.h"
#include "raylib.h"
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

    // 1. Progress Bar Logic (Simulating assets loading dynamically)
    // In a real application, replace this with your actual asset loader progress
    lState->progress += deltaTime * 0.4f; 
    if (lState->progress >= 1.0f)
    {
        lState->progress = 1.0f;
        state->currentScreen = APP_SCREEN_MAIN_MENU; // Switch screen automatically when complete
    }

    // 2. Continuous Ambient VFX Animation
    lState->vfxRotation += deltaTime * 45.0f; // Spin ambient background elements
    lState->particlePulse += deltaTime * 3.0f; // Rapid pulsing tracking variable

    // 3. Tile Bounce Physics Simulation
    for (int i = 0; i < TILE_COUNT; i++)
    {
        lState->tileTimers[i] += deltaTime;
        if (lState->tileTimers[i] > 0.0f)
        {
            // Bouncing math using a decaying sine wave
            float time = lState->tileTimers[i] * 5.0f; // Control speed of falling
            if (time < 3.14159f * 2.5f) // Stop calculating bounce after it settles down
            {
                // Decaying spring math simulation
                lState->tileYOffsets[i] = -200.0f * (expf(-time * 0.5f) * cosf(time * 2.0f));
            }
            else
            {
                lState->tileYOffsets[i] = 0.0f; // Flat resting baseline position
            }
        }
    }
}

void LoadingDraw(AppState* state, LoadingState* lState)
{
    if (state == NULL || lState == NULL) return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    
    // Dark modern game background slate
    ClearBackground((Color){ 54, 80, 76, 255 });

    // ----------------------------------------------------
    // VFX Level 1: Background Ambient Glow Rings
    // ----------------------------------------------------
    Vector2 center = { screenWidth / 2.0f, screenHeight / 2.0f };
    float pulseRadius = (screenHeight * 0.25f) + (sinf(lState->particlePulse) * 15.0f);
    
    DrawCircleLinesV(center, pulseRadius, (Color){ 45, 54, 72, 180 });
    DrawCircleLinesV(center, pulseRadius + 30.0f, (Color){ 45, 54, 72, 90 });
    
    // Spinning crosshairs to indicate active background loading tasks
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
    int baseFontSize = screenHeight / 25;
    if (baseFontSize < 20) baseFontSize = 20;

    float tileSize = baseFontSize * 1.8f;
    float tileSpacing = tileSize * 0.15f;
    float totalWidth = (TILE_COUNT * tileSize) + ((TILE_COUNT - 1) * tileSpacing);
    float startX = (screenWidth - totalWidth) / 2.0f;
    float startY = (screenHeight / 2.0f) - (tileSize / 2.0f) - 40.0f;

    for (int i = 0; i < TILE_COUNT; i++)
    {
        float currentY = startY + lState->tileYOffsets[i];
        
        // Skip drawing entirely if it hasn't reached its internal timer activation entry point
        if (lState->tileTimers[i] < 0.0f) continue;

        Rectangle tileRect = { startX + i * (tileSize + tileSpacing), currentY, tileSize, tileSize };

        // Draw wood-style tile drop shadow
        DrawRectangleRounded((Rectangle){ tileRect.x + 4, tileRect.y + 6, tileRect.width, tileRect.height }, 0.2f, 4, (Color){ 10, 12, 16, 180 });

        // Draw Scrabble Tile Base (Classic Birch/Beige Color Scheme)
        DrawRectangleRounded(tileRect, 0.2f, 4, (Color){ 238, 219, 184, 255 });
        DrawRectangleRoundedLines(tileRect, 0.2f, 4, (Color){ 204, 179, 136, 255 });

        // Draw Central Letter Character
        char letterStr[2] = { LOADING_TEXT[i], '\0' };
        int textWidth = MeasureText(letterStr, baseFontSize * 1.2);
        DrawText(
            letterStr, 
            tileRect.x + (tileSize - textWidth) / 2.0f, 
            tileRect.y + (tileSize - (baseFontSize * 1.2f)) / 2.0f - 2.0f, 
            baseFontSize * 1.2f, 
            (Color){ 44, 34, 20, 255 }
        );

        // Draw Scrabble Style Letter Point Value Subscript (e.g., L = 1, O = 1)
        // Hardcoded generic values for aesthetic balance
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

    // Track frame border background
    DrawRectangleRounded((Rectangle){ barX, barY, barWidth, barHeight }, 0.5f, 4, (Color){ 45, 54, 72, 255 });
    
    // Filled progress slider (Colored accent green)
    float filledWidth = barWidth * lState->progress;
    if (filledWidth > 0.0f)
    {
        DrawRectangleRounded((Rectangle){ barX, barY, filledWidth, barHeight }, 0.5f, 4, (Color){ 46, 204, 113, 255 });
        
        // Glowing end particle node tracking active load percentage front
        float particleRadius = 4.0f + (sinf(lState->particlePulse * 2.0f) * 2.0f);
        DrawCircle(barX + filledWidth, barY + (barHeight / 2.0f), particleRadius, (Color){ 120, 240, 170, 255 });
    }

    // Percentage Counter Text display
    int percentValue = (int)(lState->progress * 100.0f);
    const char* percentText = TextFormat("%d%% COMPLETE", percentValue);
    int pTextWidth = MeasureText(percentText, baseFontSize * 0.6f);
    DrawText(
        percentText, 
        (screenWidth - pTextWidth) / 2.0f, 
        barY + barHeight + 16.0f, 
        baseFontSize * 0.6f, 
        (Color){ 140, 155, 175, 255 }
    );
}