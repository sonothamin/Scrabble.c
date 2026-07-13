#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

typedef struct Tile
{
    char letter;
    int value;
    bool isBlankSubstitute;
} Tile;

void TileInit(Tile* tile, char letter, int value, bool isBlankSubstitute);
int GetTileScore(char letter);

#endif
