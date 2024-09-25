#ifndef __JAKARTA_DEBUG_H__
#define __JAKARTA_DEBUG_H__

#define PRODUCTION 1

#define TOP_LEVEL 34
#define CREATION 32
#define REMOVAL 31
#define LOG 36

void debug_message(const char* message, unsigned char type);

#endif