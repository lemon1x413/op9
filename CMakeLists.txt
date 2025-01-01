cmake_minimum_required(VERSION 3.29)
project(op9 C)

set(CMAKE_C_STANDARD 99)

add_executable(op9 main.c
        color.h
        functions.h
        io.h
        sort.h)
