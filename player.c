#include "player.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "map.h"
#include <time.h>

PLAYER *create_player(unsigned int *player_count, LOCATION spawn_point) {
    PLAYER *player = malloc(sizeof(PLAYER));
    if (!player) {
        printf("Error: Can't allocate memory for player");
        exit(1);
    }

    player->id = *player_count;
    *player_count += 1;


    player->deaths = 0;
    player->coins_found = 0;
    player->coins_brought = 0;

    player->spawn_point = player->current_location = spawn_point;

    return player;
}

LOCATION get_random_free_location(MAP *map) {

    LOCATION location;

    srand(time(NULL));
    unsigned int x = rand() % map->width;
    unsigned int y = rand() % map->height;


    while (map->map[y][x] != ' ') {
        x = rand() % map->width;
        y = rand() % map->height;
    }

    location.x = x;
    location.y = y;

    return location;
}

void destroy_player(PLAYER **player) {

    if (!player || !*player) {
        return;
    }

    free(*player);
    *player = NULL;
}


