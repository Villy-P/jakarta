#include <string.h>

#include "data_structures/symbol_table.h"
#include "core.h"
#include "cmd.h"
#include "types.h"
#include "debug.h"

int main(int argc, char *argv[]) {
    setup_logs();
    symbol_table_init();
    fprintf(logs.main, "[PROGRAM] Program started with %d arguments\n", argc);

    CmdArgs args = {0};
    parse_args(argc, argv, &args);

    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    jakarta_cmd_read_file(args.input_file, tokenizer);
    jakarta_cmd_out_file(args.output_file);

    cleanup_logs();
    fprintf(logs.main, "[PROGRAM] Program finished\n");
}