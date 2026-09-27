#pragma once

#include "ds_string.h"
#include <stdint.h>

typedef struct {
    ds_string file_name;
    int32_t line;
    int32_t col;
} Token;
