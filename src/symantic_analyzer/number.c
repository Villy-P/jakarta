#include "semantic_analyzer.h"
#include <math.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Type* get_raw_number_type(char* number) {
    int num = atoi(number);
    return NULL;
}

// TODO: Refactor (theres probably a much better way to do this)
long long string_to_number(char* str) {
    long long number = 0;
    for (long long i = strlen(str) - 1; i >= 0; i--)
        number += (str[i] - 48) * (pow(10, strlen(str) - i - 1));
    return number;
}