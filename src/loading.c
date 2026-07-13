#include "loading.h"
#include "raylib.h"
#include <math.h>
#include <string.h>

#define TILE_COUNT 10
static const char* LOADING_TEXT = "LOADING...";

// Real Scrabble scoring lookup helper
static const char* GetScrabbleScore(char c) 
{
    switch (c) {
        case 'D': case 'G': return "2";
        case 'B': case 'C': case 'M': case 'P': return "3";
        case 'F': case 'H': case 'V': case 'W': case 'Y': return "4";
        case 'K': return "5";
        case 'J': case 'X': return "8";
        case 'Q': case 'Z': return "10";
        default: return "1"; // L, O, A, I, N, U etc.
    }
}

void LoadingInit(LoadingState* lState)
{
    if (lState == NULL) return;
    
    lState->progress = 0.0f;
    lState->vfxRotation = 0.0f;
    lState->particlePulse = 0.0f;
    
    // Stagger the drop timing of the Scrabble tiles
    for (int i = 0; i < TILE_COUNT; i++)
    {
        lState->tileTimers[i] = -0.18f * i; // Slightly increased stagger delay for better pacing
        lState->tileYOffsets[i] = -400.0f;  // Dropped from higher up for higher impact momentum
    }
}

void LoadingUpdate(AppState* state, LoadingState* lState, float deltaTime)
{
    if (state == NULL || lState == NULL) return;

    // 1. Progress Bar Logic
    lState->progress += deltaTime * 0.25f; // Slightly slowed down to enjoy the beautiful animation sequence
    if (lState->progress >= 1.0f)
    {
        lState->progress = 1.0f;
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }

    // 2. Continuous Ambient VFX Animation
    lState->vfxRotation += deltaTime * 20.0f;  // Slowed rotation down for a hypnotic, premium ambient feel
    lState->particlePulse += deltaTime * 4.0f; 

    // 3. Physical Spring Bounce + Secondary Idle Sway Simulation
    for (int i = 0; i < TILE_COUNT; i++)
    {
        lState->tileTimers[i] += deltaTime;
        if (lState->tileTimers[i] > 0.0f)
        {
            float t = lState->tileTimers[i];
            
            // Underdamped harmonic oscillator equations (Spring constant k = 14.0, Damping c = 3.5)
            float frequency = 14.0f;
            float damping = 3.5f;
            
            // Phase 1: High energy physics drop impact
            float bounceOffset = -400.0f * expf(-damping * t) * cosf(frequency * t);
            
            // Phase 2: Microscopic procedural idle wave sway so tiles don't instantly lock dead
            // Uses a phase-offset based on tile position so they wave fluidly down the line
            float idleSway = sinf(lState->particlePulse * 1.5f + (i * 0.4f)) * 2.5f;
            
            lState->tileYOffsets[i] = bounceOffset + idleSway;
        }
    }
}

void LoadingDraw(AppState* state, LoadingState* lState)
{
    if (state == NULL || lState == NULL) return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    
    // Rich, dark cinematic slate backdrop
    ClearBackground((Color){ 24, 32, 38, 255 });

    // ----------------------------------------------------
    // VFX Level 1: Vignette & Ambient Radial Glow Line Rings
    // ----------------------------------------------------
    Vector2 center = { screenWidth / 2.0f, screenHeight / 2.0f };
    float pulseRadius = (screenHeight * 0.22f) + (sinf(lState->particlePulse) * 8.0f);
    
    DrawCircleLinesV(center, pulseRadius, (Color){ 46, 204, 113, 40 });
    DrawCircleLinesV(center, pulseRadius + 45.0f, (Color){ 46, 204, 113, 15 });
    
    // Elegant sweeping scanner radar beam
    DrawRectanglePro(
        (Rectangle){ center.x, center.y, 2.0f, (float)screenHeight * 0.7f },
        (Vector2){ 1.0f, (screenHeight * 0.7f) / 2.0f },
        lState->vfxRotation,
        (Color){ 46, 204, 113, 20 }
    );
    DrawRectanglePro(
        (Rectangle){ center.x, center.y, 2.0f, (float)screenHeight * 0.7f },
        (Vector2){ 1.0f, (screenHeight * 0.7f) / 2.0f },
        lState->vfxRotation + 90.0f,
        (Color){ 46, 204, 113, 10 }
    );

    // ----------------------------------------------------
    // VFX Level 2: Physical Scrabble Tiles
    // ----------------------------------------------------
    int baseFontSize = screenHeight / 22;
    if (baseFontSize < 24) baseFontSize = 24;

    float tileSize = baseFontSize * 1.7f;
    float tileSpacing = tileSize * 0.12f;
    float totalWidth = (TILE_COUNT * tileSize) + ((TILE_COUNT - 1) * tileSpacing);
    float startX = (screenWidth - totalWidth) / 2.0f;
    float startY = (screenHeight / 2.0f) - (tileSize / 2.0f) - 60.0f;

    for (int i = 0; i < TILE_COUNT; i++)
    {
        if (lState->tileTimers[i] < 0.0f) continue;

        float currentY = startY + lState->tileYOffsets[i];
        float currentX = startX + i * (tileSize + tileSpacing);
        Rectangle tileRect = { currentX, currentY, tileSize, tileSize };

        // 1. Dynamic Drop Shadow (Shadow distance scales realistically with the height offset)
        float heightFactor = fmaxf(0.0f, -lState->tileYOffsets[i] / 400.0f);
        float shadowOffset = 6.0f + (heightFactor * 15.0f);
        float shadowAlpha = 180 - (int)(heightFactor * 140);
        DrawRectangleRounded(
            (Rectangle){ tileRect.x + shadowOffset, tileRect.y + shadowOffset, tileRect.width, tileRect.height }, 
            0.18f, 4, (Color){ 10, 14, 18, (unsigned char)shadowAlpha }
        );

        // 2. Tile Body (Birch Ivory Base)
        DrawRectangleRounded(tileRect, 0.18f, 4, (Color){ 244, 228, 198, 255 });
        
        // Premium interior core beveling lines
        Rectangle innerBevel = { tileRect.x + 3, tileRect.y + 3, tileRect.width - 6, tileRect.height - 6 };
        DrawRectangleRoundedLines(innerBevel, 0.15f, 4, (Color){ 255, 248, 230, 255 });
        DrawRectangleRoundedLines(tileRect, 0.18f, 4, (Color){ 194, 169, 126, 255 });

        // 3. Central Character Printing
        char letterStr[2] = { LOADING_TEXT[i], '\0' };
        int textWidth = MeasureText(letterStr, baseFontSize * 1.1f);
        DrawText(
            letterStr, 
            tileRect.x + (tileSize - textWidth) / 2.0f, 
            tileRect.y + (tileSize - (baseFontSize * 1.1f)) / 2.0f - 4.0f, 
            baseFontSize * 1.1f, 
            (Color){ 38, 28, 16, 255 }
        );

        // 4. Correct Scrabble Score Display
        const char* score = GetScrabbleScore(LOADING_TEXT[i]);
        DrawText(
            score, 
            tileRect.x + tileRect.width - (baseFontSize * 0.45f), 
            tileRect.y + tileRect.height - (baseFontSize * 0.55f), 
            baseFontSize * 0.35f, 
            (Color){ 120, 95, 68, 255 }
        );
    }

    // ----------------------------------------------------
    // VFX Level 3: Glow Progress Tracking Bar
    // ----------------------------------------------------
    float barWidth = screenWidth * 0.45f;
    float barHeight = 8.0f;
    float barX = (screenWidth - barWidth) / 2.0f;
    float barY = (screenHeight / 2.0f) + 70.0f;

    // Track Background
    DrawRectangleRounded((Rectangle){ barX, barY, barWidth, barHeight }, 0.5f, 4, (Color){ 36, 44, 52, 255 });
    
    float filledWidth = barWidth * lState->progress;
    if (filledWidth > 0.0f)
    {
        // Neo-Green progress fill
        DrawRectangleRounded((Rectangle){ barX, barY, filledWidth, barHeight }, 0.5f, 4, (Color){ 46, 204, 113, 255 });
        
        // Dynamic Pulsing Head Emitter
        float particleRadius = 4.0f + (sinf(lState->particlePulse * 3.0f) * 1.5f);
        Color glowColor = (Color){ 143, 242, 184, 255 };
        
        DrawCircle(barX + filledWidth, barY + (barHeight / 2.0f), particleRadius + 4.0f, (Color){ 46, 204, 113, 80 });
        DrawCircle(barX + filledWidth, barY + (barHeight / 2.0f), particleRadius, glowColor);
    }

    // Centered percentage status text
    int percentValue = (int)(lState->progress * 100.0f);
    const char* percentText = TextFormat("%d%% LOADED", percentValue);
    int pTextWidth = MeasureText(percentText, baseFontSize * 0.5f);
    DrawText(
        percentText, 
        (screenWidth - pTextWidth) / 2.0f, 
        barY + barHeight + 20.0f, 
        baseFontSize * 0.5f, 
        (Color){ 105, 125, 140, 255 }
    );
}