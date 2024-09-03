#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "cmd.h"

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0)
            jakarta_cmd_read_file(i++, argc, argv[i + 1]);
        else if (strcmp(argv[i], "-o") == 0)
            jakarta_cmd_out_file(i++, argc, argv[i + 1]);
    }
}