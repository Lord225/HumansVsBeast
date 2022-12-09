//
// Created by Mateusz on 08.12.2022.
//

#include "beast.h"
#include <stdlib.h>
#include <ncurses.h>

Beast *create_beast(Location spawn_point) {
    Beast *beast = calloc(1, sizeof(Beast));
    beast->current_location = spawn_point;

    return beast;
}

int get_random_direction(void) {
    srand(time(NULL));
    int direction = rand() % 4;

    int dir;

    switch (direction) {
        case 0:
            dir = KEY_UP;
            break;
        case 1:
            dir = KEY_DOWN;
            break;
        case 2:
            dir = KEY_LEFT;
            break;
        case 3:
            dir = KEY_RIGHT;
            break;
    }

    return dir;
}



void destroy_beast(Beast **beast) {
    if (!beast || !*beast) {
        return;
    }

    free(*beast);
    *beast = NULL;
}





