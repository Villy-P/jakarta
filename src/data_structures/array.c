#include <stdlib.h>
#include <stdio.h>

#include "data_structures/array.h"

Array* create_array(unsigned int initial_size) {
    Array* array = (Array*)malloc(sizeof(Array));
    array->data = (void**)malloc(initial_size * sizeof(void*));
    array->length = 0;
    array->capacity = initial_size;
    return array;
}

void add_to_array(Array* array, void* data) {
    if (array->length == array->capacity) {
        array->capacity *= 2;
        array->data = (void**)realloc(array->data, array->capacity * sizeof(void*));
    }
    array->data[array->length] = data;
    array->length++;
}

void* get_from_array(Array* array, unsigned int index) {
    if (!array) {
        fprintf(stderr, "Error: Null array pointer\n");
        return NULL;
    }
    if (index >= array->length) {
        fprintf(stderr, "Index out of bounds\n");
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