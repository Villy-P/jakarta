cmake_minimum_required(VERSION 3.10)
project(Jakarta)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

include_directories(include/)

add_executable(JAKARTA ${SOURCES})

set_target_properties(JAKARTA PROPERTIES 
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(JAKARTA PRIVATE 
    "-Wall" 
    "-fdiagnostics-color=always"
    "-g"
)

target_link_libraries(JAKARTA "regex")