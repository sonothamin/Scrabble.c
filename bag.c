#include "bag.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool init_bag_from_file(TileBag *bag)
{
    FILE *file = fopen("letters.txt", "r");
    if(file == NULL)
    {
        printf("Could not open the letters file\n");
        return false;
    }
    bag -> tiles_remaining = 0;
    char letter;
    int quantity;
    int value;

    while((fscanf(file, " %c %d %d", &letter, &quantity, &value)) == 3 )
    {
        for(int i = 0; i < quantity; i++)
        {
            if(bag -> tiles_remaining >= MAX_BAG_TILES)
            {
                break;
            }
            bag -> tiles[bag -> tiles_remaining].letter = letter;
            bag -> tiles[bag -> tiles_remaining].value = value;
            bag -> tiles_remaining++;
        }
    }
    fclose(file);
    return true;
}