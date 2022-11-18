#ifndef HUMANSVSBEAST_GAME_H
#define HUMANSVSBEAST_GAME_H

#include "map.h"
#include "player.h"

typedef struct GAME {
    MAP *map;
    PLAYER *players[4];
    unsigned int player_count;
} GAME;

GAME *create_game(MAP *map);

void destroy_game(GAME **game);

void player_move(GAME *game, unsigned int player_id, int key);

#endif //HUMANSVSBEAST_GAME_H
