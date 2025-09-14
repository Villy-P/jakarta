#pragma once

#include <stdio.h>

#define STRING_EQUAL 0
#define FILE_NAME_SIZE 256
#define INPUT_FILE_ARGUMENT "-f"
#define OUTPUT_FILE_ARGUMENT "-o"
#define ARGUMENT_START_INDEX 1

#define STRING_BUFFER_LENGTH 256

typedef struct {
    char input_file[FILE_NAME_SIZE];
    char output_file[FILE_NAME_SIZE];
} CmdArgs;

void jakarta_cmd_read_file(const char* file_location);
void jakarta_cmd_out_file(const char* file_location);

void parse_args(int argc, char* argv[], CmdArgs* args);