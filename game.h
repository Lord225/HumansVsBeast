#ifndef HUMANSVSBEAST_GAME_H
#define HUMANSVSBEAST_GAME_H

#include <pthread.h>
#include "player.h"
#include "map.h"
#include <stdbool.h>

#define MAX_PLAYERS 4

typedef struct Game {
    unsigned int round_number;
    Map *map;
    Location campsite_location;
    Player *players[MAX_PLAYERS];
    unsigned int player_count;
    pthread_mutex_t game_mutex;
} Game;

Game *create_game(Map *map);

void destroy_game(Game **game);



void display_non_static_game_info(Game *game);

void display_static_game_info(Game *game);

void display_game_info(Game *game);

void display_game_legend(Game *game);

unsigned int find_free_player_slot(Game *game);

void spawn_player(Game *game, unsigned int player_id);

void display_players_on_map(Game *game);

void player_move(Map *map, Player *player);

void move_players(Game *game);

int validate_player_move(Map *map, Player *player, Location new_location);

#endif //HUMANSVSBEAST_GAME_H
