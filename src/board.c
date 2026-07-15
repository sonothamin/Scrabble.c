#include "board.h"
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "raylib.h"

void BoardInit(GameBoard *board, const char *filename)
{
    if (board == NULL)
        return;

    BoardReset(board);
    board->sideSize = BOARD_SIDE;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        file = fopen("resources/board_layout.txt", "r");
    }
    if (file == NULL)
    {
        return;
    }

    char token[4]; // 3 for the type and one is the null character
    int row = 0, col = 0;
    while ((fscanf(file, "%3s", token)) != EOF && (row < BOARD_SIDE))
    {
        if (strcmp(token, "..") == 0)
            board->cells[row][col] = LUXURY_NONE;
        else if (strcmp(token, "2L") == 0)
            board->cells[row][col] = LUXURY_DOUBLE_LETTER;
        else if (strcmp(token, "2W") == 0)
            board->cells[row][col] = LUXURY_DOUBLE_WORD;
        else if (strcmp(token, "3L") == 0)
            board->cells[row][col] = LUXURY_TRIPLE_LETTER;
        else if (strcmp(token, "3W") == 0)
            board->cells[row][col] = LUXURY_TRIPLE_WORD;
        else if (strcmp(token, "ST") == 0)
            board->cells[row][col] = LUXURY_DOUBLE_WORD; // when the 1st word is formed it is automatically double word
        else
            board->cells[row][col] = LUXURY_NONE;
        col++;
        if (col >= BOARD_SIDE)
        {
            col = 0;
            row++;
        }
    }
    fclose(file);
}

void BoardReset(GameBoard *board)
{
    if (board == NULL)
        return;

    memset(board, 0, sizeof(*board));
    board->sideSize = BOARD_SIDE;
}

void DrawTheCellTypes(GameBoard *match, float boardX, float boardY, float boardVisualSize, float cellSize)
{
    if (!match)
        return;

    for (int r = 0; r < match->sideSize; r++)
    {
        for (int c = 0; c < match->sideSize; c++)
        {
            // Calculate exact bounding rectangle for this cell coordinates
            Rectangle cellBounds = {
                boardX + (c * cellSize),
                boardY + (r * cellSize),
                cellSize,
                cellSize};

            Color bgStyle = (Color){00, 00, 00, 255}; // Default normal board color
            const char *cellText = "";
            Color textColor = (Color){200, 200, 215, 255};

            // Read the logic type mapping from your game state matrix
            switch (match->cells[r][c])
            {
            case LUXURY_DOUBLE_LETTER:
                bgStyle = (Color){155, 203, 237, 255}; // Light Blue
                cellText = "DL";
                textColor = (Color){25, 45, 65, 255};
                break;
            case LUXURY_TRIPLE_LETTER:
                bgStyle = (Color){45, 120, 210, 255}; // Dark Blue
                cellText = "TL";
                textColor = (Color){255, 255, 255, 255};
                break;
            case LUXURY_DOUBLE_WORD:
                bgStyle = (Color){237, 160, 160, 255}; // Pink / Coral
                cellText = "DW";
                textColor = (Color){65, 20, 20, 255};

                if (r == match->sideSize / 2 && c == match->sideSize / 2)
                {
                    cellText = "*";
                }
                break;
            case LUXURY_TRIPLE_WORD:
                bgStyle = (Color){220, 55, 55, 255}; // Deep Red
                cellText = "TW";
                textColor = (Color){255, 255, 255, 255};
                break;
            case LUXURY_NONE:
            default:
                break;
            }

            // Draw styling if it's a special bonus multiplier square
            if (match->cells[r][c] != LUXURY_NONE)
            {
                // Inset by 1px so it nests perfectly inside your grid outline lines
                DrawRectangleRec((Rectangle){cellBounds.x + 1, cellBounds.y + 1, cellBounds.width - 2, cellBounds.height - 2}, bgStyle);

                int cellFontSize = (int)(cellSize * 0.38f);
                int textWidth = MeasureText(cellText, cellFontSize);

                float textX = cellBounds.x + (cellSize - textWidth) / 2.0f;
                float textY = cellBounds.y + (cellSize - cellFontSize) / 2.0f;

                DrawText(cellText, textX, textY, cellFontSize, textColor);
            }
        }
    }
}