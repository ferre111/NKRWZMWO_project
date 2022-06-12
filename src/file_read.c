#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "file_read.h"

#define MAX_FIELD_STRING_LENGTH 10

typedef enum {
    FIELD_FIRST_NODE,
    FIELD_SECOND_NODE,
    FIELD_EDGE_COST

} file_format_field_T;

static int32_t get_size(char *file_name, size_t *x_qty, size_t *y_qty, size_t *max_char_per_line_no);

int32_t read_graph_data(void) {
    FILE * file_ptr;
    file_format_field_T field;
    char c;
    char field_string[MAX_FIELD_STRING_LENGTH] = "";
    size_t node_index;
    size_t neighbour_index;
    unsigned int i;
    char file_name[] = "Graph.txt";

    field = FIELD_FIRST_NODE;
    node_index = 0U;
    neighbour_index = 0U;
    i = 0U;
    file_ptr = fopen(file_name, "r");

    if (NULL == file_ptr) {
        printf("Unable to open file %s\n", file_name);
        return 1;
    }

    for(;;){
        c = fgetc(file_ptr);

        if (c == EOF) {
            break;
        }
        else if (c == '\n') {
            node_index++;
            neighbour_index = 0U;
        }
        else if ((c == ',') || (c == ';')) {
            i = 0U;

            switch (field)
            {
                case FIELD_FIRST_NODE:
                    graph[node_index].base_node = atoi(field_string);
                    memset(field_string, 0u, sizeof(field_string));

                    field = FIELD_SECOND_NODE;
                    break;

                case FIELD_SECOND_NODE:
                    graph[node_index].neigh[neighbour_index].node = atoi(field_string);
                    memset(field_string, 0u, sizeof(field_string));

                    field = FIELD_EDGE_COST;
                    break;

                case FIELD_EDGE_COST:
                    graph[node_index].neigh[neighbour_index].cost = atoi(field_string);
                    memset(field_string, 0u, sizeof(field_string));

                    field = FIELD_FIRST_NODE;
                    break;

                default:
                    break;
            }

            if (c == ';') {
                neighbour_index++;
            }
        }
        else {
            field_string[i] = c;
            i++;
        }
    }

    fclose(file_ptr);
    return 0;
}

/* Warning! Each line must have exactly the same number of elements. The file must have empty line at the end. 
 * x_qty - x quantity
 * y_qty - y quantity
 * max_char_per_line_no - maximum number of characters per line
 */
static int32_t get_size(char *file_name, size_t *x_qty, size_t *y_qty, size_t *max_char_per_line_no) {
    if (NULL == x_qty || 0U != *x_qty ||
        NULL == y_qty || 0U != *y_qty ||
        NULL == max_char_per_line_no || 0U != *max_char_per_line_no ||
        NULL == file_name) {
        return 1;
    }
    FILE * file_ptr;
    char c = 0;

    file_ptr = fopen(file_name, "r");

    if (NULL == file_ptr) {
        printf("Unable to open file %s, errno: %s\n", file_name, strerror(errno));
        return 1;
    }

    size_t tmp_char_per_line_no = 0U;
    bool first_line = true; // is it the first line?
    for(;;){
        c = fgetc(file_ptr);

        tmp_char_per_line_no++;
        if (c == EOF) { // end of the file
            break;
        } else if (c == '\n') { // end of the line
            first_line = false;
            (*y_qty)++;
            tmp_char_per_line_no++; // additional increment for terminator character 
            if (tmp_char_per_line_no > *max_char_per_line_no) {
                *max_char_per_line_no = tmp_char_per_line_no;
            }
            tmp_char_per_line_no = 0U;
        } else if (c == ';' && first_line){ // end of the element
            (*x_qty)++;
        }
    }

    fclose(file_ptr);
    return 0;
}
