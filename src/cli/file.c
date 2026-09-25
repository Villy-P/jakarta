#define DS_C_IMPLEMENTATION
#include <libds_c.h>

#include "cli.h"
#include "error-handling.h"
#include "state.h"

void set_input_file(CmdArgs* args, const char* value, SharedCompilerState* state) {
    if (!value) {
        ds_compile_error_ptr_array_push(&state->errors,
            error_invalid_file_location(value));
        return;
    }
    strncpy(args->input_file, value, FILE_NAME_SIZE - 1);
    args->input_file[FILE_NAME_SIZE - 1] = '\0';
}

void set_output_file(CmdArgs* args, const char* value, SharedCompilerState* state) {
    if (!value) {
        ds_compile_error_ptr_array_push(&state->errors,
            error_invalid_file_location(value));
        return;
    }
    strncpy(args->output_file, value, FILE_NAME_SIZE - 1);
    args->output_file[FILE_NAME_SIZE - 1] = '\0';
}
