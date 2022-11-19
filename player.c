#include "player.h"
#include <stdlib.h>
#include <stdio.h>
//#include "map.h"


Player *create_player(unsigned int free_slot, Location spawn_point) {
    Player *player = malloc(sizeof(Player));
    if (!player) {
        printf("Error: Can't allocate memory for player");
        exit(1);
    }

    player->id = free_slot;
//    *player_count += 1;


    player->deaths = 0;
    player->coins_found = 0;
    player->coins_brought = 0;

    player->spawn_point = player->current_location = spawn_point;

    return player;
}

void destroy_player(Player **player) {

    if (!player || !*player) {
        return;
    }

    free(*player);
    *player = NULL;
}


