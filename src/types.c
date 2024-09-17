#include <stdlib.h>

#include "types.h"

Identifier** types = NULL;
unsigned int max_type_length = INITIAL_TYPES_LENGTH;
unsigned int current_type_length = 0;

void create_base_types() {
    if (types == NULL)
        types = malloc(sizeof(Identifier*) * INITIAL_TYPES_LENGTH);
}