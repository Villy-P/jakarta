#include "core.h"

void print_stack_trace_old() {
    #ifdef _WIN32
    HANDLE       process = GetCurrentProcess();
    HANDLE       thread  = GetCurrentThread();
    CONTEXT      context;
    STACKFRAME64 stack;
    DWORD        machine_type;

    RtlCaptureContext(&context);

    ZeroMemory(&stack, sizeof(STACKFRAME64));

    #ifdef _M_IX86
        machine_type           = IMAGE_FILE_MACHINE_I386;
        stack.AddrPC.Offset    = context.Eip;
        stack.AddrFrame.Offset = context.Ebp;
        stack.AddrStack.Offset = context.Esp;
    #elif _M_X64
        machine_type           = IMAGE_FILE_MACHINE_AMD64;
        stack.AddrPC.Offset    = context.Rip;
        stack.AddrFrame.Offset = context.Rsp;
        stack.AddrStack.Offset = context.Rsp;
    #elif _M_ARM64
        machine_type           = IMAGE_FILE_MACHINE_ARM64;
        stack.AddrPC.Offset    = context.Pc;
        stack.AddrFrame.Offset = context.Fp;
        stack.AddrStack.Offset = context.Sp;
    #else
        #error "Unsupported platform"
    #endif

    stack.AddrPC.Mode    = AddrModeFlat;
    stack.AddrFrame.Mode = AddrModeFlat;
    stack.AddrStack.Mode = AddrModeFlat;

    SymInitialize(process, NULL, TRUE);
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

    printf("Stack trace:\n");
    printf("    %-40s %-18s %s\n", "Function", "Address", "Line");
    printf("    %-40s %-18s %s\n", "--------", "-------", "----");

    DWORD frame_number = 0;
    while (StackWalk64(
        machine_type,
        process,
        thread,
        &stack,
        &context,
        NULL,
        SymFunctionTableAccess64,
        SymGetModuleBase64,
        NULL)) {
        if (stack.AddrPC.Offset == 0)
            break;

        DWORD64 symbol_addr  = stack.AddrPC.Offset;
        DWORD64 displacement = 0;
        char symbol_buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)] = {0};
        SYMBOL_INFO *symbol  = (SYMBOL_INFO *)symbol_buffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen   = MAX_SYM_NAME;

        // Get line information
        IMAGEHLP_LINE64 line = {0};
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        DWORD line_displacement = 0;
        BOOL has_line = SymGetLineFromAddr64(process, symbol_addr, &line_displacement, &line);

        char function_name[MAX_SYM_NAME] = "Unknown";
        if (SymFromAddr(process, symbol_addr, &displacement, symbol)) {
            strncpy(function_name, symbol->Name, MAX_SYM_NAME - 1);
            function_name[MAX_SYM_NAME - 1] = '\0'; // Ensure null termination
        }

        // Format line information
        char line_info[256] = "Unknown";
        if (has_line) {
            snprintf(line_info, sizeof(line_info), "%s:%lu", line.FileName, line.LineNumber);
        }

        // Print with better alignment using format specifiers
        printf("    %-40.40s 0x%016llX %s\n",
               function_name,
               symbol_addr,
               line_info);

        frame_number++;
    }

    SymCleanup(process);
    #endif
}