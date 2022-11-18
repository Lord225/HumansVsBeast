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

static bool server_running = true;

typedef struct ConnHandlingArgs {
    int sfd;
    Game *game;
} ConnHandlingArgs;

void *connection_handlig(void *arg) {
    ConnHandlingArgs args = *(ConnHandlingArgs *) arg;

    int response;
    while (server_running) {


        // define client address
        struct sockaddr_in client_info = {0};
        socklen_t client_info_len = sizeof(client_info);

        // accept connection
        int cfd = accept(args.sfd, (struct sockaddr *) &client_info, &client_info_len);
        if (cfd < 0) {
            fprintf(stderr, "[CONN_THERED] [ERROR] accept failed\n");
        } else {
            fprintf(stderr, "[CONN_THERED] [INFO] client connected\n");
            pthread_mutex_lock(&args.game->players_mutex);

            if (args.game->player_count < 4) {

                Player *player = create_player(&args.game->player_count, get_random_free_location(args.game->map));
                args.game->players[args.game->player_count] = player;
                args.game->player_count++;
                response = 0;
            } else {
                response = -1;
            }

            pthread_mutex_unlock(&args.game->players_mutex);

        }
    }
}

int main(int argc, char *argv[]) {

    int sfd = create_socket(PORT);
    if (0 > sfd) {
        return -1;
    }

//




    Map *map = load_map();

    Game *game = create_game(map);

    ConnHandlingArgs args = {sfd, game};

    pthread_t threadConnHandling;

    pthread_create(&threadConnHandling, NULL, connection_handlig, &args);

//    display_map(map);

//    Player *player1 = create_player(&game->player_count, get_random_free_location(map));
//
//    game->players[game->player_count++] = player1;

//  init screen
    initscr();
//    raw();
    noecho();
    cbreak();
    curs_set(0);
    refresh();

    start_color();


    refresh();
//    start_color();
    init_pair(PLAYER_COLOR, COLOR_WHITE, COLOR_MAGENTA);

//    attron(COLOR_PAIR(PLAYER_COLOR));
//    mvprintw(player1->spawn_point.y, player1->spawn_point.x, "%d", 1);
//    attroff(COLOR_PAIR(1));
//    refresh();
//

    keypad(stdscr, TRUE);

    // #################### SOCKETS ####################





    // close socket



    // #################### SOCKETS ####################



    while (server_running) {
        display_gamee_info(game);
        refresh();

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



    endwin(); // end screen

    destroy_game(&game);

    printf("Server closed\n");

    return 0;
}

