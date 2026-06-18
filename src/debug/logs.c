#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdarg.h>

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
    else {
        setvbuf(logs.tokens, NULL, _IONBF, 0);
        debug_message("Log file created successfully", LOG);
    }

    logs.ast = fopen("logs/ast.log", "w");
    if (logs.ast == NULL)        
        debug_message("Error: Could not create log file", LOG);
    else {
        setvbuf(logs.ast, NULL, _IONBF, 0);
        debug_message("Log file created successfully", LOG);
    }

    logs.main = fopen("logs/main.log", "w");
    if (logs.main == NULL)
        debug_message("Error: Could not create log file", LOG);
    else {
        setvbuf(logs.main, NULL, _IONBF, 0);
        debug_message("Log file created successfully", LOG);
    }
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

    if (logs.main != NULL) {
        fclose(logs.main);
        debug_message("Log file closed successfully", LOG);
    }
}

void log_msg(FILE* file, const char* format, ...) {
    va_list args;
    va_start(args, format);

    time_t raw_time;
    struct tm *time_info;
    char timestamp[20];

    time(&raw_time);
    time_info = localtime(&raw_time);

    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
    fprintf(file, "[%s] ", timestamp);
    vfprintf(file, format, args);
    va_end(args);
}