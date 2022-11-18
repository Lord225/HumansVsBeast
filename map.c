#include "map.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_FILE "map.txt"

MAP *load_map(void) {

    FILE *f = fopen(MAP_FILE, "r");

    if (!f) {
        printf("Error: Can't open map file");
        exit(1);
    }

    MAP *map = malloc(sizeof(MAP));
    if (!map) {
        printf("Error: Can't allocate memory for map");
        exit(1);
    }

    map->height = 0;
    map->width = 0;
    map->map = NULL;

    fscanf(f, "%d %d\n", &map->height, &map->width);

    map->map = malloc(sizeof(char *) * map->height);
    if (!map->map) {
        printf("Error: Can't allocate memory for map");
        exit(1);
    }
    for (int i = 0; i < map->height; i++) {
        map->map[i] = malloc(sizeof(char) * map->width);
        if (!map->map[i]) {
            printf("Error: Can't allocate memory for map");
            exit(1);
        }
        for (int j = 0; j < map->width; j++) {
            fscanf(f, "%c", &map->map[i][j]);
        }
        fscanf(f, "\n");
    }

    fclose(f);

    return map;
}

void destroy_map(MAP **map) {

    if (!map || !*map) {
        return;
    }

    for (int i = 0; i < (*map)->height; i++) {
        free((*map)->map[i]);
    }
    free((*map)->map);
    free(*map);
    *map = NULL;

}

void display_map(MAP *map) {

    if (!map) {
        printf("Error: Can't display map");
        exit(1);
    }

    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            printf("%c", map->map[i][j]);
        }
        printf("\n");
    }

}

void display_map_ncurses(MAP *map) {

    if (!map) {
        printf("Error: Can't display map");
        exit(1);
    }

//    start_color();
    init_pair(MONEY_COLOR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CAMP_COLOR, COLOR_WHITE, COLOR_GREEN);

    for (int i = 0; i < map->height; i++) {
        move(i, 0);
        for (int j = 0; j < map->width; j++) {
            if (map->map[i][j] == '?') {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            } else if (map->map[i][j] == 'c' || map->map[i][j] == 't' || map->map[i][j] == 'T') {
                attron(COLOR_PAIR(2));
                printw("%c", map->map[i][j]);
                attroff(COLOR_PAIR(1));
            } else if (map->map[i][j] == 'A') {
                attron(COLOR_PAIR(3));
                printw("%c", map->map[i][j]);
                attroff(COLOR_PAIR(3));
            } else {

                printw("%c", map->map[i][j]);
            }

        }
    }

}

LOCATION get_random_free_location(MAP *map) {

    LOCATION location;

    srand(time(NULL));
    unsigned int x = rand() % map->width;
    unsigned int y = rand() % map->height;


    while (map->map[y][x] != ' ') {
        x = rand() % map->width;
        y = rand() % map->height;
    }

    location.x = x;
    location.y = y;

    return location;
}