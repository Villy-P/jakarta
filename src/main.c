#ifdef __WIN32
#include <errhandlingapi.h>
#endif

#include "cmd.h"
#include "core.h"
#include "data_structures/compiler_state.h"
#include "data_structures/tokenizer.h"
#include "debug.h"

int main(int argc, char* argv[]) {
    setup_logs();
    log_msg(logs.main, "[PROGRAM] Program started with %d arguments", argc);

    #ifdef __WIN32
    SetUnhandledExceptionFilter(handle_seg_fault);
    #else
    install_seg_fault_handler();
    #endif
    log_msg(logs.main, "[PROGRAM] Set up segmentation fault handler");

    CmdArgs args = {0};
    parse_args(argc, argv, &args);

    CompilerState* state = create_compiler_state();

    create_base_types(state);
    add_built_in_functions(state);

    jakarta_cmd_read_file(args.input_file, state);
    jakarta_cmd_out_file(args.output_file);

    log_msg(logs.main, "[PROGRAM] Program finished");
    cleanup_logs();
}