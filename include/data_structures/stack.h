#pragma once

#define STACK_EMPTY -1
#define STACK_OK 0
#define STACK_ERR_ALLOC 1
#define STACK_ERR_EMPTY 2
#define STACK_ERR_NULL 3

typedef struct StackDef {
    void* data;
    int top;
    int total_elements;
    int member_size;
} Stack;

Stack* create_stack(int member_size, int total_elements);
int push_to_stack(Stack* s, void* data);
int expand_stack(Stack* s);
int pop_from_stack(Stack* s, void* target);
void reverse_stack(Stack* s);
int get_stack_index(Stack* s, int index, void* target);