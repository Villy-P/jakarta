#include <stdlib.h>

#include "free.h"
#include "types.h"

void free_type_data() {
    for (unsigned short i = 0; i < current_type_length; i++)
        free(types[i]);
    free(types);
}