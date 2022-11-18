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
#include "socket.h"
#include <stdbool.h>
#include "pthread.h"
#include "threads.h"

#define PORT 9002

bool server_running = true;

int main(int argc, char *argv[]) {

    int sfd = create_socket(PORT);
    if (0 > sfd) {
        return -1;
    }

    pthread_t connection_handler;

    Map *map = load_map();

    Game *game = create_game(map);

//    display_map(map);

    Player *player1 = create_player(&game->player_count, get_random_free_location(map));

    game->players[game->player_count++] = player1;

//  init screen
    initscr();
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



    while (server_running) {
        display_map(map);



        sleep(1);
    }


//    int key = ' ';
//    while (server_running) {
//
//        pthread_create(&connection_handler, NULL, handle_connection, (void *) &cfd);
//
//        ssize_t recvd = recv(cfd, &key, sizeof(key) - 1, 0);
//        if(key == 'q') {
//            server_running = false;
//        }
//        usleep(500000);
//        player_move(game, 0, key);
//        refresh();
//        int response = 1;
//        send(cfd,&response, sizeof(response), 0);
//    }

    close(cfd);

    endwin(); // end screen

    destroy_game(&game);

    printf("Server closed\n");

    return 0;
}

