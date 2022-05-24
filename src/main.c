#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "file_read.h"

#define GRAPH_FILE_NAME "/home/kwit666/Documents/Karol/Studia/Semestr 8/NKRWZMWO/projekt/graph.txt"
#define TOP_NUMBER (7 + 1)
#define MAX_NEIGH 10
#define TOP_NUMBER_POWER 3125
#define MAX_COST 5
typedef struct {
    size_t top;
    size_t cost;
} neigh;

typedef struct {
    size_t edge_top_x;
    size_t edge_top_y;
} edge;

typedef struct {
    size_t cost;
    size_t edge[MAX_NEIGH];
} path_with_cost;

neigh graph[TOP_NUMBER][MAX_NEIGH] = {
    [1] = {{.top = 2, .cost = 3}},
    [2] = {
            [0] = {.top = 1, .cost = 2},
            [1] = {.top = 3, .cost = 1},
            [2] = {.top = 4, .cost = 3},
            [3] = {.top = 5, .cost = 7},
            [4] = {.top = 7, .cost = 7}
    },
    [3] = {
            [0] = {.top = 2, .cost = 1},
            [1] = {.top = 4, .cost = 4},
    },
    [4] = {
            [0] = {.top = 2, .cost = 1},
            [1] = {.top = 3, .cost = 4},
            [2] = {.top = 5, .cost = 8},
    },
    [5] = {
            [0] = {.top = 2, .cost = 6},
            [1] = {.top = 4, .cost = 9},
    },
    [6] = {
            [0] = {.top = 7, .cost = 6}
    },
    [7] = {
            [0] = {.top = 6, .cost = 6},
            [1] = {.top = 2, .cost = 6}
    }
};

path_with_cost paths_with_cost[TOP_NUMBER_POWER];
edge paths[TOP_NUMBER_POWER][TOP_NUMBER];
bool visited[TOP_NUMBER];


size_t dfs(size_t current_top) {
    static size_t edge_iter = 0;
    static size_t path_iter = 0;
    static size_t return_counter = 0;

    visited[current_top] = true;

    size_t top_neigh_num = 0U;
    for (size_t i = 0U; i < MAX_NEIGH; i++) {
        if (graph[current_top][i].cost == 0U) {
            top_neigh_num = i;
            break;
        }
    }

    for (size_t neigh = 0U; neigh < top_neigh_num; neigh++) {
        if (!visited[graph[current_top][neigh].top]) {
            if (return_counter) {
                path_iter++;
                edge_iter -= return_counter;
                memcpy(&paths[path_iter][0], &paths[path_iter - 1][0], sizeof(edge) * edge_iter);
                return_counter = 0;
            }
            paths[path_iter][edge_iter].edge_top_x = current_top;
            paths[path_iter][edge_iter].edge_top_y = graph[current_top][neigh].top;
            edge_iter++;
            dfs(graph[current_top][neigh].top);
            visited[graph[current_top][neigh].top] = false;
            return_counter++;
        }
    }

    return path_iter;
}

void cost_filtering(size_t path_iter) {
    size_t path_cost = 0;
    size_t top = 0;
    size_t tmp_path_cost = 0;

    for (size_t path = 0U; path < path_iter; path++) {
        for (size_t edge = 0U; edge < MAX_NEIGH; edge++) {
            for (size_t neigh = 0U; neigh < MAX_NEIGH; neigh++){
                if (paths[path][edge].edge_top_y == graph[paths[path][edge].edge_top_x][neigh].top) {
                    tmp_path_cost += graph[paths[path][edge].edge_top_x][neigh].cost;
                    break;
                }
            }
            if (tmp_path_cost > MAX_COST) {
                break;
            } else {
                path_cost = tmp_path_cost;
            }
        }
        memcpy(paths);
    }
}

int main(void) {
    // graph_array_settings settings;
    // if (read_graph_data(GRAPH_FILE_NAME, &settings)) {
    //     printf("Errno: %s\n", strerror(errno));
    //     return 1;
    // }

    // uint32_t (*graph_array)[settings.y_qty][settings.x_qty] = (uint32_t(*)[settings.y_qty][settings.x_qty])settings.pointer;

    // for(size_t y = 0U; y < settings.y_qty; y++) {
    //     for(size_t x = 0U; x < settings.x_qty; x++) {
    //         printf("%u ", (*graph_array)[y][x]);
    //     }
    //     printf("\n");
    // }

    // free(graph_array);

    dfs(1);
    return 0;
}
