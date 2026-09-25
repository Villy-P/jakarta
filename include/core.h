#pragma once

#include <bits/types/siginfo_t.h>

// segfault-handler.c
void handle_seg_fault(int sig, siginfo_t* info, void* ucontext);
void install_seg_fault_handler(void);
