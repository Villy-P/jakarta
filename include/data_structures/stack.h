#pragma once

#include <stddef.h>

#define STACK_EMPTY -1
#define STACK_OK 0
#define STACK_ERR_ALLOC 1
#define STACK_ERR_EMPTY 2
#define STACK_ERR_NULL 3

typedef struct StackDef {
    void* data;
    size_t top;
    size_t total_elements;
    size_t member_size;
} Stack;

Stack* create_stack(size_t member_size, size_t total_elements);
bool push_to_stack(Stack* stack, void* data);
bool expand_stack(Stack* stack);
bool pop_from_stack(Stack* stack, void* target);
void reverse_stack(Stack* stack);
bool get_stack_index(Stack* stack, size_t index, void* target);