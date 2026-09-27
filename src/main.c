#include "core.h"
#include "cli.h"
#include "ds/containers.h"
#include "ds_string.h"
#include "error-handling.h"
#include "state.h"

#define INITIAL_ERROR_CAPACITY 10

int main(int argc, char* argv[]) {
    install_seg_fault_handler();

    SharedCompilerState state = {nullptr, {nullptr, 0, 0, 0, nullptr, nullptr}};
    ds_compile_error_ptr_array_init_default(&state.errors);

    CmdArgs args = {};
    parse_args(argc, argv, &args, &state);

    ds_string_deinit(&args.input_file);
    ds_string_deinit(&args.output_file);

    print_all_errors(&state.errors);
    return 0;
}
