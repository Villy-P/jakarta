#include <string.h>

#include "core.h"
#include "cmd.h"
#include "types/types.h"
#include "free.h"
#include "debug.h"

#define STRING_EQUAL 0
#define FILE_NAME_SIZE 256
#define INPUT_FILE_ARGUMENT "-f"
#define OUTPUT_FILE_ARGUMENT "-o"
#define ARGUMENT_START_INDEX 1

int main(int argc, char *argv[]) {
    debug_message("Started Program", TOP_LEVEL);

    CmdArgs args = {0};
    parse_args(argc, argv, &args);

    jakarta_cmd_read_file(args.input_file);
    jakarta_cmd_out_file(args.output_file);
}