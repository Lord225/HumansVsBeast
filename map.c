#include "map.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_FILE "map.txt"

Map *load_map(void) {

    FILE *f = fopen(MAP_FILE, "r");

    if (!f) {
        printf("Error: Can't open map file");
        exit(1);
    }

    Map *map = calloc(1, sizeof(Map));
    if (!map) {
        printf("Error: Can't allocate memory for map");
        exit(1);
    }

    map->height = 0;
    map->width = 0;
    map->fields = NULL;

    fscanf(f, "%d %d\n", &map->height, &map->width);

    map->fields = calloc(map->height, sizeof(Field *));
    if (!map->fields) {
        printf("Error: Can't allocate memory for map");
        exit(1);
    }
    for (int i = 0; i < map->height; i++) {
        map->fields[i] = calloc(map->width, sizeof(Field));
        if (!map->fields[i]) {
            printf("Error: Can't allocate memory for map");
            exit(1);
        }
        for (int j = 0; j < map->width; j++) {
            fscanf(f, "%c", (char *) &map->fields[i][j].tile);
        }
        fscanf(f, "\n");
    }

    fclose(f);

    return map;
}

void destroy_map(Map **map) {

    if (!map || !*map) {
        return;
    }

    for (int i = 0; i < (*map)->height; i++) {
        free((*map)->fields[i]);
    }
    free((*map)->fields);
    free(*map);
    *map = NULL;

}

void display_map(Map *map) {

    if (!map) {
        printf("Error: Can't display map");
        exit(1);
    }

//    start_color();


    for (int i = 0; i < map->height; i++) {
        move(i, 0);
        for (int j = 0; j < map->width; j++) {
            if (map->fields[i][j].tile == WALL) {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            } else if (map->fields[i][j].tile == COIN || map->fields[i][j].tile == TREASURE ||
                       map->fields[i][j].tile == LARGE_TREASURE) {
                attron(COLOR_PAIR(2));
                printw("%c", map->fields[i][j].tile);
                attroff(COLOR_PAIR(1));
            } else if (map->fields[i][j].tile == CAMPSITE) {
                attron(COLOR_PAIR(3));
                printw("%c", map->fields[i][j].tile);
                attroff(COLOR_PAIR(3));
            } else {

                printw("%c", map->fields[i][j].tile);
            }

        }
    }

}

Location get_random_free_location(Map *map) {

    Location location;

    srand(time(NULL));
    int x = rand() % map->width;
    int y = rand() % map->height;


    while (map->fields[y][x].tile != EMPTY) {
        x = rand() % map->width;
        y = rand() % map->height;
    }

    location.x = x;
    location.y = y;

    return location;
}

void add_new_coin(Map *map, Location location) {

    map->fields[location.y][location.x].tile = COIN;

}

void add_new_treasure(Map *map, Location location) {

    map->fields[location.y][location.x].tile = TREASURE;

}

void add_new_large_treasure(Map *map, Location location) {

    map->fields[location.y][location.x].tile = LARGE_TREASURE;

}
