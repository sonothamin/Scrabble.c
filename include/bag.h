#ifndef BAG_H
#define BAG_H

#include <stdbool.h>

#define MAX_BAG_TILES 100

typedef struct Tile
{
    char letter;
    int value;
} Tile;

typedef struct TileBag
{
    Tile tiles[MAX_BAG_TILES];
    int tiles_remaining;
} TileBag;

bool init_bag_from_file(TileBag *bag);
void shuffle_tiles_in_bag(TileBag *bag);
Tile taking_out_tile_from_bag(TileBag *bag);
#endif