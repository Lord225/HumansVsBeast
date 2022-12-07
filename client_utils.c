//
// Created by Mateusz on 07.12.2022.
//

#include "client_utils.h"
#include "common.h"
#include "ncurses.h"
#include "utils.h"

void display_player_map(PlayerSight *player_sight) {
//    PlayerSight *player_sight = (PlayerSight *) arg;

    for(int i=0;i<player_sight->radius;i++) {
        for(int j=0;j<player_sight->radius;j++) {

            mvprintw(player_sight->cord_y-player_sight->radius/2+i,player_sight->cord_x-player_sight->radius/2+j, "%c", player_sight->fields[i][j].tile);
        }
    }


//    return NULL;
}

void display_stats(ServerInfoForPlayer *server_info_for_player) {

    int column = server_info_for_player->map_width+3;
    int row = 1;
    mvprintw(row++, column, "Server's PID: %d", server_info_for_player->server_pid);
    mvprintw(row++, column + 1, "Campsite X/Y:");
    mvprintw(row++, column + 1, "Round number:");
    row += 1;
    mvprintw(row++, column, "Player");
    mvprintw(row++, column + 1, "Number:     %d", server_info_for_player->player_number);
    mvprintw(row++, column + 1, "Type:       %s","HUMAN");
    mvprintw(row++, column + 1, "Curr X/Y    %d/%d", server_info_for_player->player_sight.cord_x, server_info_for_player->player_sight.cord_y);
    mvprintw(row++, column + 1, "Deaths      %d", server_info_for_player->deaths);
    row += 1;
    mvprintw(row++, column + 1, "Coins");
    mvprintw(row++, column + 5, "carried %d", server_info_for_player->coins_found);
    mvprintw(row++, column + 5, "brought %d",server_info_for_player->coins_brought);

    display_game_legend(server_info_for_player->map_width, server_info_for_player->map_height);
}

void display_game_legend(int column, int row) {
    row = row - 9;
    column = column + 3;

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
