#include "core.h"
#include <ctrace/ctrace.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_ERROR_CODE 1
#define MAX_CTRACE_DEPTH 32

void handle_seg_fault(int sig, siginfo_t* info, void* ucontext) {
    (void)sig;
    (void)ucontext;

    printf(
        "\033[31mSegmentation Fault (Access Violation) detected at address "
        "%p\033[0m\n",
        info->si_addr);
    fflush(stdout);

    ctrace_stacktrace trace = ctrace_generate_trace(0, MAX_CTRACE_DEPTH);
    ctrace_print_stacktrace(&trace, stdout, 1);
    ctrace_free_stacktrace(&trace);

    _Exit(DEFAULT_ERROR_CODE);
}

void install_seg_fault_handler(void) {
    struct sigaction siga;
    memset(&siga, 0, sizeof(siga));
    siga.sa_flags = SA_SIGINFO;
    siga.sa_sigaction = handle_seg_fault;
    sigemptyset(&siga.sa_mask);

    if (sigaction(SIGSEGV, &siga, nullptr) == -1) {
        perror("sigaction");
        _Exit(EXIT_FAILURE);
    }
}

