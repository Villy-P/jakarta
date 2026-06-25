#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "data_structures/array.h"

static const int ARRAY_CAPACITY_INCREASE_MULTIPLIER = 2;
static const int ARRAY_INITIAL_SIZE_DEFAULT = 4;

Array* create_array(size_t initial_size) {
    if (initial_size == 0) { 
        initial_size = ARRAY_INITIAL_SIZE_DEFAULT;
    }
    Array* array = malloc(sizeof(Array));
    if (!array) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Array");
        return nullptr;
    }
    array->data = (void**)malloc(initial_size * sizeof(void*));
    if (!array->data) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Array data\n");
        free(array);
        return nullptr;
    }

    array->length = 0;
    array->capacity = initial_size;
    return array;
}

bool init_array(Array* array, size_t initial_size) {
    if (!array) {
        return false;
    }
    size_t size = (initial_size == 0) ? ARRAY_INITIAL_SIZE_DEFAULT : initial_size;

    array->data = (void**)malloc(size * sizeof(void*));
    if (!array->data) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Array data\n");
        return false;
    }

    array->length = 0;
    array->capacity = initial_size;
    return true;
}

void ensure_capacity(Array* array) {
    if (array->capacity == 0) {
        array->capacity = ARRAY_INITIAL_SIZE_DEFAULT;
        array->data = (void**)malloc(array->capacity * sizeof(void*));
        if (!array->data) {
            jakarta_error(ERR_CUSTOM, nullptr, "Failed to increase Array Capacity");
        }
    } else if (array->length >= array->capacity) {
        void* temp = realloc((void*)array->data, array->capacity * 2 * sizeof(void*));
        if (!temp) {
            jakarta_error(ERR_CUSTOM, nullptr, "Failed to increase Array Capacity");
            return;
        }
        array->data = (void**)temp;
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
        jakarta_error(ERR_CUSTOM, nullptr, "Null Array Exception");
        return nullptr;
    }
    if (index >= array->length) {
        jakarta_error(ERR_CUSTOM, nullptr, "Index out of bounds");
        return nullptr;
    }
    void** data_ptr = array->data;
    return data_ptr[index];
}

void remove_from_array(Array* array, size_t index) {
    if (!array) {
        jakarta_error(ERR_CUSTOM, nullptr, "Null Array Exception");
        return;
    }
    if (index >= array->length) {
        jakarta_error(ERR_CUSTOM, nullptr, "Index out of bounds");
        return;
    }

    void** data_ptr = array->data;
    
    size_t num_elements_to_move = (array->length - 1) - index;

    if (num_elements_to_move > 0) {
        memmove((void*)&data_ptr[index], (void*)&data_ptr[index + 1], num_elements_to_move * sizeof(void*));
    }
    
    array->length--;
}

void free_array(Array* array) {
    free((void*)array->data);
    free(array);
}