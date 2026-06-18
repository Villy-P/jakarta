cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

set(EXEC_NAME "JAKARTA")
set(EXEC_NAME_TEST "JAKARTA_TEST")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE JAKARTA_SOURCES CONFIGURE_DEPENDS "src/*.c" "src/**/*.c" "src/*.cpp")
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
target_compile_options(common_flags INTERFACE
    -Wall 
    -Wextra 
    -g 
    -gcodeview
    $<$<CONFIG:Debug>:-fdiagnostics-color=always>
)

target_compile_definitions(common_flags INTERFACE _WIN32)
target_link_options(common_flags INTERFACE
    -g
    -Wl,--export-all-symbols
    -Wl,--enable-auto-image-base
)

add_executable(${EXEC_NAME} ${JAKARTA_SOURCES})
target_include_directories(${EXEC_NAME} PRIVATE include/)

target_link_libraries(${EXEC_NAME} PRIVATE cpptrace::cpptrace common_flags)

# 5. Define the test target
add_executable(${EXEC_NAME_TEST} ${JAKARTA_TEST_SOURCES})
target_include_directories(${EXEC_NAME_TEST} PRIVATE include/)
target_link_libraries(${EXEC_NAME_TEST} PRIVATE cpptrace::cpptrace common_flags)

if(UNIX)
    # Unix specific links if needed
else()
    target_link_libraries(${EXEC_NAME} PRIVATE regex dbghelp psapi)
    target_link_libraries(${EXEC_NAME_TEST} PRIVATE regex dbghelp psapi)
endif()