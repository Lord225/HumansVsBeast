//
// Created by Mateusz on 08.12.2022.
//

#include "beast_behaviour.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"


Beast *add_new_beast(Game *game) {
    if (game->beast_count < MAX_BEASTS) {
        Location location = get_random_free_location(game);
        Beast *beast = create_beast(location);
        game->beasts[game->beast_count] = beast;
        game->beast_count++;
        return beast;
    }

    return NULL;
}

void display_beasts_on_map(Game *game) {
    attron(COLOR_PAIR(BEAST_COLOR));
    for (int i = 0; i < game->beast_count; i++) {
        mvprintw(game->beasts[i]->current_location.y, game->beasts[i]->current_location.x, "*");
    }
    attroff(COLOR_PAIR(BEAST_COLOR));
}

void move_beasts(Game *game) {
    for (int i = 0; i < game->beast_count; i++) {
        move_beast(game, game->beasts[i]);
    }
}

void move_beast(Game *game, Beast *beast) {

    Location new_location = beast->current_location;

    if (beast->is_stunned) {
        beast->is_stunned = 0;
        return;
    }

    switch (beast->direction) {
        case KEY_UP:
            new_location.y -= 1;

            break;
        case KEY_DOWN:
            new_location.y += 1;

            break;
        case KEY_LEFT:

            new_location.x -= 1;

            break;
        case KEY_RIGHT:

            new_location.x += 1;

            break;
        default:
            break;
    }

    int is_valid_move = validate_beast_move(game->map, beast, new_location);
    if (is_valid_move) {
        beast->current_location = new_location;
    }


}

int validate_beast_move(Map *map, Beast *beast, Location new_location) {

    if (map->fields[new_location.y][new_location.x].tile == WALL) {
        return 0;
    }
    return 1;
}


void move_beast_towards_player(Game *game, Beast *beast) {

    bool is_visited[game->map->height * game->map->width];
    for (int i = 0; i < game->map->height * game->map->width; i++) {
        is_visited[i] = false;
    }
//    is_visited[beast->current_location.y * game->map->width + beast->current_location.x] = true;
    Payload move = move_beast_towards_player_recurrent(game, beast->current_location, is_visited, 6,
                                                       beast->current_location);
    printw("move: %d", move.cost);


    switch (move.dir) {
        case 0:
            beast->direction = KEY_UP;
            break;
        case 1:
            beast->direction = KEY_DOWN;
            break;
        case 2:
            beast->direction = KEY_LEFT;
            break;
        case 3:
            beast->direction = KEY_RIGHT;
            break;
        default:
//            beast->direction = get_random_direction();
            beast->direction = 0;
            break;
    }


}


Payload move_beast_towards_player_recurrent(Game *game, Location location, bool *is_visited, int recurrent_limit,
                                            Location orginal_location) {
    // if recurent depth is 0 return
    if (recurrent_limit == 0) {
        return (Payload) {10000000, -1};
    }

    // mark current location as visited
    is_visited[location.x + location.y * game->map->width] = true;
    Payload best = (Payload) {1000000, -1};
    // for each direction
    for (int i = 0; i < 4; i++) {
//        int ir = rand() % 4;
        int ir = i;
        // calculate new position
        Location new_location = location;
        switch (ir) {
            case 0:
                new_location.y -= 1;
                break;
            case 1:
                new_location.y += 1;
                break;
            case 2:
                new_location.x -= 1;
                break;
            case 3:
                new_location.x += 1;
                break;
            default:
                break;

        }
        if (is_visited[new_location.x + new_location.y * game->map->width]) {
            return (Payload) {1000000, -1};
        } else {
            is_visited[new_location.x + new_location.y * game->map->width] = true;
        }
        // if new position is player return direction
        if (is_player_on(game, new_location.x, new_location.y)) {
            return (Payload) {0, ir};
        }
//        if (is_outside_view(new_location, orginal_location)) {
//            return {1000000, -1};
//        }

        // if new position is not visited and is not wall
        if (game->map->fields[new_location.y][new_location.x].tile != WALL) {
            // call function recursively
            Payload new_cost = move_beast_towards_player_recurrent(game, new_location, is_visited, recurrent_limit - 1,
                                                                   orginal_location);
            if (new_cost.cost < best.cost) {
                best = new_cost;
            }

        }

    }
    best.cost += 1;

    return best;
}

int is_outside_view(Location new_location, Location orginal_location) {
    if (abs(new_location.x - orginal_location.x) > 2 || abs(new_location.y - orginal_location.y) > 2) {
        return 1;
    }
    return 0;
}