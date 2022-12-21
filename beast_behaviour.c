#include "beast_behaviour.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "utils.h"
#include "dfs.h"
#include "game.h"

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
    if (game->map->fields[beast->current_location.y][beast->current_location.x].tile == BUSH) {
        beast->is_stunned = 1;
    }

    for(int i=0;i<MAX_PLAYERS; i++) {
        if(game->players[i]) {
            if(game->players[i]->current_location.x == beast->current_location.x && game->players[i]->current_location.y == beast->current_location.y) {
                game->players[i]->is_dead = 1;
            }
        }
    }


}

int validate_beast_move(Map *map, Beast *beast, Location new_location) {

    if (map->fields[new_location.y][new_location.x].tile == WALL) {
        return 0;
    }
    return 1;
}


int beast_ai(Game *game, Beast *beast) {

    int mat[PLAYER_SIGHT][PLAYER_SIGHT];
    for (int i = 0; i < PLAYER_SIGHT; i++) {
        for (int j = 0; j < PLAYER_SIGHT; j++) {
            int y = beast->current_location.y - PLAYER_SIGHT / 2 + i;
            int x = beast->current_location.x - PLAYER_SIGHT / 2 + j;
            if (y < 0 || y >= game->map->height || x < 0 || x >= game->map->width) {
                mat[i][j] = 1;
            } else if (game->map->fields[y][x].tile == WALL) {
                mat[i][j] = 1;
            } else {
                mat[i][j] = 0;
            }

        }
    }

    struct point src = {PLAYER_SIGHT / 2, PLAYER_SIGHT / 2};

    int nearest_player = INT_MAX;
    int min = INT_MAX;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i]) {
            if (abs(game->players[i]->current_location.x - beast->current_location.x) <= PLAYER_SIGHT / 2 &&
                abs(game->players[i]->current_location.y - beast->current_location.y) <= PLAYER_SIGHT / 2) {
                struct point dst = {game->players[i]->current_location.y - beast->current_location.y + PLAYER_SIGHT / 2,
                                    game->players[i]->current_location.x - beast->current_location.x +
                                    PLAYER_SIGHT / 2};
                int dist = findShortestPathLength(mat, src, dst);
                if (dist < min) {
                    min = dist;
                    nearest_player = i;
                }
            }
        }
    }

    int new_dir = -2;
    if (nearest_player == INT_MAX) {
        return get_random_direction();
    }

    int min_dir = INT_MAX;
    int dist;


    if (nearest_player != INT_MAX) {
        struct point dst = {
                game->players[nearest_player]->current_location.y - beast->current_location.y + PLAYER_SIGHT / 2,
                game->players[nearest_player]->current_location.x - beast->current_location.x + PLAYER_SIGHT / 2};

        src.first = PLAYER_SIGHT / 2 - 1;
        src.second = PLAYER_SIGHT / 2;

        dist = findShortestPathLength(mat, src, dst);
        if (dist < min_dir && dist != -1) {
            min_dir = dist;
            new_dir = KEY_UP;
        }

        src.first = PLAYER_SIGHT / 2 + 1;
        src.second = PLAYER_SIGHT / 2;

        dist = findShortestPathLength(mat, src, dst);
        if (dist < min_dir && dist != -1) {
            min_dir = dist;
            new_dir = KEY_DOWN;
        }

        src.first = PLAYER_SIGHT / 2;
        src.second = PLAYER_SIGHT / 2 - 1;

        dist = findShortestPathLength(mat, src, dst);
        if (dist < min_dir && dist != -1) {
            min_dir = dist;
            new_dir = KEY_LEFT;
        }

        src.first = PLAYER_SIGHT / 2;
        src.second = PLAYER_SIGHT / 2 + 1;

        dist = findShortestPathLength(mat, src, dst);
        if (dist < min_dir && dist != -1) {
            min_dir = dist;
            new_dir = KEY_RIGHT;
        }

    }

    if (new_dir < 0) {
        return get_random_direction();
    }
    return new_dir;

}
