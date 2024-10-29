#include "stack.h"

Stack* create_stack() {
    Stack* stack = malloc(sizeof(Stack));
    stack->size = 0;
    return stack;
}
