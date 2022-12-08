//
// Created by Mateusz on 05.12.2022.
//

#ifndef HUMANSVSBEAST_COMMON_H
#define HUMANSVSBEAST_COMMON_H

#include <stdbool.h>
#include "map.h"

typedef struct PlayerSight {
    Field fields[PLAYER_SIGHT][PLAYER_SIGHT];
    int radius;
    int cord_x;
    int cord_y;
} PlayerSight;

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
    PlayerSight player_sight;
    int map_height;
    int map_width;
    int server_pid;
    int round_number;
    int deaths;
    int coins_found;
    int coins_brought;
    int player_number;

    bool disconnect_player_server_closed;

} ServerInfoForPlayer;



#endif //HUMANSVSBEAST_COMMON_H
