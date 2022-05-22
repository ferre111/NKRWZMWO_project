#ifndef FILE_READ_H
#define FILE_READ_H

#include <stdio.h>
#include <stdint.h>

typedef struct {
    void *pointer;
    size_t x_qty;
    size_t y_qty;
} graph_array_settings;

int32_t read_graph_data(char *file_name, graph_array_settings *settings);

#endif // FILE_READ_H
