#include "game.h"
#include "ui.h"
#include "app_state.h"
#include "raylib.h"
#include "raygui.h"
#include "sound.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void GameInit(GameState *match)
{
    if (match == NULL)
        return;

    memset(match, 0, sizeof(*match));
    match->mode = GAME_MODE_LOCAL_1V1;
    match->specialTilesEnabled = true;
    BoardInit(&match->board, "board_layout.txt");
    init_player(&match->players[0]);
    init_player(&match->players[1]);
    init_bag_from_file(&match->tileBag);
    shuffle_tiles_in_bag(&match->tileBag);
    refill_rack(&match->players[0], &match->tileBag);
    refill_rack(&match->players[1], &match->tileBag);
    match->activePlayerIdx = 0;
    match->tileBagCount = match->tileBag.tiles_remaining;
    PlaySoundEffect(SFX_GAME_START);
}

void GameUpdate(AppState *state)
{
    if (!state)
        return;

    if (IsKeyPressed(KEY_ESCAPE))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void GameDraw(AppState *state)
{
    if (!state)
        return;

    if (state->gamestate == NULL)
    {
        state->gamestate = (GameState *)malloc(sizeof(GameState));
        if (state->gamestate != NULL)
            GameInit(state->gamestate);
    }

    GameState *match = state->gamestate;
    if (!match)
        return;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){24, 32, 38, 255});
    int baseFontSize = fmaxf(15, screenHeight / 42);
    ApplyScrabbleTheme(baseFontSize);

    const float padding = screenWidth * 0.03f;
    const float layoutGap = screenWidth * 0.02f;
    const float rightSideX = (screenWidth * 0.45f) + padding + layoutGap;
    const float rightSideWidth = screenWidth - rightSideX - padding;

    // --- LEFT COLUMN: MAIN BOARD PANEL ---
    Rectangle boardBoundaries = {padding, padding, screenWidth * 0.45f, screenHeight - (padding * 2.0f)};
    GuiGroupBox(boardBoundaries, "Game Board");

    float groupBoxHeaderHeight = 25.0f;
    float gridInnerPad = 15.0f;
    float usableWidth = boardBoundaries.width - (gridInnerPad * 2.0f);
    float usableHeight = boardBoundaries.height - groupBoxHeaderHeight - (gridInnerPad * 2.0f);
    float boardVisualSize = fminf(usableWidth, usableHeight);
    float boardX = boardBoundaries.x + (boardBoundaries.width - boardVisualSize) / 2.0f;
    float boardY = boardBoundaries.y + groupBoxHeaderHeight + (usableHeight - boardVisualSize) / 2.0f;

    DrawRectangle(boardX, boardY, boardVisualSize, boardVisualSize, (Color){34, 44, 52, 255});
    float cellSize = boardVisualSize / (float)match->board.sideSize;

    DrawTheCellTypes(&match->board, boardX, boardY, boardVisualSize, cellSize);

    for (int i = 0; i <= match->board.sideSize; i++)
    {
        DrawLineV((Vector2){boardX + (i * cellSize), boardY}, (Vector2){boardX + (i * cellSize), boardY + boardVisualSize}, (Color){54, 68, 82, 100});
        DrawLineV((Vector2){boardX, boardY + (i * cellSize)}, (Vector2){boardX + boardVisualSize, boardY + (i * cellSize)}, (Color){54, 68, 82, 100});
    }

    // --- RIGHT COLUMN: STATISTICS & MANAGEMENT ---
    float topPanelsY = padding;
    float topPanelsHeight = screenHeight * 0.18f;
    float halfRightWidth = (rightSideWidth - layoutGap) / 2.0f;

    // Block A: Game Details
    Rectangle detailsRect = {rightSideX, topPanelsY, halfRightWidth, topPanelsHeight};
    GuiGroupBox(detailsRect, "Match Status");

    float tableY = detailsRect.y + 25.0f;
    float rowHeight = (topPanelsHeight - 35.0f) / 3.0f;
    const char *keys[3] = {"Mode:", "Special Tiles:", "Tiles Left:"};

    const char *modeStr = (match->mode == GAME_MODE_LOCAL_1V1) ? "Local 1v1" : "AI vs Player";
    const char *tilesLeftStr = TextFormat("%d Tiles", match->tileBagCount);
    const char *values[3] = {modeStr, match->specialTilesEnabled ? "Enabled" : "Disabled", tilesLeftStr};

    for (int i = 0; i < 3; i++)
    {
        float currentRowY = tableY + (i * rowHeight);
        if (i % 2 == 0)
        {
            DrawRectangleRec((Rectangle){detailsRect.x + 6.0f, currentRowY, halfRightWidth - 12.0f, rowHeight}, (Color){30, 38, 46, 180});
        }
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
        GuiLabel((Rectangle){detailsRect.x + 12.0f, currentRowY, halfRightWidth * 0.45f, rowHeight}, keys[i]);
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0xAACF9BFF);
        GuiLabel((Rectangle){detailsRect.x + 12.0f + (halfRightWidth * 0.45f), currentRowY, (halfRightWidth * 0.55f) - 18.0f, rowHeight}, values[i]);
    }

    // Block B: Score Panel
    Rectangle scoreRect = {rightSideX + halfRightWidth + layoutGap, topPanelsY, halfRightWidth, topPanelsHeight};
    GuiGroupBox(scoreRect, "Current Score");

    float scoreColW = (halfRightWidth - 20.0f) / 2.0f;
    int massiveFontSize = (int)(baseFontSize * 2.5f);
    float scoreTextY = scoreRect.y + 25.0f;
    Color neonLime = (Color){58, 223, 0, 255};
    Color separatorColor = (Color){54, 68, 82, 255};

    for (int p = 0; p < 2; p++)
    {
        const char *scoreStr = TextFormat("%03d", match->players[p].score);
        float textX = scoreRect.x + 15.0f + (p * scoreColW) + (scoreColW - MeasureText(scoreStr, massiveFontSize)) / 2.0f;
        DrawText(scoreStr, textX, scoreTextY, massiveFontSize, neonLime);
    }

    float horizontalLineY = scoreTextY + massiveFontSize + 12.0f;
    DrawLineV((Vector2){scoreRect.x + 12.0f, horizontalLineY}, (Vector2){scoreRect.x + scoreRect.width - 12.0f, horizontalLineY}, separatorColor);
    DrawLineV((Vector2){scoreRect.x + 15.0f + scoreColW, scoreRect.y + 20.0f}, (Vector2){scoreRect.x + 15.0f + scoreColW, scoreRect.y + scoreRect.height - 12.0f}, separatorColor);

    GuiSetStyle(LABEL, TEXT_SIZE, (int)(baseFontSize * 0.75f));
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 1);

    GuiLabel((Rectangle){scoreRect.x + 15.0f, horizontalLineY + 8.0f, scoreColW, 20}, "PLAYER 1");
    GuiLabel((Rectangle){scoreRect.x + 15.0f + scoreColW, horizontalLineY + 8.0f, scoreColW, 20}, "PLAYER 2");

    GuiSetStyle(LABEL, TEXT_SIZE, baseFontSize);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0xAACF9BFF);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 0);

    // Center Section: Tile Racks
    float rackSectionY = topPanelsY + topPanelsHeight + layoutGap;
    float rackPanelHeight = screenHeight * 0.10f;

    for (int p = 0; p < 2; p++)
    {
        Rectangle rackRect = {rightSideX, rackSectionY + (p * (rackPanelHeight + (layoutGap * 0.5f))), rightSideWidth, rackPanelHeight};
        GuiGroupBox(rackRect, TextFormat("Player %d Rack", p + 1));

        float tileSize = rackPanelHeight * 0.6f;
        float tileSpacing = 8.0f;
        float tileY = rackRect.y + (rackPanelHeight - tileSize) / 2.0f + 4.0f;

        for (int t = 0; t < match->players[p].rack_count; t++)
        {
            Tile tile = match->players[p].rack[t];
            Rectangle tileBounds = {rackRect.x + 15.0f + (t * (tileSize + tileSpacing)), tileY, tileSize, tileSize};
            DrawRectangleRounded(tileBounds, 0.2f, 4, (Color){244, 228, 198, 255});
            DrawRectangleRoundedLines(tileBounds, 0.2f, 4, (Color){194, 169, 126, 255});

            char letterStr[2] = {tile.letter, '\0'};
            int rackTileFontSize = (int)(tileSize * 0.55f);
            DrawText(letterStr, tileBounds.x + (tileSize * 0.15f), tileBounds.y + (tileSize - rackTileFontSize) / 2.0f, rackTileFontSize, (Color){38, 28, 16, 255});

            int scoreValue = GetTileScore(tile.letter);
            const char *scoreStr = TextFormat("%d", scoreValue);
            int scoreFontSize = (int)(tileSize * 0.22f);
            DrawText(scoreStr, tileBounds.x + tileSize - MeasureText(scoreStr, scoreFontSize) - (tileSize * 0.12f), tileBounds.y + tileSize - scoreFontSize - (tileSize * 0.10f), scoreFontSize, (Color){80, 65, 50, 255});
        }
    }

    // Lower Section: History Logs
    float historySectionY = rackSectionY + (rackPanelHeight * 2.0f) + (layoutGap * 1.5f);
    float bottomRowHeight = screenHeight * 0.07f;
    float historyPanelHeight = screenHeight - historySectionY - bottomRowHeight - padding - layoutGap;

    Rectangle historyOuterRect = {rightSideX, historySectionY, rightSideWidth, historyPanelHeight};
    GuiGroupBox(historyOuterRect, "Points History");

    float historySplitWidth = (rightSideWidth - (layoutGap * 1.5f)) / 2.0f;
    float historyInnerY = historySectionY + 25.0f;
    float ledgerRowH = (historyPanelHeight - 55.0f) / 5.0f;

    for (int p = 0; p < 2; p++)
    {
        Rectangle subHistoryRect = {historyOuterRect.x + 12.0f + (p * (historySplitWidth + layoutGap)), historyInnerY, historySplitWidth, historyPanelHeight - 40.0f};

        DrawRectangleRec(subHistoryRect, (Color){30, 38, 46, 255});
        DrawRectangleLinesEx(subHistoryRect, 1.0f, (Color){54, 68, 82, 255});
        DrawRectangle(subHistoryRect.x, subHistoryRect.y, subHistoryRect.width, 28, (Color){45, 55, 66, 255});
        DrawText(TextFormat("Player %d History", p + 1), subHistoryRect.x + 12, subHistoryRect.y + 7, baseFontSize - 2, (Color){196, 181, 137, 255});

        for (int w = 0; w < 0 && w < 5; w++)
        {
            float currentRowY = subHistoryRect.y + 28.0f + (w * ledgerRowH);
            if (w % 2 == 1)
            {
                DrawRectangle(subHistoryRect.x + 1, currentRowY, subHistoryRect.width - 2, ledgerRowH, (Color){36, 46, 56, 255});
            }

            float textY = currentRowY + (ledgerRowH - (baseFontSize - 2)) / 2.0f;
            DrawText("", subHistoryRect.x + 15.0f, textY, baseFontSize - 2, (Color){200, 200, 215, 255});

            const char *ptsStr = "+0";
            DrawText(ptsStr, subHistoryRect.x + subHistoryRect.width - MeasureText(ptsStr, baseFontSize - 2) - 15.0f, textY, baseFontSize - 2, (Color){111, 200, 140, 255});
        }
    }

    // --- FOOTER MANAGEMENT ACTION BAR ---
    float footerY = screenHeight - bottomRowHeight - padding;
    float elementH = 45.0f;
    float elementY = footerY + (bottomRowHeight - elementH) / 2.0f;

    float actionBtnWidth = rightSideWidth * 0.35f;
    float turnIndicatorWidth = rightSideWidth - actionBtnWidth - layoutGap;

    Rectangle turnRect = {rightSideX, elementY, turnIndicatorWidth, elementH};
    DrawRectangleRec(turnRect, (Color){33, 43, 51, 255});
    DrawRectangleLinesEx(turnRect, 1.0f, separatorColor);

    Color activeAlertColor = (match->activePlayerIdx == 0) ? (Color){0, 220, 255, 255} : (Color){255, 180, 0, 255};
    const char *activeTurnStr = (match->activePlayerIdx == 0) ? "PLAYER 1" : "PLAYER 2";

    int turnFontSize = (int)(baseFontSize * 1.6f);
    float indicatorTextY = turnRect.y + (turnRect.height - turnFontSize) / 2.0f;

    DrawRectangle(turnRect.x + 2, turnRect.y + 2, 8, turnRect.height - 4, activeAlertColor);

    DrawText("TURN:", turnRect.x + 22.0f, indicatorTextY, turnFontSize, (Color){150, 165, 175, 255});
    DrawText(activeTurnStr, turnRect.x + 22.0f + MeasureText("TURN: ", turnFontSize), indicatorTextY, turnFontSize, activeAlertColor);

    Rectangle actionBtnRect = {rightSideX + rightSideWidth - actionBtnWidth, elementY, actionBtnWidth, elementH};
    if (GuiButton(actionBtnRect, "Save & Exit Match"))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}