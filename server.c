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
#include "utils.h"
#include "common.h"

#define DEBUG 1

#define PORT 9002


typedef struct Server {
    bool server_running;
    int sfd;
    Game *game;
} Server;


void *player_thread(void *arg) {

    Player **p= (Player **) arg;
    Player *player = *p;

    ClientInfo  client_info = {0};
    client_info.is_connected = true;

    while(client_info.is_connected) {

        ssize_t bytes_recived = recv(player->cfd, &client_info, sizeof(ClientInfo), 0);
        if(bytes_recived <= 0) {
            client_info.is_connected = false;
            destroy_player(p);
        }
        player->was_key_sent_last_turn=true;
        player->last_key = client_info.key;

        if(player->last_key=='q') {
            client_info.is_connected = false;
            destroy_player(p);
        }
//        printw("bytes_recived: %d", (int)bytes_recived);
//        player_move(player, client_info.key);

//        send(player->cfd, (void*)1, sizeof(int), 0);
//        printf("player loc y %ud", player->current_location.y);

    }


    return NULL;
}


void *connection_handlig(void *arg) {
    Server args = *(Server *) arg;

    while (args.server_running) {


        // define client address
        struct sockaddr_in client_info = {0};
        socklen_t client_info_len = sizeof(client_info);

        // accept connection
        int cfd = accept(args.sfd, (struct sockaddr *) &client_info, &client_info_len);
        if (cfd < 0) {
//#if DEBUG
//            fprintf(stderr, "[CONN_THERED] [ERROR] accept failed\n");
//#endif
        } else {
//#if DEBUG
//            fprintf(stderr, "[CONN_THERED] [INFO] client connected\n");
//#endif
            pthread_mutex_lock(&args.game->game_mutex);

            if (args.game->player_count < 4) {
                int pid;
                recv(cfd, &pid, sizeof(pid), 0);
                unsigned int free_slot = find_free_player_slot(args.game);
                Player *player = create_player(free_slot, get_random_free_location(args.game->map));
                args.game->players[free_slot] = player;
                args.game->players[free_slot]->pid = pid;
                args.game->players[free_slot]->cfd = cfd;
                args.game->player_count++;
                // inint mutex
//                pthread_mutex_init(&args.game->players[free_slot]->player_mutex, NULL);


                pthread_create(&player->thread, NULL, player_thread, &args.game->players[free_slot]);

            } else {
                fprintf(stderr, "[CONN_THERED] [INFO] game is full\n");
            }
            pthread_mutex_unlock(&args.game->game_mutex);

        }
    }
    return NULL;
}


Server server;


int main(void) {

    int sfd = create_socket(PORT);
    if (0 > sfd) {
        return -1;
    }


    Map *map = load_map();

    Game *game = create_game(map);

//    ConnHandlingArgs args = {sfd, game};

//    Server server = {true, sfd, game};

    server.server_running = true;
    server.sfd = sfd;
    server.game = game;

    pthread_t threadConnHandling;

    pthread_create(&threadConnHandling, NULL, connection_handlig, &server);

    // initialize ncurses screen
    init_screen();


//    attron(COLOR_PAIR(PLAYER_COLOR));
//    mvprintw(player1->spawn_point.y, player1->spawn_point.x, "%d", 1);
//    attroff(COLOR_PAIR(1));
//    refresh();
//


    display_static_game_info(game);
    display_game_legend(game);
    refresh();

    while (server.server_running) {

        display_map(game->map);



        pthread_mutex_lock(&game->game_mutex);
        move_players(game);
        display_players_on_map(game);
        display_non_static_game_info(game);
        refresh();
        pthread_mutex_unlock(&game->game_mutex);

//#if DEBUG
//        move(game->map->height + 1, 0);
//#endif
        sleep(1);

        game->round_number += 1;
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



    done_screen(); // end ncurses screen

    destroy_game(&game);

    printf("Server closed\n");

    return 0;
}

