#ifndef HUMANSVSBEAST_MAP_H
#define HUMANSVSBEAST_MAP_H

#define PLAYER_COLOR 1
#define MONEY_COLOR 2
#define CAMP_COLOR 3
#define BEAST_COLOR 4

typedef enum Tile {
    EMPTY = ' ',
    WALL = '?',
    CAMPSITE = 'A',
    BUSH = '#',
    COIN = 'c',
    TREASURE = 't',
    LARGE_TREASURE = 'T',
    DROPPED_TRERSURE = 'D'
} Tile;

typedef struct Field {
    Tile tile;
} Field;

typedef struct Map {
    Field **fields;
    int height;
    int width;
} Map;

typedef struct Location {
    unsigned int x;
    unsigned int y;
} Location;

Map *load_map(void);

void destroy_map(Map **map);

void display_map(Map *map);

Location get_random_free_location(Map *map);

#endif //HUMANSVSBEAST_MAP_H
