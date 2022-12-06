//
// Created by Mateusz on 05.12.2022.
//

#ifndef HUMANSVSBEAST_COMMON_H
#define HUMANSVSBEAST_COMMON_H

#include <stdbool.h>

typedef struct ClientInfo {
    int is_connected;
    int key;
} ClientInfo;

typedef struct ServerInfo {
    bool connected;
    bool serverClosed;
    bool server_is_full;
    bool disconnect;
} ServerInfo;

#endif //HUMANSVSBEAST_COMMON_H
