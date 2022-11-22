#include "game.h"
#include <stdlib.h>
#include "map.h"
#include "player.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "utils.h"


Game *create_game(Map *map) {
    Game *game = malloc(sizeof(Game));
    game->round_number = 0;
    game->map = map;

    game->player_count = 0;
    for (int i = 0; i < 4; i++) {
        game->players[i] = NULL;
    }
    pthread_mutex_init(&game->players_mutex, NULL);

    return game;
}

void display_game_legend(Game *game) {
    int row = game->map->height - 9;
    int column = game->map->width + 3;

    mvprintw(row++, column, "Legend:");
    attron(COLOR_PAIR(PLAYER_COLOR));
    mvprintw(row, column + 1, "1234");
    attroff(COLOR_PAIR(PLAYER_COLOR));
    mvprintw(row++, column + 6, "- players");
    attron(A_REVERSE);
    mvprintw(row, column + 1, " ");
    attroff(A_REVERSE);
    mvprintw(row++, column + 6, "- wall");
    mvprintw(row++, column + 1, "#    - bushes (slow down)");
    attron(COLOR_PAIR(BEAST_COLOR));
    mvprintw(row, column + 1, "*");
    attroff(COLOR_PAIR(BEAST_COLOR));
    mvprintw(row++, column + 6, "- wild beast");
    attron(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 1, "c");
    attroff(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 6, "- one coin");
    attron(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 27, "D");
    attroff(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row++, column + 29, "- dropped treasure");
    attron(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 1, "t");
    attroff(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row++, column + 6, "- treasure (10 coins)");
    attron(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 1, "T");
    attroff(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row++, column + 6, "- large treasure (50 coins)");
    attron(COLOR_PAIR(CAMP_COLOR));
    mvprintw(row, column + 1, "A");
    attroff(COLOR_PAIR(CAMP_COLOR));
    mvprintw(row, column + 6, "- campsite");
}

void display_non_static_game_info(Game *game) {

    int column = game->map->width + 3;
    int row = 1;
    row += 1;
    mvprintw(row++, column + 1 + strlen("Campsite X/Y: "), "%d/%d", game->campsite_location.x,
             game->campsite_location.y);
    mvprintw(row++, column + 1, "Round number: %d", game->round_number);
    row += 2;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d", game->players[i]->pid);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-");
        }
    }
    row += 1;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%s", "HUMAN");
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-");
        }
    }
    row += 1;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d/%d", game->players[i]->current_location.x,
                     game->players[i]->current_location.y);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-/-");
        }
    }
    row += 1;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d", game->players[i]->deaths);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-");
        }
    }

    row += 3;

    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d", game->players[i]->coins_found);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-");
        }
    }

    row += 1;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d", game->players[i]->coins_brought);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-");
        }
    }

}

void display_static_game_info(Game *game) {
    int column = game->map->width + 3;
    int row = 1;
    mvprintw(row++, column, "Server's PID: %d", getpid());
    mvprintw(row++, column + 1, "Campsite X/Y:");
    mvprintw(row++, column + 1, "Round number:");
    row += 1;
    mvprintw(row++, column, "Parameter:   Player1  Player2  Player3  Player4");
    mvprintw(row++, column + 1, "PID");
    mvprintw(row++, column + 1, "Type");
    mvprintw(row++, column + 1, "Curr X/Y");
    mvprintw(row++, column + 1, "Deaths");
    row += 1;
    mvprintw(row++, column + 1, "Coins");
    mvprintw(row++, column + 5, "carried");
    mvprintw(row++, column + 5, "brought");

}

void destroy_game(Game **game) {
    if (!game || !*game) {
        return;
    }

    destroy_map(&(*game)->map);

    for (size_t i = 0; i < (*game)->player_count; i++) {
        destroy_player(&(*game)->players[i]);
    }

    pthread_mutex_destroy(&(*game)->players_mutex);

    free(*game);
    *game = NULL;
}

void player_move(Game *game, unsigned int player_id, int key) {

    Player *player = game->players[player_id];

    switch (key) {
        case KEY_UP:
            if (game->map->fields[player->current_location.y - 1][player->current_location.x].tile != WALL) {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.y -= 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id + 1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_DOWN:
            if (game->map->fields[player->current_location.y + 1][player->current_location.x].tile != WALL) {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.y += 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id + 1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_LEFT:
            if (game->map->fields[player->current_location.y][player->current_location.x - 1].tile != WALL) {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.x -= 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id + 1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_RIGHT:
            if (game->map->fields[player->current_location.y][player->current_location.x + 1].tile != WALL) {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.x += 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id + 1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        default:
            break;
    }

}

unsigned int find_free_player_slot(Game *game) {
    for (size_t i = 0; i < 4; i++) {
        if (!game->players[i]) {
            return i;
        }
    }
    return -1;
}
