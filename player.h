#ifndef HUMANSVSBEAST_PLAYER_H
#define HUMANSVSBEAST_PLAYER_H

#include "map.h"
#include <pthread.h>

typedef struct Player {
    int id;
    int pid;
    int cfd;
    Location current_location;
    Location spawn_point;

    int deaths;
    int coins_found;
    int coins_brought;

    int last_key;
    int was_key_sent_last_turn;
    int is_stunned;
    int turn_passed;

    pthread_t thread;
//    pthread_mutex_t player_mutex;

} Player;


Player *create_player(int free_slot, Location spawn_point);

void destroy_player(Player **player);





#endif //HUMANSVSBEAST_PLAYER_H
