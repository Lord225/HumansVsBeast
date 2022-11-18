#ifndef HUMANSVSBEAST_MAP_H
#define HUMANSVSBEAST_MAP_H

#define PLAYER_COLOR 1
#define MONEY_COLOR 2
#define CAMP_COLOR 3

typedef struct MAP {
    char **map;
    int height;
    int width;
} MAP;

typedef struct LOCATION {
    unsigned int x;
    unsigned int y;
} LOCATION;

MAP *load_map(void);

void display_map(MAP *map);

void destroy_map(MAP **map);

void display_map_ncurses(MAP *map);

LOCATION get_random_free_location(MAP *map);

#endif //HUMANSVSBEAST_MAP_H
