#include <stdlib.h>
#include <stdio.h>

#include "data_structures/array.h"

Array* create_array(unsigned int initial_size) {
    if (initial_size == 0) initial_size = 4;
    Array* array = malloc(sizeof(Array));
    if (!array) {
        fprintf(stderr, "Failed to allocate Array struct\n");
        exit(EXIT_FAILURE);
    }
    array->data = malloc(initial_size * sizeof(void*));
    if (!array->data) {
        fprintf(stderr, "Failed to allocate Array data\n");
        free(array);
        exit(EXIT_FAILURE);
    }

    array->length = 0;
    array->capacity = initial_size;
    return array;
}

void add_to_array(Array* array, void* data) {
    if (array->capacity == 0) {
        array->capacity = 4;
        array->data = malloc(array->capacity * sizeof(void*));
    } else if (array->length == array->capacity) {
        void** temp = realloc(array->data, array->capacity * 2 * sizeof(void*));
        if (!temp) {
            fprintf_s(stderr, "Failed to allocate memory for array expansion\n");
            return;
        }
        array->data = temp;
        array->capacity *= 2;
    }
    array->data[array->length++] = data;
}

void* get_from_array(Array* array, unsigned int index) {
    if (!array) {
        fprintf_s(stderr, "Error: Null array pointer\n");
        return NULL;
    }
    if (index >= array->length) {
        fprintf_s(stderr, "Index out of bounds\n");
        return NULL;
    }
    return array->data[index];
}

void remove_from_array(Array* array, unsigned int index) {
    if (index >= array->length) {
        printf("Index out of bounds\n");
        return;
    }
    for (unsigned int i = index; i < array->length - 1; ++i)
        array->data[i] = array->data[i + 1];
    array->length--;
}

void free_array(Array* array) {
    free(array->data);
    free(array);
}