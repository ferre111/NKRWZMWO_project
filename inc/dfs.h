#ifndef DFS_H
#define DFS_H

#include <stdlib.h>
#include <stdbool.h>

#define NODES_MAX_NUMBER 20
#define MAX_NEIGH 10
#define NODE_NUMBER_POWER 3125

#define MAX_COST 8

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

extern path_with_cost_T paths_with_cost[NODE_NUMBER_POWER];
extern node_T graph[NODES_MAX_NUMBER];

size_t dfs(size_t node_number, node_T *graph);
void cost_filtering(size_t path_iter, node_T *graph);

#endif