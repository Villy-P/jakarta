#include <stddef.h>
#include <stdlib.h>

#include "stack.h"

Stack* create_stack() {
    Stack* stack = malloc(sizeof(Stack));
    stack->size = 0;
    stack->head = NULL;
    return stack;
}

void add_to_stack(Stack* stack, STACK_TYPE value) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    new_node->content = value;
    new_node->next = (stack->head == NULL) ? NULL : stack->head;
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

void print_stack(Stack* stack) {
    StackNode* current = stack->head;
    while (current != NULL) {
        printf("%s ", current->content->token->content);
        current = current->next;
    }
    printf("\n");
}
