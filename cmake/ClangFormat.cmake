find_program(CLANG_FORMAT_EXE NAMES clang-format clang-format-18 clang-format-17)

if(CLANG_FORMAT_EXE)
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${JAKARTA_FORMAT_SOURCES}
        COMMENT "Running clang-format..."
        USES_TERMINAL
        VERBATIM
    )

    add_custom_target(clang-format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror -style=file ${JAKARTA_FORMAT_SOURCES}
        COMMENT "Running clang-format check..."
        USES_TERMINAL
        VERBATIM
    )
else()
    message(STATUS "clang-format not found")
endif()