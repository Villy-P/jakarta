cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

set(EXEC_NAME "JAKARTA")
set(EXEC_NAME_TEST "JAKARTA_TEST")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE JAKARTA_SOURCES CONFIGURE_DEPENDS 
    ${CMAKE_SOURCE_DIR}/src/*.c
)
file(GLOB_RECURSE JAKARTA_TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/test/*.c
)
file(GLOB_RECURSE JAKARTA_FORMAT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/include/*.h
)

list(FILTER JAKARTA_TEST_SOURCES EXCLUDE REGEX ".*src/main.c$")

include_directories(include/)

find_program(PRE_COMMIT_EXE NAMES pre-commit)
if(NOT PRE_COMMIT_EXE)
    message(WARNING "pre-commit not found — run: pip install pre-commit --break-system-packages")
endif()

include(FetchContent)
fetchcontent_declare(cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG        v0.7.5
)
fetchcontent_makeavailable(cpptrace)

fetchcontent_declare(libds-c
    GIT_REPOSITORY https://github.com/Villy-P/libds-c.git
    GIT_TAG        main
)
fetchcontent_makeavailable(libds-c)

add_definitions(-DDS_DEFINE_STACK_IMPLEMENTATION)

add_library(jakarta_common_flags INTERFACE)

target_compile_options(jakarta_common_flags INTERFACE
    -Wall 
    -Wextra 
    -g 
    -O0
    $<$<PLATFORM_ID:Windows>:-gcodeview>
    $<$<CONFIG:Debug>:-fdiagnostics-color=always>
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PCRE2 REQUIRED libpcre2-8)

target_link_options(jakarta_common_flags INTERFACE
    -g
    $<$<PLATFORM_ID:Windows>:-mconsole>
    $<$<PLATFORM_ID:Windows>:-Wl,--export-all-symbols>
    $<$<PLATFORM_ID:Windows>:-Wl,--enable-auto-image-base>
)

add_executable(${EXEC_NAME} ${JAKARTA_SOURCES})
target_include_directories(${EXEC_NAME} PRIVATE include/ ${PCRE2_INCLUDE_DIRS} libds-c)

target_link_libraries(${EXEC_NAME} PRIVATE libds-c::libds-c cpptrace::cpptrace jakarta_common_flags)

add_executable(${EXEC_NAME_TEST} ${JAKARTA_TEST_SOURCES})
target_include_directories(${EXEC_NAME_TEST} PRIVATE include/ ${PCRE2_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME_TEST} PRIVATE libds-c::libds-c cpptrace::cpptrace jakarta_common_flags)

target_link_libraries(${EXEC_NAME} PRIVATE ${PCRE2_LINK_LIBRARIES})
target_link_libraries(${EXEC_NAME_TEST} PRIVATE ${PCRE2_LINK_LIBRARIES})

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
find_program(CPPCHECK_EXE NAMES cppcheck)
find_program(RUN_CLANG_TIDY_EXE NAMES run-clang-tidy run-clang-tidy.py)
find_package(Python3 COMPONENTS Interpreter)

if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CLANG_TIDY_COMMAND
        ${CLANG_TIDY_EXE}
        -quiet
        --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        -p ${CMAKE_BINARY_DIR}
    )
    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY_COMMAND} ${JAKARTA_SOURCES}
        COMMENT "Running clang-tidy..."
        USES_TERMINAL
        VERBATIM
    )

    set_target_properties(${EXEC_NAME} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    set_target_properties(${EXEC_NAME_TEST} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
else()
    message(STATUS "clang-tidy not found")
endif()

if(CPPCHECK_EXE)
    message(STATUS "cppcheck found: ${CPPCHECK_EXE}")
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXE}
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            --file-filter=${CMAKE_SOURCE_DIR}/src/*
            --file-filter=${CMAKE_SOURCE_DIR}/include/*
            --enable=warning,style,performance,portability
            --suppress=missingIncludeSystem
            --inline-suppr
            --error-exitcode=1
            --quiet
            -j8
        COMMENT "Running cppcheck..."
        USES_TERMINAL
        VERBATIM
    )
else()
    message(STATUS "cppcheck not found")
endif()

if(RUN_CLANG_TIDY_EXE AND Python3_Interpreter_FOUND)
    message(STATUS "run-clang-tidy found: ${RUN_CLANG_TIDY_EXE}")
    add_custom_target(run-clang-tidy
        COMMAND Python3::Interpreter ${RUN_CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} -quiet
            "^(?!.*build).*(include|src|test)(/|\\\\).*\\.(cpp|h|c|hpp)$"
        COMMENT "Running run-clang-tidy..."
        USES_TERMINAL
        VERBATIM
    )
else()
    message(STATUS "run-clang-tidy/Python not found")
endif()
