#pragma once

#include "state.h"
#define FILE_NAME_SIZE 256

typedef struct {
    char input_file[FILE_NAME_SIZE];
    char output_file[FILE_NAME_SIZE];
} CmdArgs;

void parse_args(int argc, char* argv[], CmdArgs* args,
                SharedCompilerState* state);
void set_output_file(CmdArgs* args, const char* value, SharedCompilerState* state);
void set_input_file(CmdArgs* args, const char* value, SharedCompilerState* state);
