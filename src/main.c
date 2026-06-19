#include <string.h>

#include "data_structures/symbol_table.h"
#include "core.h"
#include "cmd.h"
#include "types.h"
#include "debug.h"

int main(int argc, char *argv[]) {
    setup_logs();
    symbol_table_init();
    log_msg(logs.main, "[PROGRAM] Program started with %d arguments\n", argc);

    SetUnhandledExceptionFilter(handle_seg_fault);
    log_msg(logs.main, "[PROGRAM] Set up segmentation fault handler\n");

    CmdArgs args = {0};
    parse_args(argc, argv, &args);

    create_base_types();
    add_built_in_functions();

    jakarta_cmd_read_file(args.input_file);
    jakarta_cmd_out_file(args.output_file);

    cleanup_logs();
    log_msg(logs.main, "[PROGRAM] Program finished\n");
}