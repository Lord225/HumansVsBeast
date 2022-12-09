#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include "game.h"
#include "common.h"
#include "beast_behaviour.h"
#include "socket.h"
#include "utils.h"


#define BEASTS_AT_START 1

#define PORT 9002

pthread_cond_t beastCond;
pthread_cond_t keyboardCond;


typedef struct Server {
    bool server_running;
    int sfd;
    Game *game;
} Server;

typedef struct BeastPayload {
    Beast *beast;
    Server *server;

} BeastPayload;


void *beast_thread(void *arg) {
    BeastPayload *payload = (BeastPayload *) arg;
    Beast *beast = payload->beast;
    Game *game = payload->server->game;

    while (payload->server->server_running) {

//        pthread_mutex_lock(&game->game_mutex);

        pthread_cond_wait(&beastCond, &game->game_mutex);

        int new_dir = beast_ai(game, beast);
        beast->direction = new_dir;
        pthread_mutex_unlock(&game->game_mutex);

    }

    return NULL;
}


void *player_thread(void *arg) {

    Player **p = (Player **) arg;
    Player *player = *p;

    ClientInfoForServer client_info = {0};
    client_info.is_connected = true;

    while (client_info.is_connected) {

        ssize_t bytes_recived = recv(player->cfd, &client_info, sizeof(ClientInfoForServer), 0);
        if (bytes_recived <= 0) {
            client_info.is_connected = false;
            destroy_player(p);
        }
        player->was_key_sent_last_turn = true;
        player->last_key = client_info.key;

        if (player->last_key == 'q' || player->last_key == 'Q') {
            client_info.is_connected = false;
            destroy_player(p);
        }

    }


    return NULL;
}


void *connection_handlig(void *arg) {
    Server *args = (Server *) arg;

    while (args->server_running) {


        // define client address
        struct sockaddr_in client_info = {0};
        socklen_t client_info_len = sizeof(client_info);

        // accept connection
        int cfd = accept(args->sfd, (struct sockaddr *) &client_info, &client_info_len);
        if (cfd < 0) {

        } else {

            pthread_mutex_lock(&args->game->game_mutex);

            int free_slot = find_free_player_slot(args->game);

            if (free_slot != -1) {
                struct ServerInfoForPlayer server_info = {0};
                server_info.server_is_full = false;
                send(cfd, &server_info, sizeof(ServerInfoForPlayer), 0);

                int pid;
                recv(cfd, &pid, sizeof(pid), 0);

                Player *player = create_player(free_slot, get_random_free_location(args->game));
                args->game->players[free_slot] = player;
                args->game->players[free_slot]->pid = pid;
                args->game->players[free_slot]->cfd = cfd;
                args->game->player_count++;

                pthread_create(&player->thread, NULL, player_thread, &args->game->players[free_slot]);

            } else {
                struct ServerInfoForPlayer server_info = {0};
                server_info.server_is_full = true;

                send(cfd, (struct ServerInfoForPlayer *) &server_info, sizeof(struct ServerInfoForPlayer), 0);

            }
            pthread_mutex_unlock(&args->game->game_mutex);

        }
    }
    return NULL;
}

void *gameLoop(void *arg) {

    Server *args = (Server *) arg;

    Game *game = args->game;

    display_map(game->map);

    refresh();

    if (game->beast_count < MAX_BEASTS && BEASTS_AT_START > 0) {
        pthread_mutex_lock(&game->game_mutex);
        for (int i = 0; i < BEASTS_AT_START; i++) {
            Beast *beast = add_new_beast(game);
            if (beast != NULL) {
                BeastPayload payload = {0};
                payload.beast = beast;
                payload.server = args;
                pthread_create(&beast->thread, NULL, beast_thread, &payload);
            }
        }
        pthread_mutex_unlock(&game->game_mutex);
    }

    while (args->server_running) {

        pthread_mutex_lock(&game->game_mutex);
        move_players(game);
        if (game->beast_count > 0) {
            move_beasts(game);
        }
        kill_and_respawn_dead_players(game);
        send_map_data_to_all_players(game);
        display_map(game->map);
        display_players_on_map(game);
        if (game->beast_count > 0) {
            display_beasts_on_map(game);
        }

        display_non_static_game_info(game);

        refresh();
        pthread_mutex_unlock(&game->game_mutex);

        usleep(500000);
        pthread_cond_signal(&beastCond);
        pthread_cond_signal(&keyboardCond);

        game->round_number += 1;

    }

    return NULL;
}


int main(void) {

    int sfd = create_socket(PORT);
    if (0 > sfd) {
        return -1;
    }


    Map *map = load_map();

    Game *game = create_game(map);

//    ConnHandlingArgs args = {sfd, game};

//    Server server = {true, sfd, game};

    Server server;
    server.server_running = true;
    server.sfd = sfd;
    server.game = game;

    pthread_t threadConnHandling;
    pthread_t threadGameLoop;


    pthread_create(&threadConnHandling, NULL, connection_handlig, &server);


    // initialize ncurses screen
    init_screen();

    display_static_game_info(game);
    display_game_legend(game);
    refresh();

    pthread_create(&threadGameLoop, NULL, gameLoop, &server);


    while (server.server_running) {
        int key = getch();
        if (key == 'q' || key == 'Q') {
            pthread_mutex_lock(&game->game_mutex);
            disconnect_players(game->players);
            pthread_mutex_unlock(&game->game_mutex);
            server.server_running = false;
            pthread_join(threadGameLoop, NULL);

        } else if (key == 'c') {
//            pthread_mutex_lock(&game->game_mutex);
            pthread_cond_wait(&keyboardCond, &game->game_mutex);
            add_new_coin(game->map, get_random_free_location(game));
            pthread_mutex_unlock(&game->game_mutex);
        } else if (key == 't') {
//            pthread_mutex_lock(&game->game_mutex);
            pthread_cond_wait(&keyboardCond, &game->game_mutex);
            add_new_treasure(game->map, get_random_free_location(game));
            pthread_mutex_unlock(&game->game_mutex);
        } else if (key == 'T') {
//            pthread_mutex_lock(&game->game_mutex);
            pthread_cond_wait(&keyboardCond, &game->game_mutex);
            add_new_large_treasure(game->map, get_random_free_location(game));
            pthread_mutex_unlock(&game->game_mutex);
        } else if (key == 'b' || key == 'B') {
            if (game->beast_count < MAX_BEASTS) {
                pthread_cond_wait(&keyboardCond, &game->game_mutex);
//                pthread_mutex_lock(&game->game_mutex);
                Beast *beast = add_new_beast(game);
                if (beast != NULL) {
                    BeastPayload payload = {0};
                    payload.beast = beast;
                    payload.server = &server;
                    pthread_create(&beast->thread, NULL, beast_thread, &payload);
                }
                pthread_mutex_unlock(&game->game_mutex);
            }

        }
        flushinp();
    }


//    pthread_join(threadGameLoop, NULL);
    pthread_cancel(threadConnHandling);
    pthread_join(threadConnHandling, NULL);


    done_screen(); // end ncurses screen
//    printf("server stopped\n");


//    printf("threadConnHandling joined\n");

//    printf("threadGameLoop joined\n");


    destroy_game(&game);

    printf("Server closed\n");

    return 0;
}

