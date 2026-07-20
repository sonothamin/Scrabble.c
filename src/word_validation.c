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
            continue; // for wild card point is zero
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
            totalScore += 0;
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

        char wordBuffer[BOARD_SIDE + 1] = {0};
        Tile placedTilesBuffer[BOARD_SIDE];
        int charCount = 0;
        bool wordFound = false;

        // Horizontal scan
        for (int y = 0; y < BOARD_SIDE; y++)
        {
            for (int x = 0; x < BOARD_SIDE; x++)
            {
                if (current_Grid[y][x].letter != '\0' && previous_Grid[y][x].letter == '\0')
                {
                    int rightMostX = x;

                    while (rightMostX > 0 && current_Grid[y][rightMostX - 1].letter != '\0')
                    {
                        rightMostX--;
                    }
                    int leftMostX = rightMostX;

                    while (leftMostX < BOARD_SIDE && current_Grid[y][leftMostX].letter != '\0')
                    {
                        wordBuffer[charCount] = current_Grid[y][leftMostX].letter;
                        placedTilesBuffer[charCount] = current_Grid[y][leftMostX];
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
    }
}