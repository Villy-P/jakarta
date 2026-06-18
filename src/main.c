#include <string.h>

#include "core.h"
#include "cmd.h"
#include "types.h"
#include "debug.h"


int main(int argc, char *argv[]) {
    debug_message("Started Program", TOP_LEVEL);
    setup_logs();

    CmdArgs args = {0};
    parse_args(argc, argv, &args);

    Tokenizer* tokenizer = create_tokenizer(INITIAL_TOKENS_LENGTH);
    jakarta_cmd_read_file(args.input_file, tokenizer);
    jakarta_cmd_out_file(args.output_file);

    cleanup_logs();
    debug_message("Finished Program", TOP_LEVEL);
}