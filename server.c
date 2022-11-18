#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "map.h"
#include "player.h"
#include "game.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 9002

int main(int argc, char *argv[]) {


    MAP *map = load_map();

    GAME *game = create_game(map);

//    display_map(map);

    PLAYER *player1 = create_player(&game->player_count, get_random_free_location(map));

    game->players[game->player_count++] = player1;


    initscr(); // init screen
//    raw();
    noecho();
    cbreak();
    curs_set(0);
    refresh();

    start_color();

    display_map_ncurses(map);
    refresh();
//    start_color();
    init_pair(PLAYER_COLOR, COLOR_WHITE, COLOR_MAGENTA);

    attron(COLOR_PAIR(PLAYER_COLOR));
    mvprintw(player1->spawn_point.y, player1->spawn_point.x, "%d", 1);
    attroff(COLOR_PAIR(1));
    refresh();
//

    keypad(stdscr, TRUE);

    // #################### SOCKETS ####################


    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sfd) {
        perror("socket");
        endwin();
        return -1;
    }

    // define server address
    struct sockaddr_in server_info = {0};
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr.s_addr = INADDR_ANY;

    socklen_t server_info_len = sizeof(server_info);

    int opt = 1;
    if(0 > setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        endwin();
        return -1;
    }

    // bind socket to address
    if (0 > bind(sfd, (struct sockaddr *) &server_info, server_info_len)) {
        perror("bind");
        endwin();
        return -1;
    }

    // listen for connections
    if (0 > listen(sfd, 0)) {
        perror("listen");
        endwin();
        return -1;
    }

    // define client address
    struct sockaddr_in client_info = {0};
    socklen_t client_info_len = sizeof(client_info);

    // accept connection
    int cfd = accept(sfd, (struct sockaddr *) &client_info, &client_info_len);
    if (0 > cfd) {
        perror("accept");
        endwin();
        return -1;
    }


    // close socket



    // #################### SOCKETS ####################

    int key = ' ';

    while (tolower(key) != 'q') {

        ssize_t recvd = recv(cfd, &key, sizeof(key) - 1, 0);

        player_move(game, 0, key);
        refresh();
    }

    close(cfd);

    endwin(); // end screen

    destroy_game(&game);

    return 0;
}

