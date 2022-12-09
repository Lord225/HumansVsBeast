#ifndef HUMANSVSBEAST_BEAST_BEHAVIOUR_H
#define HUMANSVSBEAST_BEAST_BEHAVIOUR_H

#include "game.h"
#include "beast.h"

Beast *add_new_beast(Game *game);

void display_beasts_on_map(Game *game);

void move_beast(Game *game, Beast *beast);

void move_beasts(Game *game);

int validate_beast_move(Map *map, Beast *beast, Location new_location);

int beast_ai(Game *game, Beast *beast);


#endif //HUMANSVSBEAST_BEAST_BEHAVIOUR_H
