cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

set(EXEC_NAME "JAKARTA")
set(EXEC_NAME_TEST "JAKARTA_TEST")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE JAKARTA_SOURCES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp")
file(GLOB_RECURSE JAKARTA_TEST_SOURCES CONFIGURE_DEPENDS "src/*.c" "src/**/*.c" "test/*.c" "src/*.cpp")

list(FILTER JAKARTA_TEST_SOURCES EXCLUDE REGEX ".*src/main.c$")

include_directories(include/)

include(FetchContent)
fetchcontent_declare(cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG        v0.7.5
)
fetchcontent_makeavailable(cpptrace)

add_library(common_flags INTERFACE)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
find_program(CPPCHECK_EXE NAMES cppcheck)

if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CLANG_TIDY_COMMAND
        ${CLANG_TIDY_EXE}
        --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        -p ${CMAKE_BINARY_DIR}
    )
else()
    message(STATUS "clang-tidy not found")
endif()

if(CPPCHECK_EXE)
    message(STATUS "cppcheck found: ${CPPCHECK_EXE}")
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXE}
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            --enable=warning,style,performance,portability
            --suppress=missingIncludeSystem
            --inline-suppr
            --error-exitcode=1
            --quiet
        COMMENT "Running cppcheck..."
        VERBATIM
    )
else()
    message(STATUS "cppcheck not found")
endif()

target_compile_options(common_flags INTERFACE
    -Wall 
    -Wextra 
    -g 
    -O0
    -gcodeview
    $<$<CONFIG:Debug>:-fdiagnostics-color=always>
)

set(PCRE2_INCLUDE_DIRS "C:/msys64/mingw64/include")
set(PCRE2_LIBRARIES "C:/msys64/mingw64/lib/libpcre2-8.dll.a")

target_compile_definitions(common_flags INTERFACE _WIN32)
target_link_options(common_flags INTERFACE
    -g
    -Wl,--export-all-symbols
    -Wl,--enable-auto-image-base
)

add_executable(${EXEC_NAME} ${JAKARTA_SOURCES})
target_include_directories(${EXEC_NAME} PRIVATE include/ ${PCRE2_INCLUDE_DIRS})

target_link_libraries(${EXEC_NAME} PRIVATE cpptrace::cpptrace common_flags)

# 5. Define the test target
add_executable(${EXEC_NAME_TEST} ${JAKARTA_TEST_SOURCES})
target_include_directories(${EXEC_NAME_TEST} PRIVATE include/ ${PCRE2_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME_TEST} PRIVATE cpptrace::cpptrace common_flags)

if(UNIX)
    # Unix specific links if needed
else()
    target_link_libraries(${EXEC_NAME} PRIVATE ${PCRE2_LIBRARIES} dbghelp psapi)
    target_link_libraries(${EXEC_NAME_TEST} PRIVATE ${PCRE2_LIBRARIES} dbghelp psapi)
endif()

if(CLANG_TIDY_EXE)
    set_target_properties(${EXEC_NAME} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    set_target_properties(${EXEC_NAME_TEST} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()