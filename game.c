#include "game.h"
#include <stdlib.h>
#include "map.h"
#include "player.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


Game *create_game(Map *map) {
    Game *game = malloc(sizeof(Game));
    game->map = map;
    game->player_count = 0;

    return game;
}

void destroy_game(Game **game) {
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

void player_move(Game *game, unsigned int player_id, int key) {

    Player *player = game->players[player_id];

    switch (key) {
        case KEY_UP:
            if (game->map->map[player->current_location.y - 1][player->current_location.x] != '?') {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.y -= 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_DOWN:
            if (game->map->map[player->current_location.y + 1][player->current_location.x] != '?') {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.y += 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_LEFT:
            if (game->map->map[player->current_location.y][player->current_location.x - 1] != '?') {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.x -= 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_RIGHT:
            if (game->map->map[player->current_location.y][player->current_location.x + 1] != '?') {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.x += 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        default:
            break;
    }

}
