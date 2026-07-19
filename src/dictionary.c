#include "dictionary.h"
#include "error_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool Load_Dictionary(Dictionary *dictionary, const char *dictionary_file)
{
    FILE *file = fopen(dictionary_file, "r");

    if (!file)
    {
        ReportCriticalError("Dictionary Load Failure", TextFormat("Could not Locate dictionary file at: %s", dictionary_file));
        return false;
    }

    dictionary->size = 0;
    dictionary->capacity = 1000;                                       // initial capacity, it can be expanded using realloc
    dictionary->words = malloc(dictionary->capacity * sizeof(char *)); // typically 1000*8 = 8000 bytes

    if (!dictionary->words)
    {
        ReportCriticalError("System Memory Exhausted", "Failed to allocate memory array for dictionary");
        fclose(file);
        return false;
    }

    char line[64]; // array for each line
    while (fgets(line, sizeof(line), file))
    {
        strtok(line, "\r\n"); // removes the hidden newline

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
    }
}