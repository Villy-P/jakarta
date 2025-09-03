#pragma once

typedef struct ArrayDef {
    void** data;
    unsigned int length;
    unsigned int capacity;
} Array;

Array* create_array(unsigned int initial_size);
void add_to_array(Array* array, void* data);
void* get_from_array(Array* array, unsigned int index);
void remove_from_array(Array* array, unsigned int index);
void free_array(Array* array);