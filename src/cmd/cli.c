#include "cmd.h"
#include "core.h"

static void set_input_file(CmdArgs* args, const char* value) {
    if (!value) return handle_error(ERROR_INVALID_FILE_LOCATION, NULL, NULL, INPUT_FILE_ARGUMENT);
    strncpy(args->input_file, value, FILE_NAME_SIZE - 1);
    args->input_file[FILE_NAME_SIZE - 1] = '\0';
}

static void set_output_file(CmdArgs* args, const char* value) {
    if (!value) return handle_error(ERROR_INVALID_FILE_LOCATION, NULL, NULL, OUTPUT_FILE_ARGUMENT);
    strncpy(args->output_file, value, FILE_NAME_SIZE - 1);
    args->output_file[FILE_NAME_SIZE - 1] = '\0';
}

void parse_args(int argc, char* argv[], CmdArgs* args) {
    for (int i = ARGUMENT_START_INDEX; i < argc; i++) {
        if (strcmp(argv[i], INPUT_FILE_ARGUMENT) == 0)
            set_input_file(args, argv[i + 1]);
        else if (strcmp(argv[i], OUTPUT_FILE_ARGUMENT) == 0)
            set_output_file(args, argv[i + 1]);
    }
}