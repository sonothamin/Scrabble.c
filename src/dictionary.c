#include "dictionary.h"
#include "error_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void Free_Dictionary(Dictionary *dictionary)
{
    if (!dictionary || !dictionary->words)
        return;

    for (int i = 0; i < dictionary->size; i++)
    {
        free(dictionary->words[i]);
    }

    free(dictionary->words);
    dictionary->words = NULL;
    dictionary->size = 0;
    dictionary->capacity = 0;
}

bool Load_Dictionary(Dictionary *dictionary, const char *dictionary_file)
{
    if (!dictionary || !dictionary_file)
    {
        ReportCriticalError("Dictionary struct or file is not received", "Failed to receive struct or the dictionary file");
        return false;
    }

    FILE *file = fopen(dictionary_file, "r");

    if (!file)
    {
        char errorMessage[512];
        snprintf(errorMessage, sizeof(errorMessage), "Could not open dictionary file from: %s", dictionary_file);
        ReportCriticalError("Dictionary Load Failure", errorMessage);
        return false;
    }

    dictionary->size = 0;
    dictionary->capacity = 1000;
    dictionary->words = malloc(dictionary->capacity * sizeof(char *));

    if (!dictionary->words)
    {
        ReportCriticalError("System Memory Exhausted", "Failed to allocate memory array for dictionary");
        fclose(file);
        return false;
    }

    char line[64];
    while (fgets(line, sizeof(line), file))
    {
        strtok(line, "\r\n");

        if (strlen(line) == 0)
            continue;

        if (dictionary->size >= dictionary->capacity)
        {
            dictionary->capacity *= 2;
            char **temp = realloc(dictionary->words, dictionary->capacity * sizeof(char *));
            if (!temp)
            {
                ReportCriticalError("System Memory Exhausted", "Reallocation failed while expanding capacity");
                Free_Dictionary(dictionary);
                fclose(file);
                return false;
            }
            dictionary->words = temp;
        }

        int wordLength = strlen(line) + 1;
        dictionary->words[dictionary->size] = malloc(wordLength * sizeof(char));
        if (!dictionary->words[dictionary->size])
        {
            ReportCriticalError("System Memory Exhausted", "Failed to allocate memory for a dictionary word");
            Free_Dictionary(dictionary);
            fclose(file);
            return false;
        }

        memcpy(dictionary->words[dictionary->size], line, wordLength);
        dictionary->size++;
    }

    fclose(file);
    return true;
}