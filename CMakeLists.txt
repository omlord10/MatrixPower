cmake_minimum_required(VERSION 4.0)
project(lab2 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab2
        src/main.c
        src/matrix.c
        src/string_utils.c
        src/common.c
        src/tests.c
        include/string_utils.h
        include/tests.h
        include/matrix.h
        include/common.h)
