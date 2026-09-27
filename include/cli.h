#pragma once

#include "ds_string.h"
#include "state.h"

typedef struct {
    ds_string input_file;
    ds_string output_file;
} CmdArgs;

void parse_args(int argc, char* argv[], CmdArgs* args,
                SharedCompilerState* state);
void set_output_file(CmdArgs* args, const char* value, SharedCompilerState* state);
void set_input_file(CmdArgs* args, const char* value, SharedCompilerState* state);
