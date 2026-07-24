#include "word_validation.h"
#include <string.h>
#include "error_service.h"

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

                    if ((previous_Grid[y][rightMostX].letter != '\0') || (y > 0 && previous_Grid[y - 1][rightMostX].letter != 0) || (y < BOARD_SIDE - 1 && previous_Grid[y + 1][rightMostX].letter != 0))
                    {
                        connectsToExisting = true;
                    }

                    wordBuffer[charCount] = current_Grid[y][rightMostX].letter;
                    placedTilesBuffer[charCount] = current_Grid[y][rightMostX];
                    charCount++;
                    leftMostX++;
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
                            (x < BOARD_SIDE - 1 && previous_Grid[bottomMostY][x + 1].letter != '\0'))
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
        ReportCriticalError("Invalid Move", "A valid word must contain at least 2 letters.");
        return 0;
    }

    if (isFirstMove && !touchesCenter)
    {
        ReportCriticalError("Invalid Move", "Fiet word must touch the starting square.");
        return 0;
    }

    if (!isFirstMove && !connectsToExisting)
    {
        ReportCriticalError("Invalid Move", "Word must connect to existing tiles on the board.");
        return 0;
    }

    if (!Is_Word_In_Dictionary(wordBuffer, dictionary))
    {
        ReportCriticalError("Invalid Word", "The formed word is not in the dictionary.");
        return 0;
    }

    return Calculate_Word_Score(wordBuffer, placedTilesBuffer, charCount);
}