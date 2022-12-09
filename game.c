#include "game.h"
#include <stdlib.h>
#include "map.h"
#include "player.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include "utils.h"
#include "common.h"


Game *create_game(Map *map) {
    Game *game = calloc(1, sizeof(Game));
//    game->round_number = 0;
    game->map = map;
    game->campsite_location = find_campsite_location(map);

//    game->player_count = 0;
    for (int i = 0; i < 4; i++) {
        game->players[i] = NULL;
    }
    pthread_mutex_init(&game->game_mutex, NULL);

    return game;
}

Location find_campsite_location(Map *map) {
    Location location = {0, 0};

    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (map->fields[i][j].tile == CAMPSITE) {
                location.x = j;
                location.y = i;
                break;
            }
        }
    }

    return location;
}

void display_game_legend(Game *game) {
    int row = game->map->height - 9;
    int column = game->map->width + 3;

    mvprintw(row++, column, "Legend:");
    attron(COLOR_PAIR(PLAYER_COLOR));
    mvprintw(row, column + 1, "1234");
    attroff(COLOR_PAIR(PLAYER_COLOR));
    mvprintw(row++, column + 6, "- players");
    attron(A_REVERSE);
    mvprintw(row, column + 1, " ");
    attroff(A_REVERSE);
    mvprintw(row++, column + 6, "- wall");
    mvprintw(row++, column + 1, "#    - bushes (slow down)");
    attron(COLOR_PAIR(BEAST_COLOR));
    mvprintw(row, column + 1, "*");
    attroff(COLOR_PAIR(BEAST_COLOR));
    mvprintw(row++, column + 6, "- wild beast");
    attron(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 1, "c");
    attroff(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 6, "- one coin");
    attron(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 27, "D");
    attroff(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row++, column + 29, "- dropped treasure");
    attron(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 1, "t");
    attroff(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row++, column + 6, "- treasure (10 coins)");
    attron(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row, column + 1, "T");
    attroff(COLOR_PAIR(MONEY_COLOR));
    mvprintw(row++, column + 6, "- large treasure (50 coins)");
    attron(COLOR_PAIR(CAMP_COLOR));
    mvprintw(row, column + 1, "A");
    attroff(COLOR_PAIR(CAMP_COLOR));
    mvprintw(row, column + 6, "- campsite");
}

void display_non_static_game_info(Game *game) {


    int column = game->map->width + 3;
    int row = 1;
    row += 1;


    mvprintw(row++, column + 1 + strlen("Campsite X/Y: "), "%02u/%02u", game->campsite_location.x,
             game->campsite_location.y);
    mvprintw(row++, column + 1, "Round number: %d", game->round_number);
    row += 2;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d", game->players[i]->pid);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-    ");
        }
    }
    row += 1;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%s", "HUMAN");
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-    ");
        }
    }
    row += 1;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%02u/%02u", game->players[i]->current_location.x,
                     game->players[i]->current_location.y);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "--/--");
        }
    }
    row += 1;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d", game->players[i]->deaths);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-    ");
        }
    }

    row += 3;

    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d", game->players[i]->coins_found);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-    ");
        }
    }

    row += 1;
    for (int i = 0; i < 4; i++) {
        if (game->players[i]) {
            mvprintw(row, column + 4 + 9 * (i + 1), "%d", game->players[i]->coins_brought);
        } else {
            mvprintw(row, column + 4 + 9 * (i + 1), "-    ");
        }
    }

}

void display_static_game_info(Game *game) {
    int column = game->map->width + 3;
    int row = 1;
    mvprintw(row++, column, "Server's PID: %d", getpid());
    mvprintw(row++, column + 1, "Campsite X/Y:");
    mvprintw(row++, column + 1, "Round number:");
    row += 1;
    mvprintw(row++, column, "Parameter:   Player1  Player2  Player3  Player4");
    mvprintw(row++, column + 1, "PID");
    mvprintw(row++, column + 1, "Type");
    mvprintw(row++, column + 1, "Curr X/Y");
    mvprintw(row++, column + 1, "Deaths");
    row += 1;
    mvprintw(row++, column + 1, "Coins");
    mvprintw(row++, column + 5, "carried");
    mvprintw(row++, column + 5, "brought");

}

void destroy_game(Game **game) {
    if (!game || !*game) {
        return;
    }

    destroy_map(&(*game)->map);

    for(int i=0;i<MAX_BEASTS;i++){
        destroy_beast(&(*game)->beasts[i]);
    }

    for (size_t i = 0; i < MAX_PLAYERS; i++) {
        if ((*game)->players[i]) {
            destroy_player(&(*game)->players[i]);
        }
    }

    pthread_mutex_destroy(&(*game)->game_mutex);

    free(*game);
    *game = NULL;
}


int find_free_player_slot(Game *game) {
    for (int i = 0; i < 4; i++) {
        if (!game->players[i]) {
            return i;
        }
    }
    return -1;
}

void spawn_player(Game *game, int player_id) {

    mvprintw(game->players[player_id]->spawn_point.y, game->players[player_id]->spawn_point.x, "%d",
             player_id + 1);

}

void display_players_on_map(Game *game) {

    attron(COLOR_PAIR(PLAYER_COLOR));
    for (size_t i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i]) {
            mvprintw(game->players[i]->current_location.y, game->players[i]->current_location.x, "%d",
                     i + 1);
        }
    }
    attroff(COLOR_PAIR(PLAYER_COLOR));
}

void move_players(Game *game) {

    for (size_t i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i]) {
            player_move(game, game->players[i]);
        }
    }
}

void player_move(Game *game, Player *player) {

    Map *map = game->map;

    if (player->is_stunned) {
        player->is_stunned = false;
//        player->was_key_sent_last_turn = false;
        kill_and_respawn_dead_players(game);
        return;
    }

    Location new_location = player->current_location;

    if (player->was_key_sent_last_turn) {
        switch (player->last_key) {
            case KEY_UP:
                new_location.y -= 1;

                break;
            case KEY_DOWN:
                new_location.y += 1;

                break;
            case KEY_LEFT:

                new_location.x -= 1;

                break;
            case KEY_RIGHT:

                new_location.x += 1;

                break;
            default:
                break;
        }

        int is_valid_move = validate_player_move(map, player, new_location);
        if (is_valid_move) {
            player->current_location = new_location;

        }


    }

    handle_player_map_interaction(game, player);

    player->was_key_sent_last_turn = false;

}

int validate_player_move(Map *map, Player *player, Location new_location) {

//    if (new_location.x < 0 || new_location.x >= map->width) {
//        return 0;
//    }
//    if (new_location.y < 0 || new_location.y >= map->height) {
//        return 0;
//    }
    if (map->fields[new_location.y][new_location.x].tile == WALL) {
        return 0;
    }
    return 1;

}

void handle_player_map_interaction(Game *game, Player *player) {

    Map *map = game->map;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] && game->players[i] != player) {
            if (game->players[i]->current_location.x == player->current_location.x &&
                game->players[i]->current_location.y == player->current_location.y) {
                player->is_dead = true;
                game->players[i]->is_dead = true;
            }
        }
    }

    for(int i=0;i<MAX_BEASTS;i++){
        if(game->beasts[i]){
            if(game->beasts[i]->current_location.x == player->current_location.x &&
               game->beasts[i]->current_location.y == player->current_location.y){
                player->is_dead = true;
            }
        }
    }

    if (map->fields[player->current_location.y][player->current_location.x].tile == COIN) {
        player->coins_found += 1;
        map->fields[player->current_location.y][player->current_location.x].tile = EMPTY;
    } else if (map->fields[player->current_location.y][player->current_location.x].tile == CAMPSITE) {
        player->coins_brought += player->coins_found;
        player->coins_found = 0;
    } else if (map->fields[player->current_location.y][player->current_location.x].tile == TREASURE) {
        player->coins_found += 10;
        map->fields[player->current_location.y][player->current_location.x].tile = EMPTY;
    } else if (map->fields[player->current_location.y][player->current_location.x].tile == LARGE_TREASURE) {
        player->coins_found += 50;
        map->fields[player->current_location.y][player->current_location.x].tile = EMPTY;
    } else if (map->fields[player->current_location.y][player->current_location.x].tile == DROPPED_TRERSURE) {
        player->coins_found += map->fields[player->current_location.y][player->current_location.x].value;
        map->fields[player->current_location.y][player->current_location.x].tile = EMPTY;
    } else if (map->fields[player->current_location.y][player->current_location.x].tile == BUSH) {
        player->is_stunned = true;
    }
}

void kill_and_respawn_dead_players(Game *game) {

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] && game->players[i]->is_dead) {
            game->players[i]->is_dead = false;
            if (game->players[i]->coins_found > 0) {
                game->map->fields[game->players[i]->current_location.y][game->players[i]->current_location.x].value += game->players[i]->coins_found;
                game->map->fields[game->players[i]->current_location.y][game->players[i]->current_location.x].tile = DROPPED_TRERSURE;
            }
            game->players[i]->coins_found = 0;
            game->players[i]->deaths += 1;
            game->players[i]->current_location = game->players[i]->spawn_point;
        }
    }

}

int send_map_data_to_player(Game *game, Player *player) {

    ServerInfoForPlayer server_info = {0};
    server_info.map_width = game->map->width;
    server_info.map_height = game->map->height;

    if(player->campsite_found) {
        server_info.campsite_x = game->campsite_location.x;
        server_info.campsite_y = game->campsite_location.y;
    } else {
        server_info.campsite_x = -1;
        server_info.campsite_y = -1;
    }


    PlayerSight player_sight = {0};

    player_sight.radius = PLAYER_SIGHT;
    player_sight.cord_x = player->current_location.x;
    player_sight.cord_y = player->current_location.y;

    server_info.player_number = player->id + 1;
    server_info.server_pid = getpid();
    server_info.deaths = player->deaths;
    server_info.coins_found = player->coins_found;
    server_info.coins_brought = player->coins_brought;
    server_info.round_number = game->round_number;


    for (int i = 0; i < PLAYER_SIGHT; i++) {
        for (int j = 0; j < PLAYER_SIGHT; j++) {
            int first = player_sight.cord_y - PLAYER_SIGHT / 2 + i;
            int second = player_sight.cord_x - PLAYER_SIGHT / 2 + j;
            if (first >= 0 && second >= 0 && first < game->map->height && second < game->map->width) {
                player_sight.fields[i][j] = game->map->fields[first][second];
        if(player_sight.fields[i][j].tile == CAMPSITE) {
            player->campsite_found = true;
        }
                for (int k = 0; k < MAX_PLAYERS; k++) {
                    if (game->players[k] && game->players[k] != player) {
                        if (game->players[k]->current_location.x == second &&
                            game->players[k]->current_location.y == first) {
                            player_sight.fields[i][j].tile = PLAYER1 + k;
                        }
                    }
                }

                for (int k = 0; k < game->beast_count; k++) {
                        if (game->beasts[k]->current_location.x == second &&
                            game->beasts[k]->current_location.y == first) {
                            player_sight.fields[i][j].tile = BEAST;
                        }
                }
            } else {
                player_sight.fields[i][j].tile = WALL;
            }

        }
    }

    server_info.player_sight = player_sight;

    send(player->cfd, &server_info, sizeof(server_info), 0);

    return 0;
}

int send_map_data_to_all_players(Game *game) {

    for (size_t i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i]) {
            send_map_data_to_player(game, game->players[i]);
        }
    }
    return 0;

}

void disconnect_players(Player *players[]) {

    for (size_t i = 0; i < MAX_PLAYERS; i++) {
        if (players[i]) {
            disconnect_player(players[i]);
        }
    }
}

void disconnect_player(Player *player) {

    ServerInfoForPlayer server_info = {0};
    server_info.disconnect_player_server_closed = true;
    send(player->cfd, &server_info, sizeof(ServerInfoForPlayer), 0);

}

Location get_random_free_location(Game *game) {

    Map *map = game->map;
    Location location = {0, 0};

    srand(time(NULL));
    int x = rand() % map->width;
    int y = rand() % map->height;


    while (map->fields[y][x].tile != EMPTY || is_entity_on(game, x, y)) {
        x = rand() % map->width;
        y = rand() % map->height;
    }

    location.x = x;
    location.y = y;

    return location;
}

int is_player_on(Game *game, int x, int y) {

    Location location;
    location.x = x;
    location.y = y;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i]) {
            if ((game->players[i]->current_location.x == location.x &&
                 game->players[i]->current_location.y == location.y) ||
                (game->players[i]->spawn_point.x == location.x && game->players[i]->spawn_point.y == location.y)) {
                return 1;
            }
        }
    }


    return 0;

}

int is_beast_on(Game *game, int x, int y) {

    Location location;
    location.x = x;
    location.y = y;

    for (int i = 0; i < game->beast_count; i++) {
        if (game->beasts[i] && game->beasts[i]->current_location.x == location.x &&
            game->beasts[i]->current_location.y == location.y) {
            return 1;
        }
    }

    return 0;

}

int is_entity_on(Game *game, int x, int y) {

    return is_player_on(game, x, y) || is_beast_on(game, x, y);

}


