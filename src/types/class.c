#include <stdlib.h>
#include <string.h>

#include "types/class.h"
#include "core.h"

ClassDefinition* create_class_definition(const char* name) {
    ClassDefinition* class = malloc(sizeof(ClassDefinition));
    if (class == NULL)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "Class");
    class->name = malloc(strlen(name) + 1);
    strncpy(class->name, name, strlen(name) + 1);
    class->member_variables = create_hashmap();
    class->member_functions = create_hashmap();
    return class;
}

