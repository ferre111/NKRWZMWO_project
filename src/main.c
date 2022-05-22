#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "file_read.h"

#define GRAPH_FILE_NAME "/home/kwit666/Documents/Karol/Studia/Semestr 8/NKRWZMWO/projekt/graph.txt"

int main(void) {
    graph_array_settings settings;
    if (read_graph_data(GRAPH_FILE_NAME, &settings)) {
        printf("Errno: %s\n", strerror(errno));
        return 1;
    }

    uint32_t (*graph_array)[settings.y_qty][settings.x_qty] = (uint32_t(*)[settings.y_qty][settings.x_qty])settings.pointer;

    for(size_t y = 0U; y < settings.y_qty; y++) {
        for(size_t x = 0U; x < settings.x_qty; x++) {
            printf("%u ", (*graph_array)[y][x]);
        }
        printf("\n");
    }

    free(graph_array);
    return 0;
}
