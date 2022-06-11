#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "file_read.h"

#define GRAPH_FILE_NAME "/home/kwit666/Documents/Karol/Studia/Semestr 8/NKRWZMWO/projekt/graph.txt"
#define NODE_NUMBER 7
#define MAX_NEIGH 10
#define NODE_NUMBER_POWER 3125
#define MAX_COST 6
typedef struct {
    size_t node;
    size_t cost;
} neigh_T;

typedef struct {
    bool visited;
    size_t base_node;
    neigh_T neigh[MAX_NEIGH];
} node_T;

typedef struct {
    size_t edge_node_x;
    size_t edge_node_y;
} edge_T;

typedef struct {
    size_t cost;
    edge_T edge[MAX_NEIGH + 1]; // this '+1' is required during checking neighbors number in dfs function
} path_with_cost_T; 

node_T graph[NODE_NUMBER] = {
    {.base_node = 1,    .neigh[0] = {.node = 2, .cost = 3}},
    {.base_node = 2,    .neigh[0] = {.node = 1, .cost = 2},
                        .neigh[1] = {.node = 3, .cost = 1},
                        .neigh[2] = {.node = 4, .cost = 3},
                        .neigh[3] = {.node = 5, .cost = 7},
                        .neigh[4] = {.node = 7, .cost = 7}},
    {.base_node = 3,    .neigh[0] = {.node = 2, .cost = 1},
                        .neigh[1] = {.node = 4, .cost = 4},},
    {.base_node = 4,    .neigh[0] = {.node = 2, .cost = 1},
                        .neigh[1] = {.node = 3, .cost = 4},
                        .neigh[2] = {.node = 5, .cost = 8},},
    {.base_node = 5,    .neigh[0] = {.node = 2, .cost = 6},
                        .neigh[1] = {.node = 4, .cost = 9},},
    {.base_node = 6,    .neigh[0] = {.node = 7, .cost = 6}},
    {.base_node = 7,    .neigh[0] = {.node = 6, .cost = 6},
                        .neigh[1] = {.node = 2, .cost = 6},}
};

// neigh_T graph[NODE_NUMBER][MAX_NEIGH] = {
//     [1] = {{.node = 2, .cost = 3}},
//     [2] = {
//             [0] = {.node = 1, .cost = 2},
//             [1] = {.node = 3, .cost = 1},
//             [2] = {.node = 4, .cost = 3},
//             [3] = {.node = 5, .cost = 7},
//             [4] = {.node = 7, .cost = 7}
//     },
//     [3] = {
//             [0] = {.node = 2, .cost = 1},
//             [1] = {.node = 4, .cost = 4},
//     },
//     [4] = {
//             [0] = {.node = 2, .cost = 1},
//             [1] = {.node = 3, .cost = 4},
//             [2] = {.node = 5, .cost = 8},
//     },
//     [5] = {
//             [0] = {.node = 2, .cost = 6},
//             [1] = {.node = 4, .cost = 9},
//     },
//     [6] = {
//             [0] = {.node = 7, .cost = 6}
//     },
//     [7] = {
//             [0] = {.node = 6, .cost = 6},
//             [1] = {.node = 2, .cost = 6}
//     }
// };

path_with_cost_T paths_with_cost[NODE_NUMBER_POWER];

size_t dfs(size_t node_number) {
    static size_t edge_iter = 0;
    static size_t path_iter = 0;
    static size_t return_counter = 0;
    node_T *current_node;

    // find current node in graph array
    for (size_t node = 0; node < NODE_NUMBER; node++) {
        if (node_number == graph[node].base_node) {
            current_node = &graph[node];
        }
    }

    current_node->visited = true;

    // here we check how much neighbors have current node, to do that all node must have at least one empty element in the array
    size_t node_neigh_num = 0U;
    for (size_t neigh = 0U; neigh < MAX_NEIGH; neigh++) {
        if (current_node->neigh[neigh].cost == 0U) {
            node_neigh_num = neigh;
            break;
        }
    }

    for (size_t neigh = 0U; neigh < node_neigh_num; neigh++) {
        // if (!visited[graph[current_node][neigh].node]) {
        if (!graph[current_node->neigh[neigh].node].visited) {
            if (return_counter) {
                path_iter++; // now we will fill new path 
                edge_iter -= return_counter; // start fill from core path
                /* here we copy only core path */
                memcpy(&paths_with_cost[path_iter].edge[0], &paths_with_cost[path_iter - 1].edge[0], sizeof(edge_T) * edge_iter);
                return_counter = 0; //return counter is reset only when next neighbor is not visited
            }
            paths_with_cost[path_iter].edge[edge_iter].edge_node_x = node_number; // first edge node is current node
            paths_with_cost[path_iter].edge[edge_iter].edge_node_y = current_node->neigh[neigh].node;
            edge_iter++;    // next edge
            dfs(current_node->neigh[neigh].node);   // recursive call
            graph[current_node->neigh[neigh].node].visited = false; // set visited flag to false as Mr. Fortuna said
            return_counter++;   // we come back from dfs so return counter should be incremented
        }
    }

    return path_iter;
}

// void cost_filtering(size_t path_iter) {
//     size_t node = 0;
//     size_t edge = 0;
//     size_t neigh = 0;

//     for (size_t path = 0U; path < path_iter; path++) {  // iterate through all paths
//         for (edge = 0U; edge < MAX_NEIGH && paths_with_cost[path].cost <= MAX_COST; edge++) { // iterate through all edge  // ToDo equal?    
//             for (neigh = 0U; neigh < MAX_NEIGH; neigh++){   
//                 if (paths_with_cost[path].edge[edge].edge_node_y == graph[paths_with_cost[path].edge[edge].edge_node_x][neigh].node) {
//                     paths_with_cost[path].cost += graph[paths_with_cost[path].edge[edge].edge_node_x][neigh].cost;
//                     break;
//                 }
//             }
//         }
//         edge--;
//         paths_with_cost[path].cost -= graph[paths_with_cost[path].edge[edge].edge_node_x][neigh].cost;
//         memset(&paths_with_cost[path].edge[edge], 0, (MAX_NEIGH - edge) * sizeof(edge_T));  // why? kuuuuurwea
//         break;
//     }
// }

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

    size_t path_counter = dfs(1);
    // cost_filtering(path_counter);
    return 0;
}
