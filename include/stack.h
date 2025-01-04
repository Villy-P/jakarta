#ifndef __JAKARTA_STACK_H__
#define __JAKARTA_STACK_H__

#include "ast.h"

#define STACK_TYPE ASTNode*

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

#endif