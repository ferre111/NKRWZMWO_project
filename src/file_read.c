#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "file_read.h"

static int32_t get_size(char *file_name, size_t *x_qty, size_t *y_qty, size_t *max_char_per_line_no);

int32_t read_graph_data(char *file_name, graph_array_settings *settings) {
    FILE * file_ptr;
    char c = 0U;
    size_t x_qty = 0U; 
    size_t y_qty = 0U;
    size_t max_char_per_line_no = 0U;
    
    if (NULL == settings || NULL == file_name) {
        return 1;
    }

    if(get_size(file_name, &x_qty, &y_qty, &max_char_per_line_no)) {
        return 1;
    }
    
    file_ptr = fopen(file_name, "r");

    if (NULL == file_ptr) {
        printf("Unable to open file %s\n", file_name);
        return 1;
    }

    uint32_t (*graph_array)[y_qty][x_qty];
    char line[max_char_per_line_no];
    char *element;

    graph_array = calloc(1, sizeof(*graph_array));

    if (NULL == graph_array) {
        fclose(file_ptr);
        free(graph_array);
        return 1;
    }

    for(size_t y = 0U; y < y_qty; y++) {
        if(NULL == fgets((char *)line, max_char_per_line_no, file_ptr)){
            fclose(file_ptr);
            free(graph_array);
            return 1;
        }
        
        element = strtok((char *)line, ";");
        for(size_t x = 0U; x < x_qty; x++){
            (*graph_array)[y][x] = atoi((char*)element);
            element = strtok(NULL, ";");
            if (NULL == element) {
                fclose(file_ptr);
                free(graph_array);
                return 1;
            }
        }
    }

    settings->pointer = (void*)graph_array;
    settings->x_qty = x_qty;
    settings->y_qty = y_qty;

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
