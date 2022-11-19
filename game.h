#ifndef HUMANSVSBEAST_GAME_H
#define HUMANSVSBEAST_GAME_H

#include <pthread.h>
#include "player.h"
#include "map.h"
#include <stdbool.h>

typedef struct Game {
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

void display_non_static_game_info(Game *game);

void display_static_game_info(Game *game);

void display_game_info(Game *game);

void display_game_legend(Game *game);


#endif //HUMANSVSBEAST_GAME_H
