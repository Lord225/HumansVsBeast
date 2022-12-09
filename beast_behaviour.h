#ifndef HUMANSVSBEAST_BEAST_BEHAVIOUR_H
#define HUMANSVSBEAST_BEAST_BEHAVIOUR_H

#include "game.h"
#include "beast.h"

typedef struct Payload {
    int cost;
    int dir;
} Payload;



Beast *add_new_beast(Game *game);

void display_beasts_on_map(Game *game);

void move_beast(Game *game, Beast *beast);

void move_beasts(Game *game);

int validate_beast_move(Map *map, Beast *beast, Location new_location);

void move_beast_towards_player(Game *game, Beast *beast);

Payload move_beast_towards_player_recurrent(Game *game, Location location, bool *is_visited, int recurrent_limit,Location orginal_location);

int is_outside_view(Location new_location, Location orginal_location);


#endif //HUMANSVSBEAST_BEAST_BEHAVIOUR_H
