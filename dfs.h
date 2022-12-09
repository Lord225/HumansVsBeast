#ifndef HUMANSVSBEAST_DFS_H
#define HUMANSVSBEAST_DFS_H

#include <stdbool.h>
#include "map.h"
#define SIZE PLAYER_SIGHT

struct point {
    int first;
    int second;
};

void findShortestPath(int mat[SIZE][SIZE], bool visited[SIZE][SIZE],int i, int j, int x, int y, int *min_dist, int dist);
int findShortestPathLength(int mat[SIZE][SIZE],struct point src,struct point dest);
int dfs(void);

#endif //HUMANSVSBEAST_DFS_H
