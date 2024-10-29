#ifndef __JAKARTA_STACK_H__
#define __JAKARTA_STACK_H__

#define STACK_TYPE int

typedef struct StackDef {
    unsigned short size;
    StackNode* head;
} Stack;

typedef struct StackNodeDef {
    STACK_TYPE content;
    StackNode* next;
} StackNode;

Stack* create_stack();
void add_to_stack(Stack* stack, STACK_TYPE value);
STACK_TYPE pop_from_stack(Stack* stack);

#endif