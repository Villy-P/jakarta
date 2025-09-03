#include "cmd.h"
#include "core.h"

void parse_args(int argc, char* argv[], CmdArgs* args) {
    for (int i = ARGUMENT_START_INDEX; i < argc; i++) {
        if (strcmp(argv[i], INPUT_FILE_ARGUMENT) == 0) {
            if (i == argc - 1)
                jakarta_error(ERR_INVALID_FILE_LOCATION, NULL, INPUT_FILE_ARGUMENT);
            strncpy(args->input_file, argv[i + 1], FILE_NAME_SIZE);
        } else if (strcmp(argv[i], OUTPUT_FILE_ARGUMENT) == 0) {
            if (i == argc - 1)
                jakarta_error(ERR_INVALID_FILE_LOCATION, NULL, OUTPUT_FILE_ARGUMENT);
            strncpy(args->output_file, argv[i + 1], FILE_NAME_SIZE);
        }
    }
}