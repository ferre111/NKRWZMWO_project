#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "file_read.h"

/* WARNING!
 * Algorithm check number of neighbors of individual nodes based on fact that everyone neigh_T should have cost diffrent than zero, so all node must have at least one empty element in the graph array.
 * Node numbering cannot start from zero.
**/

#define GRAPH_FILE_NAME "/home/kwit666/Documents/Karol/Studia/Semestr 8/NKRWZMWO/projekt/graph.txt"
#define NODES_NUMBER 7
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
    size_t number_of_visited_chambers;
} path_with_cost_T; 

node_T graph[NODES_NUMBER] = {
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

path_with_cost_T paths_with_cost[NODE_NUMBER_POWER];

int graph_get_node(size_t node_number, node_T **node_ptr) {
    for (size_t node = 0; node < NODES_NUMBER; node++) {
        if (node_number == graph[node].base_node) {
            *node_ptr = &graph[node];
            return 0;
        }
    }
    return -1;
}

size_t dfs(size_t node_number) {
    static size_t edge_iter = 0;
    static size_t path_iter = 0;
    static size_t return_counter = 0;
    node_T *current_node;

    // find current node in graph array
    graph_get_node(node_number, &current_node);
    current_node->visited = true;
    
    node_T *neigh_node;
    // Warning! This algorithm check number of neighbors of individual nodes based on fact that everyone neigh_T should have cost diffrent than zero, so all node must have at least one empty element in the graph array.
    for (size_t neigh = 0U; (current_node->neigh[neigh].cost != 0) && !graph_get_node(current_node->neigh[neigh].node, &neigh_node); neigh++) {
        if (!neigh_node->visited) {
            if (return_counter) {
                path_iter++; // now we will fill new path 
                edge_iter -= return_counter; // start fill from core path
                /* here we copy only core path */
                memcpy(&paths_with_cost[path_iter].edge[0], &paths_with_cost[path_iter - 1].edge[0], sizeof(edge_T) * edge_iter);
                return_counter = 0; //return counter is reset only when next neighbor is not visited
            }
            paths_with_cost[path_iter].edge[edge_iter].edge_node_x = node_number; // first edge node is current node
            paths_with_cost[path_iter].edge[edge_iter].edge_node_y = current_node->neigh[neigh].node;
            edge_iter++; // next edge
            dfs(current_node->neigh[neigh].node); // recursive call
            neigh_node->visited = false; // set visited flag to false as Mr. Fortuna said
            return_counter++; // we come back from dfs so return counter should be incremented
        }
    }
    return path_iter + 1;
}

size_t graph_get_neigh_cost(node_T *node, size_t neigh_node_number) {
    for (size_t neigh = 0; neigh < MAX_NEIGH; neigh++) {
        if (node->neigh[neigh].node == neigh_node_number) {
            return node->neigh[neigh].cost;
        }
    }
    return -1;
}

void cost_filtering(size_t path_iter) {
    size_t edge = 0;
    node_T *current_node;

    for (size_t path = 0U; path < path_iter; path++) {  // iterate through all paths
        path_with_cost_T *current_path = &paths_with_cost[path];
        for (edge = 0U; edge < MAX_NEIGH &&
            ({graph_get_node(current_path->edge[edge].edge_node_x, &current_node); (paths_with_cost[path].cost + graph_get_neigh_cost(current_node, current_path->edge[edge].edge_node_y) <= MAX_COST);});
            edge++) { // iterate through all edge
            current_path->cost += graph_get_neigh_cost(current_node, current_path->edge[edge].edge_node_y);
        }
        current_path->number_of_visited_chambers = edge;
        memset(&current_path->edge[edge], 0, (MAX_NEIGH - edge) * sizeof(edge_T));  // why? kuuuuurwea
    }
}

int main(void) {
    size_t path_counter = dfs(1);

    printf("Available paths before cost consideration:\n");
    for (size_t path = 0U; path < path_counter; path++) {
        path_with_cost_T *current_path = &paths_with_cost[path];
        printf("%lu. ", path + 1);
        /* Node value cannot be equal zero */
        for(size_t edge = 0; (current_path->edge[edge].edge_node_x != 0) && (edge < MAX_NEIGH); edge++) {
            printf("%lu -> %lu; ", current_path->edge[edge].edge_node_x, current_path->edge[edge].edge_node_y);
        }
        printf("\n");
    }

    cost_filtering(path_counter);

    printf("\n\n\nAvailable paths after cost consideration:\n");
    size_t max_number_of_visited_chambers = 0;
    for (size_t path = 0U; path < path_counter; path++) {
        path_with_cost_T *current_path = &paths_with_cost[path];
        printf("%lu. ", path + 1);
        /* Node value cannot be equal zero */
        for(size_t edge = 0; (current_path->edge[edge].edge_node_x != 0) && (edge < MAX_NEIGH); edge++) {
            printf("%lu -> %lu; ", current_path->edge[edge].edge_node_x, current_path->edge[edge].edge_node_y);
        }

        if (max_number_of_visited_chambers < current_path->number_of_visited_chambers) {
            max_number_of_visited_chambers = current_path->number_of_visited_chambers;
        }
        printf("Cost of this path: %lu\n", current_path->cost);
    }

    printf("\n\n\nBest path/s: \n");
    for (size_t path = 0U; path < path_counter; path++) {
        path_with_cost_T *current_path = &paths_with_cost[path];
        if (current_path->number_of_visited_chambers == max_number_of_visited_chambers) {
            printf("%lu. ", path + 1);
        }
    }

    return 0;
}
