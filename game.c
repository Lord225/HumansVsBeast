#include "game.h"
#include <stdlib.h>
#include "map.h"
#include "player.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


Game *create_game(Map *map) {
    Game *game = malloc(sizeof(Game));

    game->pid = getpid();
    game->round_number = 0;
    game->map = map;

    game->player_count = 0;
    pthread_mutex_init(&game->players_mutex, NULL);

    return game;
}

void display_gamee_info(Game *game) {

//    int maxx=getmaxx(stdscr);
//    int maxy=getmaxy(stdscr);
//
//    for(int i=0; i<maxy; i++){
//        for(int j=0; j<maxx; j++){
//            mvprintw(i, j, "");
//        }
//    }

    display_map_ncurses(game->map);

    int start_column = game->map->width + 3;
    int start_row = 1;

    mvprintw(start_row++, start_column, "Server's PID: %d", game->pid);
    mvprintw(start_row++, start_column + 1, "Campsite X/Y: %d/%d", game->campsite_location.x,
             game->campsite_location.y);
    mvprintw(start_row++, start_column + 1, "Round number: %d", game->round_number);

    mvprintw(start_row++, start_column, "Parameter:   Player1  Player2  Player3  Player4");
    mvprintw(start_row++, start_column + 1, "----------------------------------------------------------");
    mvprintw(start_row++, start_column + 1, "----------------------------------------------------------");
    mvprintw(start_row++, start_column + 1, "----------------------------------------------------------");
    start_row += 2;
    mvprintw(start_row++, start_column + 1, "Coins");
    mvprintw(start_row++, start_column + 1, "----------------------------------------------------------");
    mvprintw(start_row++, start_column + 1, "----------------------------------------------------------");
    start_row += 2;
    mvprintw(start_row++, start_column, "Legend:");
    mvprintw(start_row++, start_column + 1, "1234 - players");
    mvprintw(start_row++, start_column + 1, "?    - wall");
    mvprintw(start_row++, start_column + 1, "#    - bushes (slow down)");
    mvprintw(start_row++, start_column + 1, "*    - wild beast");
    mvprintw(start_row++, start_column + 1, "c    - one coin                  D - dropped treasure");
    mvprintw(start_row++, start_column + 1, "t    - treasure (10 coins)");
    mvprintw(start_row++, start_column + 1, "T    - large treasure (50 coins)");
    mvprintw(start_row++, start_column + 1, "A    - campsite");


}

void destroy_game(Game **game) {
    if (!game || !*game) {
        return;
    }

    destroy_map(&(*game)->map);

    for(size_t i = 0; i < (*game)->player_count; i++) {
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
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_DOWN:
            if (game->map->fields[player->current_location.y + 1][player->current_location.x].tile != WALL) {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.y += 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_LEFT:
            if (game->map->fields[player->current_location.y][player->current_location.x - 1].tile != WALL) {
                mvprintw(player->current_location.y, player->current_location.x, " ");
                player->current_location.x -= 1;
                attron(COLOR_PAIR(PLAYER_COLOR));
                mvprintw(player->current_location.y, player->current_location.x, "%d", player_id+1);
                attroff(COLOR_PAIR(PLAYER_COLOR));
            }
            break;
        case KEY_RIGHT:
            if (game->map->fields[player->current_location.y][player->current_location.x + 1].tile != WALL) {
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
