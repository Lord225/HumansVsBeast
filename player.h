#ifndef HUMANSVSBEAST_PLAYER_H
#define HUMANSVSBEAST_PLAYER_H

#include "map.h"

typedef struct Player {
    unsigned int id;

    LOCATION current_location;
    LOCATION spawn_point;

    unsigned int deaths;
    unsigned int coins_found;
    unsigned int coins_brought;

} Player;



Player *create_player(unsigned int *player_count, LOCATION spawn_point);

void destroy_player(Player **player);



#endif //HUMANSVSBEAST_PLAYER_H
