cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

set(PROJECT_NAME "JAKARTA")
set(PROJECT_NAME_TEST "JAKARTA_TEST")

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
    target_link_libraries(${PROJECT_NAME} regex Dbghelp)
    target_link_libraries(${PROJECT_NAME_TEST} regex Dbghelp)
endif()

message("Linking against DbgHelp")
list(APPEND APP_LIBS DbgHelp)

if (WIN32)
    message("Found cv2pdb64.exe, will generate PDB files")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND $<TARGET_FILE_DIR:${PROJECT_NAME}>/cv2pdb64.exe $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Generating PDB using cv2pdb64.exe"
        VERBATIM
    )
endif()