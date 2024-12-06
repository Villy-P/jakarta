#ifndef __JAKARTA_STACK_H__
#define __JAKARTA_STACK_H__

#include "ast.h"

#define STACK_TYPE ASTNode*

typedef struct StackNodeDef {
    STACK_TYPE content;
    struct StackNodeDef* next;
} StackNode;

typedef struct StackDef {
    unsigned short size;
    StackNode* head;
} Stack;

Stack* create_stack();
void add_to_stack(Stack* stack, STACK_TYPE value);
STACK_TYPE pop_from_stack(Stack* stack);
void print_stack(Stack* stack);

#endif