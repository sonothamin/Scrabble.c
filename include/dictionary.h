#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

typedef struct Dictionary
{
    char **words; // array of addresses of individual words in RAM
    int size;     // number of words successfully read from file and loaded in memory
    int capacity; // dynamic
} Dictionary;

bool Load_Dictionary(Dictionary *dictionary, const char *dictionary_file);
void Free_Dictionary(Dictionary *dictionary);

#endif