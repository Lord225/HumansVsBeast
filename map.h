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

MAP *load_map(void);

void display_map(MAP *map);

void unload_map(MAP **map);

void display_map_ncurses(MAP *map);

#endif //HUMANSVSBEAST_MAP_H
