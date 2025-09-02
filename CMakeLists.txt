cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

set(EXEC_NAME "JAKARTA")
set(EXEC_NAME_TEST "JAKARTA_TEST")

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

add_executable(${EXEC_NAME} ${SOURCES})
add_executable(${EXEC_NAME_TEST} ${SOURCES_TEST})

set_target_properties(${EXEC_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(${EXEC_NAME_TEST} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(${EXEC_NAME} PRIVATE
    $<$<CONFIG:Debug>:
        $<$<C_COMPILER_ID:MSVC>:/Zi /Od>
        $<$<NOT:$<C_COMPILER_ID:MSVC>>:-g -Wall -Wextra -fdiagnostics-color=always>
    >
)
target_compile_options(${EXEC_NAME_TEST} PRIVATE
    $<$<CONFIG:Debug>:
        $<$<C_COMPILER_ID:MSVC>:/Zi /Od>
        $<$<NOT:$<C_COMPILER_ID:MSVC>>:-g -Wall -Wextra -fdiagnostics-color=always>
    >
)

if(UNIX)
    target_link_libraries(${EXEC_NAME})
    target_link_libraries(${EXEC_NAME_TEST})
else()
    target_link_libraries(${EXEC_NAME} regex Dbghelp)
    target_link_libraries(${EXEC_NAME_TEST} regex Dbghelp)
endif()

message("Linking against DbgHelp")
list(APPEND APP_LIBS DbgHelp)

if (WIN32)
    find_program(CV2PDB_EXECUTABLE cv2pdb64.exe PATHS ${CMAKE_SOURCE_DIR})
    if (CV2PDB_EXECUTABLE)
        message("Found cv2pdb64.exe at ${CV2PDB_EXECUTABLE}, will generate PDB files")
        add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
            COMMAND ${CV2PDB_EXECUTABLE} $<TARGET_FILE:${EXEC_NAME}>
            COMMENT "Generating PDB using cv2pdb64.exe"
            VERBATIM
        )
    else()
        message("cv2pdb64.exe not found, skipping PDB generation")
    endif()
endif()