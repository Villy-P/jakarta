#include <string.h>

#include "error.h"
#include "cmd.h"
#include "types/types.h"
#include "free.h"
#include "debug.h"

#define STRING_EQUAL 0
#define FILE_NAME_SIZE 256
#define INPUT_FILE_ARGUMENT "-f"
#define OUTPUT_FILE_ARGUMENT "-o"

int main(int argc, char *argv[]) {
    debug_message("Started Program", TOP_LEVEL);

    char input_file[FILE_NAME_SIZE];
    char output_file[FILE_NAME_SIZE];
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], INPUT_FILE_ARGUMENT) == STRING_EQUAL) {
            if (i == argc - 1)
                jakarta_error_no_file_location(INPUT_FILE_ARGUMENT);
            debug_message("Found Input File", LOG);
            strncpy(input_file, argv[i + 1], FILE_NAME_SIZE);
        } else if (strcmp(argv[i], OUTPUT_FILE_ARGUMENT) == STRING_EQUAL) {
            if (i == argc - 1)
                jakarta_error_no_file_location(OUTPUT_FILE_ARGUMENT);
            debug_message("Found Output File", LOG);
            strncpy(output_file, argv[i + 1], FILE_NAME_SIZE);
        }
    }
    jakarta_cmd_read_file(input_file);
    jakarta_cmd_out_file(output_file);
}