#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i == argc - 1)
                jakarta_error_no_file_location("-f");
            char* file_location = argv[i + 1];
            FILE* file_ptr = fopen(file_location, "r");
            if (file_ptr == NULL)
                jakarta_error_file_does_not_exist(file_location);

            i++;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i == argc - 1)
                jakarta_error_no_file_location("-o");
        }
    }
}