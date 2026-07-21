#include "bag.h"
#ifndef PLAYER_H // header protection
#define PLAYER_H

#define RACK_SIZE 7
typedef struct Player
{
    Tile rack[RACK_SIZE];
    int rack_count;
    int score;
} Player;

void init_player(Player *player);               // initialize player at the starting of the game
void refill_rack(Player *player, TileBag *bag); // check the bag and refill the rack after each turn

#endif