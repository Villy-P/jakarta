#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include "debug.h"

LogFiles logs;

#define LOG_TIMESTAMP_BUFFER 30
#define LOG_TIME_STRING_BUFFER 20

void setup_logs() {
    debug_message("Setting up logs", TOP_LEVEL);

    struct stat stats = {0};
    if (stat("logs", &stats) == -1) {
        if (mkdir("logs") != 0) {
            debug_message("Error: Failed to create 'logs' directory", LOG);
            return;
        }
    }

    logs.ast = fopen("logs/ast.log", "w");
    if (logs.ast != nullptr) {
        if (setvbuf(logs.ast, nullptr, _IONBF, 0) != 0) {
            debug_message("Error: Failed to set buffer for ast log file", LOG);
        }
    }

    logs.main = fopen("logs/main.log", "w");
    if (logs.main != nullptr) {
        if (setvbuf(logs.main, nullptr, _IONBF, 0) != 0) {
            debug_message("Error: Failed to set buffer for main log file", LOG);
        }
    }
}

void cleanup_logs() {
    debug_message("Cleaning up logs", TOP_LEVEL);

    if (logs.ast != nullptr) {
        if (fclose(logs.ast) != 0) {
            debug_message("Error: Failed to close ast log file", LOG);
        }
    }

    if (logs.main != nullptr) {
        if (fclose(logs.main) != 0) {
            debug_message("Error: Failed to close main log file", LOG);
        }
    }
}

void log_msg(FILE* file, const char* format, ...) {
    va_list args = nullptr;
    va_start(args, format);

    struct timeval timev;
    gettimeofday(&timev, nullptr);
    
    struct tm time_info;
    #ifdef _WIN32
        time_t raw_time = (time_t)timev.tv_sec;
        if (localtime_s(&time_info, &raw_time) != 0) {
            debug_message("Error: Failed to get local time", LOG);
        }
    #else
        localtime_r(&timev.tv_sec, &time_info);
    #endif

    char timestamp[LOG_TIMESTAMP_BUFFER];
    char time_str[LOG_TIME_STRING_BUFFER];
    if (strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &time_info) == 0) {
        debug_message("Error: Failed to format time", LOG);
    }

    if (fprintf(file, "[%s] ", timestamp) < 0) {
        debug_message("Error: Failed to write timestamp to log file", LOG);
    }
    if (vfprintf(file, format, args) < 0) {
        debug_message("Error: Failed to write log message", LOG);
    }
    if (fprintf(file, "\n") < 0) {
        debug_message("Error: Failed to write newline to log file", LOG);
    }

    va_end(args);
}