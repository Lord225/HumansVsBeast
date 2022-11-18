#ifndef HUMANSVSBEAST_GAME_H
#define HUMANSVSBEAST_GAME_H

#include <pthread.h>
#include "player.h"
#include "map.h"


typedef struct Game {
    unsigned int pid;
    unsigned int round_number;
    Map *map;
    Location campsite_location;
    Player *players[4];
    unsigned int player_count;
    pthread_mutex_t players_mutex;
} Game;

Game *create_game(Map *map);

void destroy_game(Game **game);

void player_move(Game *game, unsigned int player_id, int key);

void display_gamee_info(Game *game);

#endif //HUMANSVSBEAST_GAME_H
