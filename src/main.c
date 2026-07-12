#include <stdio.h>
#include "bag.h"
#include "player.h"

int main()
{
    TileBag game_bag;
    // initialize random seed 
    srand(time(NULL));

    if(!init_bag_from_file(&game_bag))
    {
        return -1; // safe exit if it cannot read from the letters.txt
    }
    shuffle_tiles_in_bag(&game_bag);
    
}