#pragma once

#define PRODUCTION 1

#define TOP_LEVEL 34
#define CREATION 32
#define REMOVAL 31
#define LOG 36

#include <stdio.h>

typedef struct {
    FILE* tokens;
    FILE* ast;
    FILE* main;
} LogFiles;

extern LogFiles logs;

void debug_message(const char* message, unsigned char type);
void setup_logs();
void cleanup_logs();
void log(FILE* file, const char* format, ...);