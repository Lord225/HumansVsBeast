#ifndef HUMANSVSBEAST_GAME_H
#define HUMANSVSBEAST_GAME_H

#include <pthread.h>
#include "player.h"
#include "map.h"
#include <stdbool.h>
#include "beast.h"

#define MAX_PLAYERS 4
#define MAX_BEASTS 10

typedef struct Game {
    int round_number;
    Map *map;
    Location campsite_location;
    Player *players[MAX_PLAYERS];
    Beast *beasts[MAX_BEASTS];
    int player_count;
    int beast_count;
    pthread_mutex_t game_mutex;
} Game;

Game *create_game(Map *map);

Location find_campsite_location(Map *map);

void destroy_game(Game **game);

void display_non_static_game_info(Game *game);

void display_static_game_info(Game *game);

void display_game_legend(Game *game);

int find_free_player_slot(Game *game);

void display_players_on_map(Game *game);

void player_move(Game *game, Player *player);

void handle_player_map_interaction(Game *game, Player *player);

void kill_and_respawn_dead_players(Game *game);

void move_players(Game *game);

int validate_player_move(Map *map, Player *player, Location new_location);

int send_map_data_to_player(Game *game, Player *player);

int send_map_data_to_all_players(Game *game);

void disconnect_players(Player *players[]);

void disconnect_player(Player *player);

Location get_random_free_location(Game *game);

int is_player_on(Game *game, int x, int y);

int is_beast_on(Game *game, int x, int y);

int is_entity_on(Game *game, int x, int y);

#endif //HUMANSVSBEAST_GAME_H
