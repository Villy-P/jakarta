#include <stdlib.h>
#include <stdio.h>

#include "error.h"

#define DEFAULT_ERROR_CODE 1

void jakarta_error(int error_code, Token* token, const char* additional_info) {
    printf("\033[31m");
    if (token != NULL)
        printf("Error at position %d:%d: ", token->line, token->col);
    else
        printf("Error: ");
    printf("ERR_CODE_%d\n", error_code);
    switch (error_code) {
        // additional_info: the argument that was missing a file
        case ERR_INVALID_FILE_LOCATION:
            printf("No file argument found for %s.\n", additional_info);
            printf("Enter a file name or location after %s in your compiler args.", additional_info);
            break;
        // additional_info: the file name that was not found
        case ERR_INVALID_FILE_NAME:
            printf("File %s does not exist.\n", additional_info);
            printf("Enter a correct file name after -f.\n");
            break;
        case ERR_UNKNOWN_SYMBOL:
            printf("Unknown symbol encountered: %s, symbol code %s.\n", token->content, get_string_from_symbol(token->symbol));
            break;
        // additional_info: the file name that could not be closed
        case ERR_CANNOT_CLOSE_FILE:
            printf("Could not close file %s.\n", additional_info);
            break;
        case ERR_UNDEFINED_IDENTIFIER:
            printf("Identifier %s does not exist in current scope.\n", token->content);
            break;
        // additional_info: the token that was duplciate
        case ERR_DUPLICATE_IDENTIFIER:
            printf("Identifier %s already exists in current scope.\n", additional_info);
            break;
        // additional_info: the custom error message
        case ERR_CUSTOM:
            printf("%s\n", additional_info);
            break;
        // additional_info: what was trying to be allocated
        case ERR_MALLOC_FAIL:
            printf("Failed to allocate memory for %s\n", additional_info);
            break;
        default:
            if (token != NULL)
                printf("Error at position %d:%d: %s\n", token->line, token->col, token->content);
            else
                printf("An error occurred.\n");
    }
    print_stack_trace();
    printf("\033[0m\n");
    exit(error_code);
}

// Code 5
void jakarta_error_invalid_token(const char* expected, const char* got) {
    printf("\033[31mThere was an error while running your code: ERR_CODE_5\n");
    printf("Invalid Token Error: Expected token %s but got %s\033[0m\n", expected, got);
    exit(DEFAULT_ERROR_CODE);
}

// Code 6
void jakarta_error_undefined_identifier(Token* identifier) {
    printf("\033[31mThere was an error while running your code at position %d:%d: ERR_CODE_6\n", identifier->line, identifier->col);
    printf("Undefined Identifier: Could not find identifier %s\033[0m\n", identifier->content);
    exit(DEFAULT_ERROR_CODE);
}

// Code 7
void jakarta_error_invalid_typedef_location(Token* token) {
    printf("\033[31mThere was an error while running your code at position %d:%d: ERR_CODE_7\n", token->line, token->col);
    printf("Typedef statement cannot be used outside of global context\033[0m\n");
    exit(DEFAULT_ERROR_CODE);
}

void print_stack_trace() {
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