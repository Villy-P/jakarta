#include "ds_string.h"

#include "cli.h"
#include "error-handling.h"
#include "state.h"

void set_input_file(CmdArgs* args, const char* value, SharedCompilerState* state) {
    if (!value) {
        ds_compile_error_ptr_array_push(&state->errors,
            error_invalid_file_location("-f"));
        return;
    }
    ds_string_init(&args->input_file, value);
}

void set_output_file(CmdArgs* args, const char* value, SharedCompilerState* state) {
    if (!value) {
        ds_compile_error_ptr_array_push(&state->errors,
            error_invalid_file_location("-o"));
        return;
    }
    ds_string_init(&args->output_file, value);
}
