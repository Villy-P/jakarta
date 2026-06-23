#include "semantic_analyzer.h"
#include <string.h>

bool is_numeric_type(char* type_name) {
    return is_integer_type(type_name) || is_decimal_type(type_name);
}

bool is_integer_type(char* type_name) {
    return strcmp(type_name, "byte")  == 0 ||
           strcmp(type_name, "short") == 0 ||
           strcmp(type_name, "int")   == 0 ||
           strcmp(type_name, "long")  == 0 ||
           strcmp(type_name, "llong") == 0;
}

bool is_decimal_type(char* type_name) {
    return strcmp(type_name, "float")  == 0 ||
           strcmp(type_name, "double") == 0;
}


// TODO(Valerius Petrini): Refactor
bool is_decimal(char* number) {
    for (unsigned long long i = 0; i < strlen(number); i++) {
        if (number[i] == '.') {
            return true;
        }
    }
    return false;
} 