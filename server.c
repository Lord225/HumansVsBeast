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

#define PORT 9002

typedef struct Server {
    bool server_running;
    int sfd;
    Game *game;
} Server;


void *connection_handlig(void *arg) {
    Server args = *(Server *) arg;

    int response;
    while (args.server_running) {


        // define client address
        struct sockaddr_in client_info = {0};

        // accept connection
        int cfd = accept(args.sfd, (struct sockaddr *) &client_info, (socklen_t *) sizeof(client_info));
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

int main(void) {

    int sfd = create_socket(PORT);
    if (0 > sfd) {
        return -1;
    }



    Map *map = load_map();

    Game *game = create_game(map);

//    ConnHandlingArgs args = {sfd, game};

    Server server = {true, sfd, game};

    pthread_t threadConnHandling;

    pthread_create(&threadConnHandling, NULL, connection_handlig, &server);

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
    init_pair(MONEY_COLOR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CAMP_COLOR, COLOR_WHITE, COLOR_GREEN);
    init_pair(BEAST_COLOR, COLOR_WHITE, COLOR_RED);

//    attron(COLOR_PAIR(PLAYER_COLOR));
//    mvprintw(player1->spawn_point.y, player1->spawn_point.x, "%d", 1);
//    attroff(COLOR_PAIR(1));
//    refresh();
//

    keypad(stdscr, TRUE);


    display_static_game_info(game);
    display_game_legend(game);

    while (server.server_running) {
        display_non_static_game_info(game);
        display_map(game->map);


        refresh();

        sleep(1);
        game->round_number+=1;
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

