#include "stack.h"

Stack* create_stack() {
    Stack* stack = malloc(sizeof(Stack));
    stack->size = 0;
    return stack;
}

void add_to_stack(Stack* stack, STACK_TYPE value) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    new_node->content = value;
    new_node->next = stack->head;
    stack->head = new_node;
    stack->size++;
}
