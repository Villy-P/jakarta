#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "data_structures/hashmap.h"
#include "types.h"

ClassDefinition* create_class_definition(const char* name) {
    ClassDefinition* class = malloc(sizeof(ClassDefinition));
    if (class == nullptr) {
        jakarta_error(ERR_MALLOC_FAIL, nullptr, "Class");
        free(class);
        return nullptr;
    }
    if (name == nullptr) {
        jakarta_error(ERR_CUSTOM, nullptr, "Class name cannot be null");
        free(class);
        return nullptr;
    }
    class->name = strdup(name);
    class->member_variables = create_hashmap();
    class->member_functions = create_hashmap();
    return class;
}

