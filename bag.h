#ifndef BAG_H
// to prevent duplicate definition when called more than once
#define BAG_H

#include <stdbool.h>

#define MAX_BAG_TILES 100
// Maximum 100 tiles in international game

// Individual tile
typedef struct Tile
{
    char letter;
    int value;
} Tile;

// Complete tile bag
typedef struct TileBag
{
    Tile tiles[MAX_BAG_TILES];
    int tiles_remaining;
} TileBag;

bool init_bag_from_file(TileBag *bag);
void shuffle_bag();
void draw_tile();
#endif