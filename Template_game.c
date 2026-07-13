#include "game.h"
#include "ui.h"
#include "raylib.h"
#include "raygui.h"

// Concrete structures matching the state machine placeholders
typedef struct {
    char letter;
    int value;
} GameTile;

typedef struct {
    char tiles[7];
    int score;
    char history[10][15]; // Last 10 words played
    int historyCount;
} PlayerData;

typedef struct {
    GameTile board[15][15];
    PlayerData players[2];
    int activePlayer;
    char gameModeStr[32];
    char specialTilesStr[32];
    char latestWordStr[32];
} InternalGameState;

// Local persistence for the state machine mockup
static InternalGameState g_MatchData = {
    .activePlayer = 0,
    .gameModeStr = "Local 1v1 Match",
    .specialTilesStr = "Standard Layout",
    .latestWordStr = "QUARTZ (+24 pts)",
    .players = {
        {
            .score = 42,
            .tiles = {'S', 'C', 'R', 'A', 'B', 'L', 'E'},
            .historyCount = 2,
            .history = {"HELLO", "WORLD"}
        },
        {
            .score = 38,
            .tiles = {'T', 'E', 'S', 'T', 'I', 'N', 'G'},
            .historyCount = 2,
            .history = {"RAYLIB", "C_LANG"}
        }
    }
};

void GameUpdate(AppState* state)
{
    if (!state) return;

    // Fast back-out routine
    if (IsKeyPressed(KEY_ESCAPE))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void GameDraw(AppState* state)
{
    if (!state) return;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // 1. Maintain Scrabble Dark Slate Core Base
    ClearBackground((Color){ 24, 32, 38, 255 });

    int baseFontSize = screenHeight / 42; 
    if (baseFontSize < 15) baseFontSize = 15;
    ApplyScrabbleTheme(baseFontSize);

    // 2. Blueprint Spatial Anchors
    const float padding = screenWidth * 0.03f;
    const float layoutGap = screenWidth * 0.02f;
    const float rightSideX = (screenWidth * 0.45f) + padding + layoutGap;
    const float rightSideWidth = screenWidth - rightSideX - padding;

    // --- LEFT COLUMN: MAIN BOARD PANEL ---
    float boardPanelWidth = (screenWidth * 0.45f);
    float boardPanelHeight = screenHeight - (padding * 2.0f);
    Rectangle boardBoundaries = { padding, padding, boardPanelWidth, boardPanelHeight };
    
    GuiGroupBox(boardBoundaries, "Game Board Here");
    
    // Draw a grid pattern mock inside the board bounds
    float gridInnerPad = 15.0f;
    float boardVisualSize = (boardPanelWidth < boardPanelHeight - 40.0f) ? boardPanelWidth - (gridInnerPad * 2.0f) : boardPanelHeight - 40.0f - (gridInnerPad * 2.0f);
    float boardX = padding + gridInnerPad;
    float boardY = padding + 30.0f + gridInnerPad;
    
    DrawRectangle(boardX, boardY, boardVisualSize, boardVisualSize, (Color){ 34, 44, 52, 255 });
    float cellSize = boardVisualSize / 15.0f;
    for (int i = 0; i <= 15; i++) 
    {
        DrawLineV((Vector2){ boardX + (i * cellSize), boardY }, (Vector2){ boardX + (i * cellSize), boardY + boardVisualSize }, (Color){ 54, 68, 82, 100 });
        DrawLineV((Vector2){ boardX, boardY + (i * cellSize) }, (Vector2){ boardX + boardVisualSize, boardY + (i * cellSize) }, (Color){ 54, 68, 82, 100 });
    }

    // --- RIGHT COLUMN: STATISTICS & MANAGEMENT ---
    
    // Top Row: Game Details & Score split panels
    float topPanelsY = padding;
    float topPanelsHeight = screenHeight * 0.16f;
    float halfRightWidth = (rightSideWidth - layoutGap) / 2.0f;

    // Block A: Game Details Panel
    Rectangle detailsRect = { rightSideX, topPanelsY, halfRightWidth, topPanelsHeight };
    GuiGroupBox(detailsRect, "Game Details");
    
    float detailsTextX = detailsRect.x + 15.0f;
    float detailsRowH = (topPanelsHeight - 20.0f) / 3.0f;
    GuiLabel((Rectangle){ detailsTextX, detailsRect.y + 15.0f, halfRightWidth - 20.0f, detailsRowH }, TextFormat("Game Mode: %s", g_MatchData.gameModeStr));
    GuiLabel((Rectangle){ detailsTextX, detailsRect.y + 15.0f + detailsRowH, halfRightWidth - 20.0f, detailsRowH }, TextFormat("Special Tiles: %s", g_MatchData.specialTilesStr));
    GuiLabel((Rectangle){ detailsTextX, detailsRect.y + 15.0f + (detailsRowH * 2.0f), halfRightWidth - 20.0f, detailsRowH }, TextFormat("Latest Word: %s", g_MatchData.latestWordStr));

    // Block B: Score Grid Panel
    Rectangle scoreRect = { rightSideX + halfRightWidth + layoutGap, topPanelsY, halfRightWidth, topPanelsHeight };
    GuiGroupBox(scoreRect, "Score");
    
    float scoreColW = (halfRightWidth - 20.0f) / 2.0f;
    // Massive scoreboard fonts
    GuiSetStyle(LABEL, TEXT_SIZE, baseFontSize * 1.5);
    GuiLabel((Rectangle){ scoreRect.x + 15.0f, scoreRect.y + 15.0f, scoreColW, topPanelsHeight * 0.4f }, TextFormat("%03d", g_MatchData.players[0].score));
    GuiLabel((Rectangle){ scoreRect.x + 15.0f + scoreColW, scoreRect.y + 15.0f, scoreColW, topPanelsHeight * 0.4f }, TextFormat("%03d", g_MatchData.players[1].score));
    
    // Reset Label styles for titles below scores
    GuiSetStyle(LABEL, TEXT_SIZE, baseFontSize);
    GuiLabel((Rectangle){ scoreRect.x + 15.0f, scoreRect.y + 15.0f + (topPanelsHeight * 0.4f), scoreColW, 20 }, "Player 1 (A)");
    GuiLabel((Rectangle){ scoreRect.x + 15.0f + scoreColW, scoreRect.y + 15.0f + (topPanelsHeight * 0.4f), scoreColW, 20 }, "Player 2 (B)");

    // Center Section: Tile Racks
    float rackSectionY = topPanelsY + topPanelsHeight + layoutGap;
    float rackPanelHeight = screenHeight * 0.10f;
    
    for (int p = 0; p < 2; p++)
    {
        Rectangle rackRect = { rightSideX, rackSectionY + (p * (rackPanelHeight + (layoutGap * 0.5f))), rightSideWidth, rackPanelHeight };
        GuiGroupBox(rackRect, TextFormat("Player %d Rack", p + 1));
        
        float tileSize = rackPanelHeight * 0.6f;
        float tileSpacing = 8.0f;
        float startTileX = rackRect.x + 15.0f;
        float tileY = rackRect.y + (rackPanelHeight - tileSize) / 2.0f + 4.0f;
        
        for (int t = 0; t < 7; t++)
        {
            Rectangle singleTileBound = { startTileX + (t * (tileSize + tileSpacing)), tileY, tileSize, tileSize };
            
            // Standard Luxury Tile Drawing
            DrawRectangleRounded(singleTileBound, 0.2f, 4, (Color){ 244, 228, 198, 255 });
            DrawRectangleRoundedLines(singleTileBound, 0.2f, 4, (Color){ 194, 169, 126, 255 });
            
            char letterStr[2] = { g_MatchData.players[p].tiles[t], '\0' };
            int rackTileFontSize = (int)(tileSize * 0.60f);
            DrawText(
                letterStr, 
                singleTileBound.x + (tileSize - MeasureText(letterStr, rackTileFontSize)) / 2.0f, 
                singleTileBound.y + (tileSize - rackTileFontSize) / 2.0f, 
                rackTileFontSize, 
                (Color){ 38, 28, 16, 255 }
            );
        }
    }

    // Lower Section: History Logs Panel
    float historySectionY = rackSectionY + (rackPanelHeight * 2.0f) + (layoutGap * 1.5f);
    float bottomRowHeight = screenHeight * 0.07f;
    float historyPanelHeight = screenHeight - historySectionY - bottomRowHeight - padding - layoutGap;

    Rectangle historyOuterRect = { rightSideX, historySectionY, rightSideWidth, historyPanelHeight };
    GuiGroupBox(historyOuterRect, "MATCH WORD HISTORY");

    float historySplitWidth = (rightSideWidth - (layoutGap * 1.5f)) / 2.0f;
    float historyInnerY = historySectionY + 20.0f;
    float historyInnerHeight = historyPanelHeight - 35.0f;

    for (int p = 0; p < 2; p++)
    {
        Rectangle subHistoryRect = { historyOuterRect.x + 12.0f + (p * (historySplitWidth + layoutGap)), historyInnerY, historySplitWidth, historyInnerHeight };
        
        // Light framing tint for tracking clear lists
        DrawRectangleRec(subHistoryRect, (Color){ 30, 38, 46, 255 });
        DrawRectangleLinesEx(subHistoryRect, 1.0f, (Color){ 54, 68, 82, 255 });
        
        // Header
        DrawRectangle(subHistoryRect.x, subHistoryRect.y, subHistoryRect.width, 24, (Color){ 45, 55, 66, 255 });
        DrawText(TextFormat(" Player %d Logs", p + 1), subHistoryRect.x + 5, subHistoryRect.y + 5, baseFontSize - 2, (Color){ 170, 207, 155, 255 });

        // Words
        float rowSpacing = (historyInnerHeight - 30.0f) / 6.0f;
        for (int w = 0; w < g_MatchData.players[p].historyCount && w < 5; w++)
        {
            DrawText(
                TextFormat("- %s", g_MatchData.players[p].history[w]), 
                subHistoryRect.x + 15.0f, 
                subHistoryRect.y + 34.0f + (w * rowSpacing), 
                baseFontSize - 1, 
                (Color){ 143, 143, 160, 255 }
            );
        }
    }

    // --- ESCAPE MANAGEMENT ACTION BAR ---
    float footerY = screenHeight - bottomRowHeight - padding;
    float actionBtnWidth = rightSideWidth * 0.35f;
    Rectangle actionBtnRect = { rightSideX + rightSideWidth - actionBtnWidth, footerY + (bottomRowHeight - 35.0f) / 2.0f, actionBtnWidth, 35.0f };

    if (GuiButton(actionBtnRect, "Save & Exit Match"))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }
}