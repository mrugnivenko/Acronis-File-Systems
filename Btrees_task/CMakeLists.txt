cmake_minimum_required(VERSION 3.15)
project(Btrees_task C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Btrees_task
        btree.c
        btree.h
        main.c)
