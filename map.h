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
    int x;
    int y;
} Location;


typedef struct DroppedTreasure {
    Location location;
    Tile tile;
    int value;
} DroppedTreasure;

Map *load_map(void);

void destroy_map(Map **map);

void display_map(Map *map);

Location get_random_free_location(Map *map);

void add_new_coin(Map *map, Location location);

void add_new_treasure(Map *map, Location location);

void add_new_large_treasure(Map *map, Location location);

#endif //HUMANSVSBEAST_MAP_H
