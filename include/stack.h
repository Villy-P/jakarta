#ifndef __JAKARTA_STACK_H__
#define __JAKARTA_STACK_H__

#include "ast.h"

#define STACK_TYPE ASTNode*

typedef struct StackDef {
    void* data;
    int top;
    int totalElements;
    int memberSize;
} Stack;

Stack* create_stack(int memberSize, int totalElements);
int push_to_stack(Stack* s, void* data);
int expand_stack(Stack* s);
int pop_from_stack(Stack* s, void* target);
// void print_stack(Stack* stack);

#endif