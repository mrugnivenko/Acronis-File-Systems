cmake_minimum_required(VERSION 3.15)
project(UTF_converter)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(UTF_converter
        converter.cpp
        converter.hpp
        main.cpp)
