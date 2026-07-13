#include "bag.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

bool init_bag_from_file(TileBag *bag)
{
    FILE *file = fopen("letters.txt", "r");
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

// tiles_remaining = 100
// Fisher-Yates Shuffle algorithm
void shuffle_tiles_in_bag(TileBag *bag)
{
    if (bag->tiles_remaining <= 1)
    {
        return; // for 1 tile we don't need to shuffle
    }
    
    // continuously change the random number
    for (int i = bag->tiles_remaining - 1; i > 0; i--) // loop will run for all the remaining tiles
    {
        // generating a random index between 0 and i
        int j = rand() % (i + 1);

        Tile temp = bag->tiles[i];
        bag->tiles[i] = bag->tiles[j];
        bag->tiles[j] = temp;
    }
}

// tiles_remaining = 100

Tile taking_out_tile_from_bag(TileBag *bag)
{
    Tile empty_tile = {'\0', 0};

    if (bag->tiles_remaining <= 0)
    {
        return empty_tile;
    }

    bag->tiles_remaining--;

    return bag->tiles[bag->tiles_remaining];
}