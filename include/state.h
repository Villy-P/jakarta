#pragma once

#include "ds/containers.h"

typedef struct {
    const char* file_name;
    ds_compile_error_ptr_array errors;
} SharedCompilerState;
