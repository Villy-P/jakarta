find_program(RUN_CLANG_TIDY_EXE NAMES run-clang-tidy run-clang-tidy.py)
find_package(Python3 COMPONENTS Interpreter)

if(RUN_CLANG_TIDY_EXE AND Python3_Interpreter_FOUND)
    message(STATUS "run-clang-tidy found: ${RUN_CLANG_TIDY_EXE}")
    add_custom_target(jakarta-clang-tidy
        COMMAND Python3::Interpreter ${RUN_CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} -quiet -fix -use-color 1
            "^(?!.*build).*(include|src|test)(/|\\\\).*\\.(cpp|h|c|hpp)$"
        COMMENT "Running jakarta-clang-tidy..."
        USES_TERMINAL
        VERBATIM
    )

    add_custom_target(jakarta-clang-tidy-check
        COMMAND Python3::Interpreter ${RUN_CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} -quiet -use-color 1
            "^(?!.*build).*(include|src|test)(/|\\\\).*\\.(cpp|h|c|hpp)$"
        COMMENT "Running jakarta-clang-tidy-check..."
        USES_TERMINAL
        VERBATIM
    )
else()
    message(STATUS "run-clang-tidy/Python not found")
endif()