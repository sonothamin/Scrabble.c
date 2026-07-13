#include "bag.h"
#ifndef PLAYER_H
#define PLAYER_H

#define RACK_SIZE 7
typedef struct Player
{
    Tile rack[RACK_SIZE];
    int rack_count;
    int score;
} Player;

void init_player(Player *player);
void refill_rack(Player *player, TileBag *bag);

#endif