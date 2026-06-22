#include "semantic_analyzer.h"
#include <string.h>

bool is_numeric(char* type_name) {
    return is_integer(type_name) || is_decimal(type_name);
}

bool is_integer(char* type_name) {
    return strcmp(type_name, "byte")  == 0 ||
           strcmp(type_name, "short") == 0 ||
           strcmp(type_name, "int")   == 0 ||
           strcmp(type_name, "long")  == 0 ||
           strcmp(type_name, "llong") == 0
}

bool is_decimal(char* type_name) {
    return strcmp(type_name, "float")  == 0 ||
           strcmp(type_name, "double") == 0
}
