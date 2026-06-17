#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "debug.h"

LogFiles logs;

void setup_logs() {
    debug_message("Setting up logs", TOP_LEVEL);

    struct stat st = {0};
    if (stat("logs", &st) == -1) {
        if (mkdir("logs") != 0) {
            debug_message("Error: Failed to create 'logs' directory", LOG);
            return;
        }
    }

    logs.tokens = fopen("logs/tokens.log", "w");
    if (logs.tokens == NULL)
        debug_message("Error: Could not create log file", LOG);
    else
        debug_message("Log file created successfully", LOG);

    logs.ast = fopen("logs/ast.log", "w");
    if (logs.ast == NULL)        
        debug_message("Error: Could not create log file", LOG);
    else
        debug_message("Log file created successfully", LOG);
}

void cleanup_logs() {
    debug_message("Cleaning up logs", TOP_LEVEL);
    if (logs.tokens != NULL) {
        fclose(logs.tokens);
        debug_message("Log file closed successfully", LOG);
    }

    if (logs.ast != NULL) {
        fclose(logs.ast);
        debug_message("Log file closed successfully", LOG);
    }
}