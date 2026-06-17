cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

set(EXEC_NAME "JAKARTA")
set(EXEC_NAME_TEST "JAKARTA_TEST")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE JAKARTA_SOURCES CONFIGURE_DEPENDS "src/*.c" "src/**/*.c")
file(GLOB_RECURSE JAKARTA_TEST_SOURCES CONFIGURE_DEPENDS "src/*.c" "src/**/*.c" "test/*.c")

list(FILTER JAKARTA_TEST_SOURCES EXCLUDE REGEX ".*src/main.c$")

include_directories(include/)

add_library(common_flags INTERFACE)
target_compile_options(common_flags INTERFACE
    -Wall -Wextra
    $<$<CONFIG:Debug>:-fdiagnostics-color=always>
)

add_executable(${EXEC_NAME} ${JAKARTA_SOURCES})
target_include_directories(${EXEC_NAME} PRIVATE include/)

add_executable(${EXEC_NAME_TEST} ${JAKARTA_TEST_SOURCES})
target_include_directories(${EXEC_NAME_TEST} PRIVATE include/)

if(UNIX)
    target_link_libraries(${EXEC_NAME} common_flags)
    target_link_libraries(${EXEC_NAME_TEST} common_flags)
else()
    target_link_libraries(${EXEC_NAME} regex Dbghelp common_flags)
    target_link_libraries(${EXEC_NAME_TEST} regex Dbghelp common_flags)
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

add_custom_command(TARGET ${EXEC_NAME_TEST} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/test
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test
    COMMENT "Copying test directory to bin/"
)