//
// Created by Mateusz on 05.12.2022.
//

#ifndef HUMANSVSBEAST_COMMON_H
#define HUMANSVSBEAST_COMMON_H

#include <stdbool.h>

typedef struct ClientInfoForServer {
    int is_connected;
    int key;
} ClientInfoForServer;

typedef struct ServerInfoForPlayer {
    bool connected;
    bool serverClosed;
    bool server_is_full;
    bool disconnect;
    int cfd;
} ServerInfoForPlayer;

#endif //HUMANSVSBEAST_COMMON_H
