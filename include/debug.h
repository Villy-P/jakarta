#pragma once

#include <stdint.h>
static const uint8_t PRODUCTION = 1;

static const uint8_t TOP_LEVEL = 34;
static const uint8_t CREATION = 32;
static const uint8_t REMOVAL = 31;
static const uint8_t LOG = 36;

#include <stdio.h>

typedef struct {
    FILE* ast;
    FILE* main;
} LogFiles;

extern LogFiles logs;  // NOLINT

void debug_message(const char* message, uint8_t type);
void setup_logs();
void cleanup_logs();
void log_msg(FILE* file, const char* format, ...);