#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/stack.h"

Stack* create_stack(int member_size, int total_elements) {
    Stack *s = malloc(sizeof(Stack));
    s->top = -1;
    s->member_size = member_size;
    s->total_elements = total_elements;
    s->data = malloc(total_elements * member_size);
    return s;
}

int push_to_stack(Stack* s, void* data) {
    if (s->top == s->total_elements - 1)
        expand_stack(s);
    s->top++;
    void* target = (char*)s->data + (s->top * s->member_size);
    memcpy_s(target, s->member_size, data, s->member_size);
    return 0;
}

int expand_stack(Stack* s) {
    s->data = realloc(s->data, s->total_elements * 2 * s->member_size);
    s->total_elements *= 2;
    return 0;
}

int pop_from_stack(Stack* s, void* target) {
    if (s->top == -1)
        return 1;
    void* source = (char*)s->data + (s->top * s->member_size);
    s->top--;
    memcpy_s(target, s->member_size, source, s->member_size);
    return 0;
}

void reverse_stack(Stack* s) {
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
