#ifndef HUMANSVSBEAST_GAME_H
#define HUMANSVSBEAST_GAME_H

#include <pthread.h>
#include "player.h"
#include "map.h"
#include <stdbool.h>

#define MAX_PLAYERS 4

typedef struct Game {
    int round_number;
    Map *map;
    Location campsite_location;
    Player *players[MAX_PLAYERS];
    int player_count;
    pthread_mutex_t game_mutex;
} Game;

Game *create_game(Map *map);

void destroy_game(Game **game);



void display_non_static_game_info(Game *game);

void display_static_game_info(Game *game);

void display_game_info(Game *game);

void display_game_legend(Game *game);

int find_free_player_slot(Game *game);

void spawn_player(Game *game, int player_id);

void display_players_on_map(Game *game);

void player_move(Map *map, Player *player);

void move_players(Game *game);

int validate_player_move(Map *map, Player *player, Location new_location);

int send_map_data_to_player(Game *game, Player *player);

int send_map_data_to_all_players(Game *game);

#endif //HUMANSVSBEAST_GAME_H
