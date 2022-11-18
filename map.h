#ifndef HUMANSVSBEAST_MAP_H
#define HUMANSVSBEAST_MAP_H

#define PLAYER_COLOR 1
#define MONEY_COLOR 2
#define CAMP_COLOR 3

typedef struct Map {
    char **map;
    int height;
    int width;
} Map;

typedef struct LOCATION {
    unsigned int x;
    unsigned int y;
} LOCATION;

Map *load_map(void);

void display_map(Map *map);

void destroy_map(Map **map);

void display_map_ncurses(Map *map);

LOCATION get_random_free_location(Map *map);

#endif //HUMANSVSBEAST_MAP_H
