#include "player.h"
#include "bag.h"
#include "error_service.h"
#include <stdio.h>

void init_player(Player *player)
{
    if (player == NULL)
    {
        ReportCriticalError("Invalid Player State", "NULL Player pointer encountered while initializing Players.");
        return;
    }

    player->rack_count = 0;
    player->score = 0;

    for (int i = 0; i < RACK_SIZE; i++)
    {
        player->rack[i].letter = '\0';
        player->rack[i].value = 0;
    }
}

void refill_rack(Player *player, TileBag *bag)
{
    for (int i = 0; i < RACK_SIZE; i++)
    {
        if (player->rack[i].letter == '\0')
        {
            Tile new_tile = taking_out_tile_from_bag(bag);
            if (new_tile.letter != '\0')
            {
                player->rack[i] = new_tile;
                player->rack_count++;
            }
        }
    }
}