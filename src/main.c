#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f")) {
            if (i == argc - 2)
                jakarta_error_no_file_location();
            char* file_location = argv[++i];
        }
    }
}