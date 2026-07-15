#include "tile.h"
#include <stddef.h>

void TileInit(Tile* tile, char letter, int value, bool isBlankSubstitute)
{
    if (tile == NULL)
        return;

    tile->letter = letter;
    tile->value = value;
    tile->isBlankSubstitute = isBlankSubstitute;
}

int GetTileScore(char letter)
{
    static const int values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};
    if (letter >= 'A' && letter <= 'Z') return values[letter - 'A'];
    if (letter >= 'a' && letter <= 'z') return values[letter - 'a'];
    return 0;
}
