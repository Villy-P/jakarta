#include <stdlib.h>
#include <stdio.h>

#include "data_structures/array.h"
#include "core.h"

#define ARRAY_CAPACITY_INCREASE_MULTIPLIER 2
#define ARRAY_INIITIAL_SIZE_DEFAULT 4

Array* create_array(unsigned int initial_size) {
    if (initial_size == 0) initial_size = ARRAY_INIITIAL_SIZE_DEFAULT;
    Array* array = malloc(sizeof(Array));
    if (!array)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Array");
    array->data = malloc(initial_size * sizeof(void*));
    if (!array->data)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Array data\n");

    array->length = 0;
    array->capacity = initial_size;
    return array;
}

void add_to_array(Array* array, void* data) {
    if (array->capacity == 0) {
        array->capacity = ARRAY_INIITIAL_SIZE_DEFAULT;
        array->data = malloc(array->capacity * sizeof(void*));
        if (!array->data)
            jakarta_error(ERR_CUSTOM, NULL, "Failed to increase Array Capacity");
    } else if (array->length == array->capacity) {
        void** temp = realloc(array->data, array->capacity * 2 * sizeof(void*));
        if (!temp)
            jakarta_error(ERR_CUSTOM, NULL, "Failed to increase Array Capacity");
        array->data = temp;
        array->capacity *= ARRAY_CAPACITY_INCREASE_MULTIPLIER;
    }
    array->data[array->length++] = data;
}

void* get_from_array(Array* array, unsigned int index) {
    if (!array)
        jakarta_error(ERR_CUSTOM, NULL, "Null Array Exception");
    if (index >= array->length)
        jakarta_error(ERR_CUSTOM, NULL, "Index out of bounds");
    return array->data[index];
}

void remove_from_array(Array* array, unsigned int index) {
    if (index >= array->length)
        jakarta_error(ERR_CUSTOM, NULL, "Index out of bounds");
    for (unsigned int i = index; i < array->length - 1; ++i)
        array->data[i] = array->data[i + 1];
    array->length--;
}

void free_array(Array* array) {
    free(array->data);
    free(array);
}