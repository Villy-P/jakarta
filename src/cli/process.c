#include "cli.h"
#include <string.h>

#define INPUT_FILE_ARGUMENT "-f"
#define OUTPUT_FILE_ARGUMENT "-o"

void parse_args(int argc, char* argv[], CmdArgs* args, SharedCompilerState* state) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], INPUT_FILE_ARGUMENT) == 0) {
            set_input_file(args, argv[++i], state);
        } else if (strcmp(argv[i], OUTPUT_FILE_ARGUMENT) == 0) {
            set_output_file(args, argv[++i], state);
        }
    }
}
