#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdarg.h>
#include <sys/time.h>

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

    logs.ast = fopen("logs/ast.log", "w");
    if (logs.ast != NULL)
        setvbuf(logs.ast, NULL, _IONBF, 0);

    logs.main = fopen("logs/main.log", "w");
    if (logs.main == NULL)
        setvbuf(logs.main, NULL, _IONBF, 0);
}

void cleanup_logs() {
    debug_message("Cleaning up logs", TOP_LEVEL);

    if (logs.ast != NULL)
        fclose(logs.ast);

    if (logs.main != NULL)
        fclose(logs.main);
}

void log_msg(FILE* file, const char* format, ...) {
    va_list args;
    va_start(args, format);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    struct tm time_info;
    #ifdef _WIN32
        time_t raw_time = (time_t)tv.tv_sec;
        localtime_s(&time_info, &raw_time);
    #else
        localtime_r(&tv.tv_sec, &time_info);
    #endif

    char timestamp[30];
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &time_info);
    
    sprintf(timestamp, "%s.%03d", time_str, (int)(tv.tv_usec / 1000));

    fprintf(file, "[%s] ", timestamp);
    vfprintf(file, format, args);
    fprintf(file, "\n");
    
    va_end(args);
}