#include "data_structures/compiler_state.h"
#include "semantic_analyzer.h"

bool are_types_compatible(TypeRegistryEntry* type1, TypeRegistryEntry* type2) {
    if (type1 == nullptr || type2 == nullptr) {
        return false;
    }
    if (type1->option != type2->option) {
        return false;
    }
    return true;
}
