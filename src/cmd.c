#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "error.h"
#include "parser.h"

#define STRING_BUFFER_LENGTH 256

void jakarta_cmd_read_file(int i, int argc, char* file_location) {
    if (i == argc - 1)
        jakarta_error_no_file_location("-f");
    FILE* file_ptr = fopen(file_location, "r");
    if (file_ptr == NULL)
        jakarta_error_file_does_not_exist(file_location);
    char buffer[STRING_BUFFER_LENGTH];
    while (fgets(buffer, sizeof(buffer), file_ptr)) {
        read_line(buffer);
    }
    fclose(file_ptr);
}

void jakarta_cmd_out_file(int i, int argc, char* file_location) {
    if (i == argc - 1)
        jakarta_error_no_file_location("-o");
    FILE* file_ptr = fopen(file_location, "w");
    
    fclose(file_ptr);
}