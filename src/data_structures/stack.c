#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "data_structures/stack.h"

#define STACK_OK 0
#define STACK_ERR_ALLOC 1
#define STACK_ERR_EMPTY 2
#define STACK_ERR_NULL 3

Stack* create_stack(int member_size, int total_elements) {
    Stack *s = malloc(sizeof(Stack));
    s->top = -1;
    s->member_size = member_size;
    s->total_elements = total_elements;
    s->data = calloc(total_elements, member_size);
    if (s->data == NULL)
        printf("Failed to allocate memory for stack\n");
    return s;
}

int expand_stack(Stack* s) {
    if (!s) return STACK_ERR_NULL;

    int new_capacity = s->total_elements * 2;
    void* new_data = realloc(s->data, new_capacity * s->member_size);
    if (!new_data) {
        printf("Failed to expand stack\n");
        return STACK_ERR_ALLOC;
    }

    s->data = new_data;
    s->total_elements = new_capacity;
    return STACK_OK;
}

int push_to_stack(Stack* s, void* data) {
    if (!s || !data) return STACK_ERR_NULL;

    if (s->top == s->total_elements - 1) {
        int result = expand_stack(s);
        if (result != STACK_OK) return result;
    }

    s->top++;
    void* target = (char*)s->data + s->top * s->member_size;

    errno_t err = memcpy_s(target, s->member_size, data, s->member_size);
    if (err != 0) return err;

    return STACK_OK;
}

int pop_from_stack(Stack* s, void* target) {
    if (!s || !target) return STACK_ERR_NULL;
    if (s->top == -1) return STACK_ERR_EMPTY;

    void* source = (char*)s->data + s->top * s->member_size;
    s->top--;

    errno_t err = memcpy_s(target, s->member_size, source, s->member_size);
    if (err != 0) return err;

    return STACK_OK;
}

int get_stack_index(Stack* s, int index, void* target) {
    if (!s || !target) return STACK_ERR_NULL;
    if (s->top == -1) return STACK_ERR_EMPTY;

    void* source = (char*)s->data + index * s->member_size;

    errno_t err = memcpy_s(target, s->member_size, source, s->member_size);
    if (err != 0) return err;

    return STACK_OK;
}

void reverse_stack(Stack* s) {
    if (s->top <= 0)
        return;
    void* temp = malloc(s->member_size);
    int start = 0;
    int end = s->top;
    while (start < end) {
        void* startPtr = (char*)s->data + (start * s->member_size);
        void* endPtr = (char*)s->data + (end * s->member_size);
        memcpy_s(temp, s->member_size, startPtr, s->member_size);
        memcpy_s(startPtr, s->member_size, endPtr, s->member_size);
        memcpy_s(endPtr, s->member_size, temp, s->member_size);
        start++;
        end--;
    }
    free(temp);
}
