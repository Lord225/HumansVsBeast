#ifndef HUMANSVSBEAST_MAP_H
#define HUMANSVSBEAST_MAP_H

#define PLAYER_SIGHT 5

typedef enum Tile {
    EMPTY = ' ',
    WALL = '?',
    CAMPSITE = 'A',
    BUSH = '#',
    COIN = 'c',
    TREASURE = 't',
    LARGE_TREASURE = 'T',
    DROPPED_TRERSURE = 'D',
    BEAST = '*',
    PLAYER1 = '1',
    PLAYER2 = '2',
    PLAYER3 = '3',
    PLAYER4 = '4',
} Tile;

typedef struct Field {
    Tile tile;
    int value;
} Field;

typedef struct Map {
    Field **fields;
    int height;
    int width;
} Map;

typedef struct Location {
    int x;
    int y;
} Location;

Map *load_map(void);

void destroy_map(Map **map);

void display_map(Map *map);

Location get_random_free_location(Map *map);

void add_new_coin(Map *map, Location location);

void add_new_treasure(Map *map, Location location);

void add_new_large_treasure(Map *map, Location location);

#endif //HUMANSVSBEAST_MAP_H
