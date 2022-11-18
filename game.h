#ifndef HUMANSVSBEAST_GAME_H
#define HUMANSVSBEAST_GAME_H

#include "player.h"
#include "map.h"


typedef struct Game {
    Map *map;
    Player *players[4];
    unsigned int player_count;
} Game;

Game *create_game(Map *map);

void destroy_game(Game **game);

void player_move(Game *game, unsigned int player_id, int key);

#endif //HUMANSVSBEAST_GAME_H
