#include "game_over.h"
#include "ui.h"
#include "raylib.h"
#include "raygui.h"
#include "sound.h"
#include <math.h>

void InitGameOverState(GameOverState *gameOver)
{
    if (!gameOver) return;
    gameOver->animTime = 0.0f;
}

void UpdateGameOverOverlay(AppState *state, GameOverState *gameOver)
{
    if (!state || !gameOver) return;

    if (state->gamestate && state->gamestate->isMatchOver)
    {
        gameOver->animTime += GetFrameTime();
    }
    else
    {
        gameOver->animTime = 0.0f;
    }
}

void DrawGameOverOverlay(AppState *state, GameOverState *gameOver)
{
    if (!state || !gameOver || !state->gamestate || !state->gamestate->isMatchOver) return;

    GameState *match = state->gamestate;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Dark backdrop overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 10, 15, 22, 235 });

    int baseFontSize = fmaxf(16, screenHeight / 38);
    ApplyScrabbleTheme(baseFontSize);

    // Dialog Dimensions
    float dialogW = fminf(screenWidth * 0.70f, 700.0f);
    float dialogH = fminf(screenHeight * 0.75f, 520.0f);
    float dialogX = (screenWidth - dialogW) / 2.0f;
    float dialogY = (screenHeight - dialogH) / 2.0f;

    Rectangle dialogRect = { dialogX, dialogY, dialogW, dialogH };

    // Outer Glow / Shadow & Main Panel Container
    DrawRectangleRounded((Rectangle){ dialogX + 6, dialogY + 8, dialogW, dialogH }, 0.06f, 6, (Color){ 0, 0, 0, 180 });
    DrawRectangleRounded(dialogRect, 0.06f, 6, (Color){ 22, 30, 40, 255 });

    // Gold / Neon accent border
    Color accentBorder = (Color){ 255, 215, 0, 255 }; // Gold
    DrawRectangleRoundedLinesEx(dialogRect, 0.06f, 6, 3.0f, accentBorder);

    // Victory / Game Over Header
    float headerH = 75.0f;
    Rectangle headerRect = { dialogX, dialogY, dialogW, headerH };
    DrawRectangleRounded((Rectangle){ headerRect.x, headerRect.y, headerRect.width, headerH }, 0.06f, 6, (Color){ 32, 42, 56, 255 });
    DrawRectangle(headerRect.x, headerRect.y + headerH - 4.0f, headerRect.width, 4, accentBorder);

    const char *headerTitle = "GAME OVER";
    int headerFontSize = (int)(baseFontSize * 2.2f);
    int headerW = MeasureAppText(headerTitle, headerFontSize);
    DrawAppText(headerTitle, dialogX + (dialogW - headerW) / 2.0f, dialogY + (headerH - headerFontSize) / 2.0f, headerFontSize, (Color){ 244, 228, 198, 255 });

    // Winner Banner Section
    float winnerY = dialogY + headerH + 25.0f;
    int winnerIdx = match->winningPlayerIdx;
    bool isTie = (match->players[0].score == match->players[1].score);

    const char *winnerText = isTie ? "IT'S A DRAW!" : TextFormat("PLAYER %d VICTORY!", winnerIdx + 1);
    Color winnerColor = isTie ? (Color){ 200, 200, 200, 255 } : ((winnerIdx == 0) ? (Color){ 0, 220, 255, 255 } : (Color){ 255, 180, 0, 255 });

    int winnerFontSize = (int)(baseFontSize * 1.8f);
    int winnerW = MeasureAppText(winnerText, winnerFontSize);

    // Glowing winner highlight badge
    Rectangle winnerBadge = { dialogX + (dialogW - winnerW - 40.0f) / 2.0f, winnerY, winnerW + 40.0f, winnerFontSize + 16.0f };
    DrawRectangleRounded(winnerBadge, 0.3f, 4, (Color){ 15, 22, 30, 255 });
    DrawRectangleRoundedLinesEx(winnerBadge, 0.3f, 4, 2.0f, winnerColor);
    DrawAppText(winnerText, winnerBadge.x + 20.0f, winnerBadge.y + 8.0f, winnerFontSize, winnerColor);

    // Score Summary Card Comparison
    float statsY = winnerY + winnerBadge.height + 25.0f;
    float statsH = dialogH - (statsY - dialogY) - 85.0f;
    float colW = (dialogW - 60.0f) / 2.0f;

    for (int p = 0; p < 2; p++)
    {
        Rectangle cardRect = { dialogX + 20.0f + (p * (colW + 20.0f)), statsY, colW, statsH };
        bool isWinnerCard = (!isTie && p == winnerIdx);

        Color cardBg = isWinnerCard ? (Color){ 28, 42, 36, 255 } : (Color){ 16, 24, 34, 255 };
        Color cardBorderC = isWinnerCard ? (Color){ 46, 202, 113, 255 } : (Color){ 45, 60, 78, 255 };

        DrawRectangleRounded(cardRect, 0.08f, 4, cardBg);
        DrawRectangleRoundedLinesEx(cardRect, 0.08f, 4, isWinnerCard ? 2.5f : 1.0f, cardBorderC);

        const char *pName = TextFormat("Player %d", p + 1);
        int pNameW = MeasureAppText(pName, baseFontSize + 2);
        DrawAppText(pName, cardRect.x + (cardRect.width - pNameW) / 2.0f, cardRect.y + 15.0f, baseFontSize + 2, (Color){ 244, 228, 198, 255 });

        // Giant Final Score Display
        const char *scoreValStr = TextFormat("%d", match->players[p].score);
        int scoreFontSize = (int)(baseFontSize * 2.8f);
        int scoreW = MeasureAppText(scoreValStr, scoreFontSize);
        DrawAppText(scoreValStr, cardRect.x + (cardRect.width - scoreW) / 2.0f, cardRect.y + 50.0f, scoreFontSize, isWinnerCard ? (Color){ 46, 202, 113, 255 } : (Color){ 180, 195, 210, 255 });

        DrawAppText("FINAL POINTS", cardRect.x + (cardRect.width - MeasureAppText("FINAL POINTS", baseFontSize - 4)) / 2.0f, cardRect.y + 50.0f + scoreFontSize + 2.0f, baseFontSize - 4, (Color){ 130, 145, 160, 255 });
    }

    // Action Buttons Footer
    float footerY = dialogY + dialogH - 65.0f;
    float btnW = (dialogW - 50.0f) / 2.0f;
    float btnH = 45.0f;

    if (GuiButton((Rectangle){ dialogX + 20.0f, footerY, btnW, btnH }, "Play Again"))
    {
        match->isMatchOver = false;
        StartNewGame(state);
        PlaySoundEffect(SFX_GAME_START);
    }

    if (GuiButton((Rectangle){ dialogX + 30.0f + btnW, footerY, btnW, btnH }, "Return to Main Menu"))
    {
        match->isMatchOver = false;
        state->currentScreen = APP_SCREEN_MAIN_MENU;
        PlaySoundEffect(SFX_BACK_NAV);
    }
}
