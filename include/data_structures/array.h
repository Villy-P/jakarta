#pragma once

#include <stdint.h>

typedef struct ArrayDef {
    void** data;
    size_t length;
    size_t capacity;
} Array;

Array* create_array(size_t initial_size);
bool init_array(Array* array, size_t initial_size);
void add_to_array(Array* array, const void* data);
void* get_from_array(Array* array, size_t index);
void remove_from_array(Array* array, size_t index);
void free_array(Array* array);
void ensure_capacity(Array* array);