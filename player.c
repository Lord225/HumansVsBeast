#include "player.h"
#include <stdlib.h>
#include <stdio.h>
//#include "map.h"
#include <stdbool.h>
#include <unistd.h>


Player *create_player(int free_slot, Location spawn_point) {
    Player *player = calloc(1,sizeof(Player));
    if (!player) {
        printf("Error: Can't allocate memory for player");
        exit(1);
    }

    player->id = free_slot;
//    *player_count += 1;


//    player->deaths = 0;
//    player->coins_found = 0;
//    player->coins_brought = 0;
//    player->is_stunned = 0;

    player->spawn_point = player->current_location = spawn_point;

    player->was_key_sent_last_turn=false;

    return player;
}

void destroy_player(Player **player) {

    if (!player || !*player) {
        return;
    }

    pthread_join((*player)->thread, NULL);
    close((*player)->cfd);

    if(*player) {
        free(*player);
        *player = NULL;
    }

}
