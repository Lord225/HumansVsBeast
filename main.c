#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "map.h"
#include "player.h"
#include "game.h"


int main(int argc, char *argv[]) {



    MAP *map = load_map();

    GAME *game = create_game(map);

//    display_map(map);

    PLAYER *player1 = create_player(&game->player_count, get_random_free_location(map));

    game->players[0] = player1;


    initscr(); // init screen
    raw();
    noecho();
    curs_set(0);
    start_color();

    display_map_ncurses(map);

//    start_color();
    init_pair(PLAYER_COLOR, COLOR_WHITE, COLOR_MAGENTA);

    attron(COLOR_PAIR(1));
    mvprintw(player1->spawn_point.y, player1->spawn_point.x, "1");
    attroff(COLOR_PAIR(1));
    refresh();

    keypad(stdscr, TRUE);

    int key = ' ';

    while (key != 'q') {
        key = getch();

        player_move(game, 0, key);

    }



    endwin(); // end screen

    destroy_game(&game);

    return 0;
}

