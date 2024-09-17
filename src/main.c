#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "cmd.h"
#include "types.h"

#define STRING_EQUAL 0
#define FILE_NAME_SIZE 256

int main(int argc, char *argv[]) {
    create_base_types();

    char input_file[FILE_NAME_SIZE];
    char output_file[FILE_NAME_SIZE];
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == STRING_EQUAL) {
            if (i == argc - 1)
                jakarta_error_no_file_location("-f");
            strncpy(input_file, argv[i + 1], FILE_NAME_SIZE);
        } else if (strcmp(argv[i], "-o") == STRING_EQUAL) {
            if (i == argc - 1)
                jakarta_error_no_file_location("-o");
            strncpy(output_file, argv[i + 1], FILE_NAME_SIZE);
        }
    }
    jakarta_cmd_read_file(input_file);
    jakarta_cmd_out_file(output_file);
}