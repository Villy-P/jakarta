#include <string.h>

#include "core.h"
#include "cmd.h"
#include "types.h"
#include "debug.h"


int main(int argc, char *argv[]) {
    debug_message("Started Program", TOP_LEVEL);

    CmdArgs args = {0};
    parse_args(argc, argv, &args);

    jakarta_cmd_read_file(args.input_file);
    jakarta_cmd_out_file(args.output_file);
}