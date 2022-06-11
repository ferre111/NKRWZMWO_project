#include "dfs.h"
#include <string.h>

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

static int graph_get_node(size_t node_number, node_T **node_ptr) {
    for (size_t node = 0; node < NODES_NUMBER; node++) {
        if (node_number == graph[node].base_node) {
            *node_ptr = &graph[node];
            return 0;
        }
    }
    return -1;
}

size_t dfs(size_t node_number, node_T *graph) {
    static size_t edge_iter = 0;
    static size_t path_iter = 0;
    static size_t return_counter = 0;
    node_T *current_node;

    // find current node in graph array
    graph_get_node(node_number, &current_node);
    current_node->visited = true;
    
    node_T *neigh_node;
    // Warning! This algorithm check number of neighbors of individual nodes based on fact that each neigh_T element should have cost diffrent than zero, so all node must have at least one empty element in the graph array.
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
            dfs(current_node->neigh[neigh].node, graph); // recursive call
            neigh_node->visited = false; // set visited flag to false as Mr. Fortuna said
            return_counter++; // we come back from dfs so return counter should be incremented
        }
    }
    return path_iter + 1;
}

static size_t graph_get_neigh_cost(node_T *node, size_t neigh_node_number) {
    for (size_t neigh = 0; neigh < MAX_NEIGH; neigh++) {
        if (node->neigh[neigh].node == neigh_node_number) {
            return node->neigh[neigh].cost;
        }
    }
    return -1;
}

static int cost_has_been_exceeded(size_t x_node_number, size_t y_node_number, node_T **node_ptr, size_t current_cost) {
    graph_get_node(x_node_number, node_ptr);
    if (current_cost + graph_get_neigh_cost(*node_ptr, y_node_number) <= MAX_COST) {
        return 0;
    }
    return -1;
}

void cost_filtering(size_t path_iter, node_T *graph) {
    size_t edge = 0;
    node_T *current_node;

    for (size_t path = 0U; path < path_iter; path++) {  // iterate through all paths
        path_with_cost_T *current_path = &paths_with_cost[path];
        for (edge = 0U; edge < MAX_NEIGH && !cost_has_been_exceeded(current_path->edge[edge].edge_node_x, current_path->edge[edge].edge_node_y, &current_node, current_path->cost); edge++) { // iterate through all edge
            current_path->cost += graph_get_neigh_cost(current_node, current_path->edge[edge].edge_node_y);
        }
        current_path->number_of_visited_chambers = edge;
        memset(&current_path->edge[edge], 0, (MAX_NEIGH - edge) * sizeof(edge_T));  // why? kuuuuurwea
    }
}
