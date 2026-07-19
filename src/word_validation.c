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

        if(cmp == 0) return true;
        else if (cmp < 0) right = mid - 1;
        else left = mid + 1;
        
    }
    return false;
}
