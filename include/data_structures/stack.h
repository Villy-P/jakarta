#pragma once

#include <stddef.h>

#include <stdint.h>


#define STACK_EMPTY -1
#define STACK_OK 0
#define STACK_ERR_ALLOC 1
#define STACK_ERR_EMPTY 2
#define STACK_ERR_NULL 3

typedef struct StackDef {
    void* data;
    int64_t top;
    int64_t total_elements;
    int64_t member_size;
} Stack;

Stack* create_stack(int64_t member_size, int64_t total_elements);
bool push_to_stack(Stack* stack, void* data);
bool expand_stack(Stack* stack);
bool pop_from_stack(Stack* stack, void* target);
void reverse_stack(Stack* stack);
bool get_stack_index(Stack* stack, int64_t index, void* target);