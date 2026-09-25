#pragma once

typedef struct CompilerErrorDef CompileError;

#include "ds_array.h"

DS_DEFINE_ARRAY(CompileError*, ds_compile_error_ptr_array, nullptr, nullptr)
