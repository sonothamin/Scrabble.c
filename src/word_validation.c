#include "word_validation.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "error_service.h"
#include "sound.h"

bool Is_Word_In_Dictionary(const char *word, const Dictionary *dictionary)
{
    if (!dictionary || dictionary->size == 0 || !dictionary->words)
    {
        ReportCriticalError("Validation Engine Error", "Attempted to search an uninitialized or empty dictionary");
        return false;
    }

    if (!word)
        return false;

    int left = 0;
    int right = dictionary->size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(word, dictionary->words[mid]);

        if (cmp == 0)
            return true;
        else if (cmp < 0)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return false;
}

int Calculate_Word_Score(const char *word, const Tile *placedTiles, int wordLength)
{
    if (!word || !placedTiles)
        return 0;

    int totalScore = 0;
    for (int i = 0; i < wordLength; i++)
    {
        if (placedTiles[i].isWildCard)
        {
            continue;
        }

        char c = word[i];
        switch (c)
        {
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
        case 'L':
        case 'N':
        case 'R':
        case 'S':
        case 'T':
            totalScore += 1;
            break;

        case 'D':
        case 'G':
            totalScore += 2;
            break;

        case 'B':
        case 'C':
        case 'M':
        case 'P':
            totalScore += 3;
            break;

        case 'F':
        case 'H':
        case 'V':
        case 'W':
        case 'Y':
            totalScore += 4;
            break;

        case 'K':
            totalScore += 5;
            break;

        case 'J':
        case 'X':
            totalScore += 8;
            break;

        case 'Q':
        case 'Z':
            totalScore += 10;
            break;

        default:
            break;
        }
    }
    return totalScore;
}

int Scan_And_Validate_Move(Tile current_Grid[BOARD_SIDE][BOARD_SIDE], Tile previous_Grid[BOARD_SIDE][BOARD_SIDE], const Dictionary *dictionary)
{
    if (!current_Grid || !previous_Grid)
    {
        ReportCriticalError("Board Evaluation Failure", "Invalid memory address referenced for grid during scanning");
        return -1;
    }

    char wordBuffer[BOARD_SIDE + 1] = {0};
    Tile placedTilesBuffer[BOARD_SIDE];
    int charCount = 0;
    bool wordFound = false;

    // Check if it is the very first move
    bool isFirstMove = true;
    for (int y = 0; y < BOARD_SIDE && isFirstMove; y++)
    {
        for (int x = 0; x < BOARD_SIDE; x++)
        {
            if (previous_Grid[y][x].letter != '\0')
            {
                isFirstMove = false;
                break;
            }
        }
    }

    bool touchesCenter = false;
    bool connectsToExisting = false;
    int centerPos = BOARD_SIDE / 2;

    // Horizontal scan
    for (int y = 0; y < BOARD_SIDE; y++)
    {
        for (int x = 0; x < BOARD_SIDE; x++)
        {
            if ((current_Grid[y][x].letter != '\0') && (previous_Grid[y][x].letter == '\0'))
            {

                int leftMostX = x;
                while (leftMostX > 0 && current_Grid[y][leftMostX - 1].letter != '\0')
                {
                    leftMostX--;
                }

                int rightMostX = leftMostX;
                while (rightMostX < BOARD_SIDE && current_Grid[y][rightMostX].letter != '\0')
                {
                    if ((y == centerPos) && (rightMostX == centerPos))
                    {
                        touchesCenter = true;
                    }

                    if ((previous_Grid[y][rightMostX].letter != '\0') ||
                        (y > 0 && previous_Grid[y - 1][rightMostX].letter != '\0') ||
                        (y < BOARD_SIDE - 1 && previous_Grid[y + 1][rightMostX].letter != '\0') ||
                        (rightMostX > 0 && previous_Grid[y][rightMostX - 1].letter != '\0') ||            // ✅ ADDED LEFT CHECK
                        (rightMostX < BOARD_SIDE - 1 && previous_Grid[y][rightMostX + 1].letter != '\0')) // ✅ ADDED RIGHT CHECK
                    {
                        connectsToExisting = true;
                    }

                    wordBuffer[charCount] = current_Grid[y][rightMostX].letter;
                    placedTilesBuffer[charCount] = current_Grid[y][rightMostX];
                    charCount++;
                    rightMostX++;
                }
                wordBuffer[charCount] = '\0';

                if (charCount > 1)
                {
                    wordFound = true;
                    break;
                }
            }
        }
        if (wordFound)
            break;
    }

    // Vertical scan if no horizontal word was found
    if (!wordFound)
    {
        charCount = 0; // Reset char counter for vertical scan
        for (int x = 0; x < BOARD_SIDE; x++)
        {
            for (int y = 0; y < BOARD_SIDE; y++)
            {
                if (current_Grid[y][x].letter != '\0' && previous_Grid[y][x].letter == '\0')
                {
                    // Find topmost bound
                    int topMostY = y;
                    while (topMostY > 0 && current_Grid[topMostY - 1][x].letter != '\0')
                    {
                        topMostY--;
                    }

                    // Read downward to bottommost bound
                    int bottomMostY = topMostY;
                    while (bottomMostY < BOARD_SIDE && current_Grid[bottomMostY][x].letter != '\0')
                    {
                        if (bottomMostY == centerPos && x == centerPos)
                        {
                            touchesCenter = true;
                        }

                        if (previous_Grid[bottomMostY][x].letter != '\0' ||
                            (x > 0 && previous_Grid[bottomMostY][x - 1].letter != '\0') ||
                            (x < BOARD_SIDE - 1 && previous_Grid[bottomMostY][x + 1].letter != '\0') ||
                            (bottomMostY > 0 && previous_Grid[bottomMostY - 1][x].letter != '\0') ||            // ✅ ADDED TOP CHECK
                            (bottomMostY < BOARD_SIDE - 1 && previous_Grid[bottomMostY + 1][x].letter != '\0')) // ✅ ADDED BOTTOM CHECK
                        {
                            connectsToExisting = true;
                        }
                        wordBuffer[charCount] = current_Grid[bottomMostY][x].letter;
                        placedTilesBuffer[charCount] = current_Grid[bottomMostY][x];
                        charCount++;
                        bottomMostY++;
                    }
                    wordBuffer[charCount] = '\0';

                    if (charCount > 1)
                    {
                        wordFound = true;
                        break;
                    }
                }
            }
            if (wordFound)
                break;
        }
    }

    if (!wordFound || charCount < 2)
    {
        ReportGameWarning("Invalid Move", "A valid word must contain at least 2 letters.", 3.0f);
        PlaySoundEffect(SFX_INVALID_MOVE);
        return 0;
    }

    if (isFirstMove && !touchesCenter)
    {
        ReportGameWarning("Invalid Move", "Fiet word must touch the starting square.", 3.0f);
        return 0;
    }

    if (!isFirstMove && !connectsToExisting)
    {
        ReportGameWarning("Invalid Move", "Word must connect to existing tiles on the board.", 3.0f);
        PlaySoundEffect(SFX_INVALID_MOVE);
        return 0;
    }
    for (int i = 0; wordBuffer[i] != '\0'; i++)
    {
        wordBuffer[i] = (char)toupper((unsigned char)wordBuffer[i]);
    }

    if (!Is_Word_In_Dictionary(wordBuffer, dictionary))
    {
        char dictWarningMsg[256];
        snprintf(dictWarningMsg, sizeof(dictWarningMsg), "%s is not in the dictionary.", wordBuffer);
        ReportGameWarning("Invalid Word", dictWarningMsg, 3.0f);
        PlaySoundEffect(SFX_INVALID_MOVE);
        return 0;
    }

    return Calculate_Word_Score(wordBuffer, placedTilesBuffer, charCount);
}