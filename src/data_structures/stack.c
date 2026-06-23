#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/stack.h"
#include "core.h"

Stack* create_stack(int64_t member_size, int64_t total_elements) {
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Stack");
    }
    stack->top = STACK_EMPTY;
    stack->member_size = member_size;
    stack->total_elements = total_elements;
    stack->data = calloc(total_elements, member_size);
    if (stack->data == NULL) {
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Stack Data");
    }
    return stack;
}

bool expand_stack(Stack* stack) {
    if (!stack) {
        jakarta_error(ERR_CUSTOM, NULL, "Stack is NULL");
    }

    int64_t new_capacity = stack->total_elements * 2;
    void* new_data = realloc(stack->data, new_capacity * stack->member_size);
    if (!new_data) {
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Stack Data Expansion");
    }

    stack->data = new_data;
    stack->total_elements = new_capacity;
    return true;
}

bool push_to_stack(Stack* stack, void* data) {
    if (!stack || !data) {
        jakarta_error(ERR_CUSTOM, NULL, "Stack or data is NULL");
    }

    if (stack->top == stack->total_elements - 1) {
        bool result = expand_stack(stack);
        if (!result) {
            return result;
        }
    }

    stack->top++;
    void* target = (char*)stack->data + stack->top * stack->member_size;

    memcpy(target, data, stack->member_size);

    return true;
}

bool pop_from_stack(Stack* stack, void* target) {
    if (!stack) {
        jakarta_error(ERR_CUSTOM, NULL, "Stack is NULL");
    }
    if (stack->top == STACK_EMPTY) {
        jakarta_error(ERR_CUSTOM, NULL, "Stack is empty");
    }

    void* source = (char*)stack->data + stack->top * stack->member_size;
    stack->top--;

    if (target == NULL) {
        return true;
    }
    memcpy(target, source, stack->member_size);
    return true;
}

bool get_stack_index(Stack* stack, int64_t index, void* target) {
    if (!stack || !target) {
        jakarta_error(ERR_CUSTOM, NULL, "Stack or target is NULL");
    }
    if (stack->top == STACK_EMPTY) {
        jakarta_error(ERR_CUSTOM, NULL, "Stack is empty");
    }

    void* source = (char*)stack->data + index * stack->member_size;

    memcpy(target, source, stack->member_size);

    return true;
}

void reverse_stack(Stack* stack) {
    if (stack->top <= 0) {
        return;
    }
    void* temp = malloc(stack->member_size);
    if (temp == NULL) {
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Temporary memory for stack reversal");
    }
    int64_t start = 0;
    int64_t end = stack->top;
    while (start < end) {
        void* startPtr = (char*)stack->data + (start * stack->member_size);
        void* endPtr = (char*)stack->data + (end * stack->member_size);
        memcpy(temp, startPtr, stack->member_size);
        memcpy(startPtr, endPtr, stack->member_size);
        memcpy(endPtr, temp, stack->member_size);
        start++;
        end--;
    }
    free(temp);
}
