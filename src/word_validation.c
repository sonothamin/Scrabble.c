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

typedef struct {
    char word[BOARD_SIDE + 1];
    Tile tiles[BOARD_SIDE];
    int length;
} FormedWord;

int Scan_And_Validate_Move(Tile current_Grid[BOARD_SIDE][BOARD_SIDE], Tile previous_Grid[BOARD_SIDE][BOARD_SIDE], const Dictionary *dictionary)
{
    if (!current_Grid || !previous_Grid)
    {
        ReportCriticalError("Board Evaluation Failure", "Invalid memory address referenced for grid during scanning");
        return -1;
    }

    // 1. Identify newly placed tiles
    int newCount = 0;
    int newX[BOARD_SIDE * BOARD_SIDE];
    int newY[BOARD_SIDE * BOARD_SIDE];

    for (int y = 0; y < BOARD_SIDE; y++)
    {
        for (int x = 0; x < BOARD_SIDE; x++)
        {
            if (current_Grid[y][x].letter != '\0' && previous_Grid[y][x].letter == '\0')
            {
                newX[newCount] = x;
                newY[newCount] = y;
                newCount++;
            }
        }
    }

    if (newCount == 0)
    {
        return 0;
    }

    // 2. Determine orientation & alignment
    bool isHorizontal = true;
    bool isVertical = true;

    int firstX = newX[0];
    int firstY = newY[0];

    for (int i = 1; i < newCount; i++)
    {
        if (newY[i] != firstY) isHorizontal = false;
        if (newX[i] != firstX) isVertical = false;
    }

    if (!isHorizontal && !isVertical)
    {
        ReportGameWarning("Invalid Move", "Newly placed tiles must be in a single straight line.", 3.0f);
        PlaySoundEffect(SFX_INVALID_MOVE);
        return 0;
    }

    // Check continuity along placement axis
    if (newCount > 1)
    {
        if (isHorizontal)
        {
            int minX = newX[0], maxX = newX[0];
            for (int i = 1; i < newCount; i++)
            {
                if (newX[i] < minX) minX = newX[i];
                if (newX[i] > maxX) maxX = newX[i];
            }
            for (int x = minX; x <= maxX; x++)
            {
                if (current_Grid[firstY][x].letter == '\0')
                {
                    ReportGameWarning("Invalid Move", "Newly placed tiles must form a continuous sequence.", 3.0f);
                    PlaySoundEffect(SFX_INVALID_MOVE);
                    return 0;
                }
            }
        }
        else // isVertical
        {
            int minY = newY[0], maxY = newY[0];
            for (int i = 1; i < newCount; i++)
            {
                if (newY[i] < minY) minY = newY[i];
                if (newY[i] > maxY) maxY = newY[i];
            }
            for (int y = minY; y <= maxY; y++)
            {
                if (current_Grid[y][firstX].letter == '\0')
                {
                    ReportGameWarning("Invalid Move", "Newly placed tiles must form a continuous sequence.", 3.0f);
                    PlaySoundEffect(SFX_INVALID_MOVE);
                    return 0;
                }
            }
        }
    }

    // 3. First move vs connectivity check
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

    int centerPos = BOARD_SIDE / 2;
    if (isFirstMove)
    {
        bool touchesCenter = false;
        for (int i = 0; i < newCount; i++)
        {
            if (newX[i] == centerPos && newY[i] == centerPos)
            {
                touchesCenter = true;
                break;
            }
        }
        if (!touchesCenter)
        {
            ReportGameWarning("Invalid Move", "First word must cover the center star square.", 3.0f);
            PlaySoundEffect(SFX_INVALID_MOVE);
            return 0;
        }
    }
    else
    {
        bool connectsToExisting = false;
        for (int i = 0; i < newCount && !connectsToExisting; i++)
        {
            int x = newX[i];
            int y = newY[i];
            if ((x > 0 && previous_Grid[y][x - 1].letter != '\0') ||
                (x < BOARD_SIDE - 1 && previous_Grid[y][x + 1].letter != '\0') ||
                (y > 0 && previous_Grid[y - 1][x].letter != '\0') ||
                (y < BOARD_SIDE - 1 && previous_Grid[y + 1][x].letter != '\0'))
            {
                connectsToExisting = true;
            }
        }
        if (!connectsToExisting)
        {
            if (isHorizontal || newCount == 1)
            {
                int y = firstY;
                int startX = newX[0];
                while (startX > 0 && current_Grid[y][startX - 1].letter != '\0') startX--;
                int endX = newX[0];
                while (endX < BOARD_SIDE - 1 && current_Grid[y][endX + 1].letter != '\0') endX++;
                for (int x = startX; x <= endX; x++)
                {
                    if (previous_Grid[y][x].letter != '\0')
                    {
                        connectsToExisting = true;
                        break;
                    }
                }
            }
            if (!connectsToExisting && (isVertical || newCount == 1))
            {
                int x = firstX;
                int startY = newY[0];
                while (startY > 0 && current_Grid[startY - 1][x].letter != '\0') startY--;
                int endY = newY[0];
                while (endY < BOARD_SIDE - 1 && current_Grid[endY + 1][x].letter != '\0') endY++;
                for (int y = startY; y <= endY; y++)
                {
                    if (previous_Grid[y][x].letter != '\0')
                    {
                        connectsToExisting = true;
                        break;
                    }
                }
            }
        }

        if (!connectsToExisting)
        {
            ReportGameWarning("Invalid Move", "Word must connect to existing tiles on the board.", 3.0f);
            PlaySoundEffect(SFX_INVALID_MOVE);
            return 0;
        }
    }

    // 4. Collect all words formed by newly placed tiles
    FormedWord words[BOARD_SIDE + 1];
    int wordCount = 0;

    bool checkHorizMain = isHorizontal || (newCount == 1);
    bool checkVertMain  = isVertical   || (newCount == 1);

    if (checkHorizMain)
    {
        int y = newY[0];
        int startX = newX[0];
        while (startX > 0 && current_Grid[y][startX - 1].letter != '\0') startX--;
        int endX = newX[0];
        while (endX < BOARD_SIDE - 1 && current_Grid[y][endX + 1].letter != '\0') endX++;

        int len = endX - startX + 1;
        if (len > 1)
        {
            FormedWord *fw = &words[wordCount++];
            fw->length = len;
            for (int k = 0; k < len; k++)
            {
                fw->word[k] = current_Grid[y][startX + k].letter;
                fw->tiles[k] = current_Grid[y][startX + k];
            }
            fw->word[len] = '\0';
        }
    }

    if (checkVertMain)
    {
        int x = newX[0];
        int startY = newY[0];
        while (startY > 0 && current_Grid[startY - 1][x].letter != '\0') startY--;
        int endY = newY[0];
        while (endY < BOARD_SIDE - 1 && current_Grid[endY + 1][x].letter != '\0') endY++;

        int len = endY - startY + 1;
        if (len > 1)
        {
            FormedWord *fw = &words[wordCount++];
            fw->length = len;
            for (int k = 0; k < len; k++)
            {
                fw->word[k] = current_Grid[startY + k][x].letter;
                fw->tiles[k] = current_Grid[startY + k][x];
            }
            fw->word[len] = '\0';
        }
    }

    // Cross-words check for each newly placed tile
    for (int i = 0; i < newCount; i++)
    {
        int x = newX[i];
        int y = newY[i];

        if (isHorizontal && newCount > 1)
        {
            int startY = y;
            while (startY > 0 && current_Grid[startY - 1][x].letter != '\0') startY--;
            int endY = y;
            while (endY < BOARD_SIDE - 1 && current_Grid[endY + 1][x].letter != '\0') endY++;

            int len = endY - startY + 1;
            if (len > 1)
            {
                FormedWord *fw = &words[wordCount++];
                fw->length = len;
                for (int k = 0; k < len; k++)
                {
                    fw->word[k] = current_Grid[startY + k][x].letter;
                    fw->tiles[k] = current_Grid[startY + k][x];
                }
                fw->word[len] = '\0';
            }
        }

        if (isVertical && newCount > 1)
        {
            int startX = x;
            while (startX > 0 && current_Grid[y][startX - 1].letter != '\0') startX--;
            int endX = x;
            while (endX < BOARD_SIDE - 1 && current_Grid[y][endX + 1].letter != '\0') endX++;

            int len = endX - startX + 1;
            if (len > 1)
            {
                FormedWord *fw = &words[wordCount++];
                fw->length = len;
                for (int k = 0; k < len; k++)
                {
                    fw->word[k] = current_Grid[y][startX + k].letter;
                    fw->tiles[k] = current_Grid[y][startX + k];
                }
                fw->word[len] = '\0';
            }
        }
    }

    if (wordCount == 0)
    {
        ReportGameWarning("Invalid Move", "A valid move must form at least one word of 2 or more letters.", 3.0f);
        PlaySoundEffect(SFX_INVALID_MOVE);
        return 0;
    }

    // 5. Validate each formed word against dictionary & calculate total score
    int totalScore = 0;
    for (int w = 0; w < wordCount; w++)
    {
        for (int k = 0; k < words[w].length; k++)
        {
            words[w].word[k] = (char)toupper((unsigned char)words[w].word[k]);
        }

        if (!Is_Word_In_Dictionary(words[w].word, dictionary))
        {
            char dictWarningMsg[256];
            snprintf(dictWarningMsg, sizeof(dictWarningMsg), "%s is not in the dictionary.", words[w].word);
            ReportGameWarning("Invalid Word", dictWarningMsg, 3.0f);
            PlaySoundEffect(SFX_INVALID_MOVE);
            return 0;
        }

        totalScore += Calculate_Word_Score(words[w].word, words[w].tiles, words[w].length);
    }

    return totalScore;
}