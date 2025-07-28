cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(UNIX)
    set(PROJECT_NAME "JAKARTA.exe")
    set(PROJECT_NAME_TEST "JAKARTA_TEST.exe")
else()
    set(PROJECT_NAME "JAKARTA")
    set(PROJECT_NAME_TEST "JAKARTA_TEST")
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c")
file(GLOB SOURCES_TEST
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c")
list(FILTER SOURCES_TEST EXCLUDE REGEX ".*src/main.c$")

include_directories(include/)

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(${PROJECT_NAME_TEST} ${SOURCES_TEST})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(${PROJECT_NAME_TEST} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:
        $<$<C_COMPILER_ID:MSVC>:/Zi /Od>
        $<$<NOT:$<C_COMPILER_ID:MSVC>>:-g -Wall -Wextra -fdiagnostics-color=always>
    >
)
target_compile_options(${PROJECT_NAME_TEST} PRIVATE
    $<$<CONFIG:Debug>:
        $<$<C_COMPILER_ID:MSVC>:/Zi /Od>
        $<$<NOT:$<C_COMPILER_ID:MSVC>>:-g -Wall -Wextra -fdiagnostics-color=always>
    >
)

if(UNIX)
    target_link_libraries(${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME_TEST})
else()
    target_link_libraries(${PROJECT_NAME} "regex" "dbghelp")
    target_link_libraries(${PROJECT_NAME_TEST} "regex" "dbghelp")
endif()