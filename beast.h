#ifndef HUMANSVSBEAST_BEAST_H
#define HUMANSVSBEAST_BEAST_H

#include <pthread.h>
#include "map.h"

typedef struct Beast {
    Location current_location;
    int direction;
    int is_stunned;
    pthread_t thread;
} Beast;

Beast *create_beast(Location spawn_point);

int get_random_direction(void);


void destroy_beast(Beast **beast);



#endif //HUMANSVSBEAST_BEAST_H
