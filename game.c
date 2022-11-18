#include "game.h"
#include <stdlib.h>
#include "map.h"
#include "player.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


GAME *create_game(MAP *map) {
    GAME *game = malloc(sizeof(GAME));
    game->map = map;
    game->player_count = 0;

    return game;
}

void destroy_game(GAME **game) {
    if (!game || !*game) {
        return;
    }

    destroy_map(&(*game)->map);

    for(size_t i = 0; i < (*game)->player_count; i++) {
        destroy_player(&(*game)->players[i]);
    }

    free(*game);
    *game = NULL;
}

void player_move(GAME *game, unsigned int player_id, int key) {

    PLAYER *player = game->players[player_id];

    switch (key) {
        case KEY_UP:
            if (game->map->map[player->current_location.y - 1][player->current_location.x] != '@') {
                player->current_location.y -= 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y + 1, player->current_location.x, " ");
            }
            break;
        case KEY_DOWN:
            if (game->map->map[player->current_location.y + 1][player->current_location.x] != '@') {
                player->current_location.y += 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y - 1, player->current_location.x, " ");

            }
            break;
        case KEY_LEFT:
            if (game->map->map[player->current_location.y][player->current_location.x - 1] != '@') {
                player->current_location.x -= 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x + 1, " ");

            }
            break;
        case KEY_RIGHT:
            if (game->map->map[player->current_location.y][player->current_location.x + 1] != '@') {
                player->current_location.x += 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x - 1, " ");
            }
            break;
        default:
            break;
    }

}
