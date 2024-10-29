#include "stack.h"
#include <stddef.h>

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

STACK_TYPE pop_from_stack(Stack* stack) {
    if (stack->head == NULL)
        return 0;
    StackNode* temp_node = stack->head;
    STACK_TYPE value = temp_node->content;
    stack->head = stack->head->next;
    free(temp_node);
    stack->size--;
    return value;
}
