#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f")) {
            printf("Found compiler argument -f");
            if (i == argc - 2) {
                printf("\033[31mDid not find file name for compiler argument -f\033[0m\n");
                exit(1);
            }
        }
    }
}