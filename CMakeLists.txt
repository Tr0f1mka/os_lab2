cmake_minimum_required(VERSION 3.10)

project(lab2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(parent src/parent/main.c)
add_executable(child src/child/main.c)

set_target_properties(parent child PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_dependencies(parent child)