#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include "file_read.h"
#include "dfs.h"

/* WARNING!
 * Algorithm check number of neighbors of individual nodes based on fact that each neigh_T element should have cost diffrent than zero, so all node must have at least one empty element in the graph array.
 * Node numbering cannot start from zero.
**/

int main(void) {
    size_t path_counter = dfs(1, graph);

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

    cost_filtering(path_counter, graph);

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
