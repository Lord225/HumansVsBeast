#include "dfs.h"
#include <limits.h>
#include <string.h>
#include <stdbool.h>


bool isSafe(int mat[SIZE][SIZE], bool visited[SIZE][SIZE], int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE) && mat[x][y] == 0 && !visited[x][y];
}


void findShortestPath(int mat[SIZE][SIZE], bool visited[SIZE][SIZE], int i, int j, int x, int y, int *min_dist, int dist) {
    if (i == x && j == y) {
        if (dist < *min_dist) {
            *min_dist = dist;
        }
        return;
    }

    visited[i][j] = true;

    if (isSafe(mat, visited, i + 1, j)) {
        findShortestPath(mat, visited, i + 1, j, x, y, min_dist, dist + 1);
    }

    if (isSafe(mat, visited, i, j + 1)) {
        findShortestPath(mat, visited, i, j + 1, x, y, min_dist, dist + 1);
    }

    if (isSafe(mat, visited, i - 1, j)) {
        findShortestPath(mat, visited, i - 1, j, x, y, min_dist, dist + 1);
    }

    if (isSafe(mat, visited, i, j - 1)) {
        findShortestPath(mat, visited, i, j - 1, x, y, min_dist, dist + 1);
    }

    visited[i][j] = false;
}


int findShortestPathLength(int mat[SIZE][SIZE], struct point src, struct point dest) {
    if (mat[src.first][src.second] == 1 || mat[dest.first][dest.second] == 1) {
        return -1;
    }

    bool visited[SIZE][SIZE];
    memset(visited, 0, sizeof(visited));
    int dist = INT_MAX;
    findShortestPath(mat, visited, src.first, src.second, dest.first, dest.second, &dist, 0);
    if (dist != INT_MAX) {
        return dist;
    }
    return -1;

}
