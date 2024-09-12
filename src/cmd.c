#include <stdlib.h>
#include <stdio.h>

#include "cmd.h"
#include "error.h"
#include "parser.h"

#define STRING_BUFFER_LENGTH 256

void jakarta_cmd_read_file(char* file_location) {
    FILE* file_ptr = fopen(file_location, "r");
    if (file_ptr == NULL)
        jakarta_error_file_does_not_exist(file_location);
    char buffer[STRING_BUFFER_LENGTH];
    while (fgets(buffer, sizeof(buffer), file_ptr)) {
        read_line(buffer);
    }
    if (fclose(file_ptr) != 0)
        jakarta_error_cannot_close_file(file_location);
}

void jakarta_cmd_out_file(char* file_location) {
    FILE* file_ptr = fopen(file_location, "w");
    
    if (fclose(file_ptr) != 0)
        jakarta_error_cannot_close_file(file_location);
}