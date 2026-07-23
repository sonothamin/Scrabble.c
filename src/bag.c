#include "bag.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

bool init_bag_from_file(TileBag *bag)
{
    FILE *file = fopen("resources/letters.txt", "r");
    if (file == NULL)
    {
        file = fopen("letters.txt", "r");
    }

    if (file == NULL)
    {
        printf("Could not open the letters file\n");
        return false;
    }
    bag->tiles_remaining = 0;
    char letter;
    int quantity;
    int value;

    while ((fscanf(file, " %c %d %d", &letter, &quantity, &value)) == 3)
    {
        for (int i = 0; i < quantity; i++)
        {
            if (bag->tiles_remaining >= MAX_BAG_TILES)
                break;

            bag->tiles[bag->tiles_remaining].letter = letter;
            bag->tiles[bag->tiles_remaining].value = value;
            bag->tiles_remaining++;
        }
    }
    fclose(file);
    return true;
}

void shuffle_tiles_in_bag(TileBag *bag)
{
    if (!bag || bag->tiles_remaining <= 1)
    {
        return;
    }

    for (int i = bag->tiles_remaining - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        Tile temp = bag->tiles[i];
        bag->tiles[i] = bag->tiles[j];
        bag->tiles[j] = temp;
    }
}

Tile taking_out_tile_from_bag(TileBag *bag)
{
    Tile empty_tile = {'\0', 0};

    if (!bag || bag->tiles_remaining <= 0)
    {
        return empty_tile;
    }

    bag->tiles_remaining--;

    return bag->tiles[bag->tiles_remaining];
}