#include <ncurses.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "common.h"
#include "utils.h"
#include <pthread.h>
#include "client_utils.h"

#define PORT 9002
#define ADDRESS 0x7f000001

bool is_running = true;

#define DEBUG 1

void debug_print(char *msg) {
    if (DEBUG) {
        printw(msg);
        refresh();
    }
}

void *listen_for_server_info(void *arg) {

    int *sfd = (int *) arg;

//    ServerInfoForPlayer *server_info = {0};
    ServerInfoForPlayer server_info = {0};

    while (is_running) {

        ssize_t recvd = recv(*sfd, &server_info, sizeof(ServerInfoForPlayer), 0);
        if (recvd <= 0) {
            is_running = false;

            break;
        }
//        printw("goodbye!\n");
//        refresh();

        if (server_info.disconnect_player_server_closed) {
            is_running = false;
            break;
        }

        erase();
        display_player_map(&server_info);
        display_stats(&server_info);
        refresh();
    }

    return NULL;
}

void *send_key_to_server(void *arg) {

    int *sfd = (int *) arg;

    ClientInfoForServer client_info = {0};
    client_info.is_connected = true;

    while (is_running) {

        int key = getch();

        client_info.key = key;
        send(*sfd, &client_info, sizeof(ClientInfoForServer), 0);

        if (key == 'q' || key == 'Q') {
            is_running = false;
            break;
        }


    }

    return NULL;
}

int main(void) {

    pthread_t listen_thread;
    pthread_t keyboard_thread;

    int pid = getpid();

    ClientInfoForServer client_message = {0};
    ServerInfoForPlayer server_message = {0};

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sfd) {
        perror("socket");
        return -1;
    }

    // define server address
    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr.s_addr = htonl(ADDRESS);

    socklen_t server_info_len = sizeof(server_info);

    // connect to server
    if (0 > connect(sfd, (struct sockaddr *) &server_info, server_info_len)) {
        perror("connect");
        return -1;
    }

    recv(sfd, &server_message, sizeof(server_message), 0);

    if (server_message.server_is_full) {
        printf("Server is full. Try again later.\n");
        close(sfd);
        return -1;
    }

    send(sfd, &pid, sizeof(pid), 0);


    // start ncurses

    init_screen();
//    printw("Welcome to the game!\n");

    client_message.is_connected = true;


    pthread_create(&listen_thread, NULL, listen_for_server_info, (void *) &sfd);
    pthread_create(&keyboard_thread, NULL, send_key_to_server, (void *) &sfd);

//    while (is_running) {
////        flushinp();
//        int key = getch();
//
//
//        client_message.key = key;
//
//        send(sfd, &client_message, sizeof(ClientInfoForServer), 0);
////        int response;
////        recv(sfd, &response, sizeof(response), 0);
//
//        if (key == 'q') {
//            break;
//        }
//
//    }
//
//
//    refresh();

    pthread_join(listen_thread, NULL);
    pthread_cancel(keyboard_thread);

    done_screen();

    close(sfd);

    printf("Disconnected\n");

    return 0;
}
