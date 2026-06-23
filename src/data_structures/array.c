#include <stdio.h>
#include <stdlib.h>


#include "data_structures/array.h"
#include "core.h"

#define ARRAY_CAPACITY_INCREASE_MULTIPLIER 2
#define ARRAY_INITIAL_SIZE_DEFAULT 4

Array* create_array(size_t initial_size) {
    if (initial_size == 0) { 
        initial_size = ARRAY_INITIAL_SIZE_DEFAULT;
    }
    Array* array = malloc(sizeof(Array));
    if (!array) {
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Array");
    }
    array->data = malloc(initial_size * sizeof(void*));
    if (!array->data) {
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Array data\n");
    }

    array->length = 0;
    array->capacity = initial_size;
    return array;
}

void ensure_capacity(Array* array) {
    if (array->capacity == 0) {
        array->capacity = ARRAY_INITIAL_SIZE_DEFAULT;
        array->data = malloc(array->capacity * sizeof(void*));
        if (!array->data) {
            jakarta_error(ERR_CUSTOM, NULL, "Failed to increase Array Capacity");
        }
    } else if (array->length >= array->capacity) {
        void* temp = realloc(array->data, array->capacity * 2 * sizeof(void*));
        if (!temp) {
            jakarta_error(ERR_CUSTOM, NULL, "Failed to increase Array Capacity");
        }
        array->data = temp;
        array->capacity *= ARRAY_CAPACITY_INCREASE_MULTIPLIER;
    }
}

void add_to_array(Array* array, const void* data) {
    ensure_capacity(array);
    void** data_ptr = array->data;
    data_ptr[array->length] = (void*)data;
    array->length++;
}

void* get_from_array(Array* array, size_t index) {
    if (!array) {
        jakarta_error(ERR_CUSTOM, NULL, "Null Array Exception");
    }
    if (index >= array->length) {
        jakarta_error(ERR_CUSTOM, NULL, "Index out of bounds");
    }
    void** data_ptr = array->data;
    return data_ptr[index];
}

void remove_from_array(Array* array, size_t index) {
    if (!array) {
        jakarta_error(ERR_CUSTOM, NULL, "Null Array Exception");
    }
    if (index >= array->length) {
        jakarta_error(ERR_CUSTOM, NULL, "Index out of bounds");
    }

    void** data_ptr = array->data;
    
    size_t num_elements_to_move = (array->length - 1) - index;

    if (num_elements_to_move > 0) {
        memmove(&data_ptr[index], &data_ptr[index + 1], num_elements_to_move * sizeof(void*));
    }
    
    array->length--;
}

void free_array(Array* array) {
    free(array->data);
    free(array);
}