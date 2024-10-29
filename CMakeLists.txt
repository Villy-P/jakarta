cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c")
file(GLOB SOURCES_TEST
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c")

include_directories(include/)

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(${PROJECT_NAME_TEST} ${SOURCES_TEST})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(${PROJECT_NAME_TEST} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall" "-Wextra" "-gdwarf-2" "-fdiagnostics-color=always" "-g")
target_compile_options(${PROJECT_NAME_TEST} PRIVATE
    "-Wall" "-Wextra" "-gdwarf-2" "-fdiagnostics-color=always" "-g")

if(UNIX)
    target_link_libraries(${PROJECT_NAME})
else()
    target_link_libraries(${PROJECT_NAME} "regex")
endif()