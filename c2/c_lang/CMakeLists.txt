cmake_minimum_required(VERSION 3.24)
project(c_lang C)

set(CMAKE_C_STANDARD 11)

add_library(matrix_lib STATIC
        src/static/matrix.c
        include/static/thread_matrix_multiplier.h src/static/thread_matrix_multiplier.c)

target_include_directories(matrix_lib
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        )

include_directories(. ./include/static)

add_compile_options(-Wall -Wextra -Wpedantic -lm)

add_executable(c_lang src/app.c)

target_link_libraries( c_lang
        PRIVATE
        matrix_lib
        )