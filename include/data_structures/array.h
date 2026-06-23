#pragma once

#include <stdint.h>

#define ARRAY_ALIGNMENT 16

typedef struct alignas(ARRAY_ALIGNMENT) ArrayDef {
    void** data;
    size_t length;
    size_t capacity;
} Array;

Array* create_array(unsigned int initial_size);
void add_to_array(Array* array, void* data);
void* get_from_array(Array* array, size_t index);
void remove_from_array(Array* array, size_t index);
void free_array(Array* array);
void ensure_capacity(Array* array);