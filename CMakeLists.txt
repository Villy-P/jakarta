cmake_minimum_required(VERSION 3.10)
project(Jakarta VERSION 1.0)

if(UNIX)
    set(PROJECT_NAME "JAKARTA.exe")
else()
    set(PROJECT_NAME "JAKARTA")
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c")

include_directories(include/)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall"
    "-Wextra"
    "-gdwarf-2"
    "-fdiagnostics-color=always"
    "-g"
)

if(UNIX)
    target_link_libraries(${PROJECT_NAME})
else()
    target_link_libraries(${PROJECT_NAME} "regex")
endif()