#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "data_structures/stack.h"
#include "core.h"

#define STACK_OK 0
#define STACK_ERR_ALLOC 1
#define STACK_ERR_EMPTY 2
#define STACK_ERR_NULL 3

Stack* create_stack(int member_size, int total_elements) {
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Stack");
    s->top = -1;
    s->member_size = member_size;
    s->total_elements = total_elements;
    s->data = calloc(total_elements, member_size);
    if (s->data == NULL)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Stack Data");
    return s;
}

int expand_stack(Stack* s) {
    if (!s)
        jakarta_error(ERR_CUSTOM, NULL, "Stack is NULL");

    int new_capacity = s->total_elements * 2;
    void* new_data = realloc(s->data, new_capacity * s->member_size);
    if (!new_data)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Stack Data Expansion");

    s->data = new_data;
    s->total_elements = new_capacity;
    return STACK_OK;
}

int push_to_stack(Stack* s, void* data) {
    if (!s || !data)
        jakarta_error(ERR_CUSTOM, NULL, "Stack or data is NULL");

    if (s->top == s->total_elements - 1) {
        int result = expand_stack(s);
        if (result != STACK_OK) return result;
    }

    s->top++;
    void* target = (char*)s->data + s->top * s->member_size;

    memcpy(target, data, s->member_size);

    return STACK_OK;
}

int pop_from_stack(Stack* s, void* target) {
    if (!s)
        jakarta_error(ERR_CUSTOM, NULL, "Stack is NULL");
    if (s->top == -1)
        jakarta_error(ERR_CUSTOM, NULL, "Stack is empty");

    void* source = (char*)s->data + s->top * s->member_size;
    s->top--;

    if (target == NULL)
        return STACK_OK;
    memcpy(target, source, s->member_size);
    return STACK_OK;
}

int get_stack_index(Stack* s, int index, void* target) {
    if (!s || !target)
        jakarta_error(ERR_CUSTOM, NULL, "Stack or target is NULL");
    if (s->top == -1)
        jakarta_error(ERR_CUSTOM, NULL, "Stack is empty");

    void* source = (char*)s->data + index * s->member_size;

    memcpy(target, source, s->member_size);

    return STACK_OK;
}

void reverse_stack(Stack* s) {
    if (s->top <= 0)
        return;
    void* temp = malloc(s->member_size);
    if (temp == NULL)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Temporary memory for stack reversal");
    int start = 0;
    int end = s->top;
    while (start < end) {
        void* startPtr = (char*)s->data + (start * s->member_size);
        void* endPtr = (char*)s->data + (end * s->member_size);
        memcpy(temp, startPtr, s->member_size);
        memcpy(startPtr, endPtr, s->member_size);
        memcpy(endPtr, temp, s->member_size);
        start++;
        end--;
    }
    free(temp);
}
