#include <stddef.h>
#include <stdlib.h>

#include "stack.h"

Stack* create_stack(int memberSize, int totalElements) {
    Stack *s = malloc(sizeof(Stack));
    s->top = -1;
    s->memberSize = memberSize;
    s->totalElements = totalElements;
    s->data = malloc(totalElements * memberSize);
    return s;
}

int push_to_stack(Stack* s, void* data) {
    if (s->top == s->totalElements - 1)
        expand_stack(s);
    s->top++;
    void* target = (char*)s->data + (s->top * s->memberSize);
    memcpy(target, data, s->memberSize);
    return 0;
}

int expand_stack(Stack* s) {
    s->data = realloc(s->data, s->totalElements * 2 * s->memberSize);
    s->totalElements *= 2;
    return 0;
}

int pop_from_stack(Stack* s, void* target) {
    if (s->top == -1)
        return 1;
    void* source = (char*)s->data + (s->top * s->memberSize);
    s->top--;
    memcpy(target, source, s->memberSize);
    return 0;
}
