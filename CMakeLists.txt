cmake_minimum_required(VERSION 3.16)

project(projet_mh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(projet_mh
    src/main.cpp
    src/graph.cpp
    src/parser.cpp
    src/coloration.cpp
    src/solver.cpp
    src/clock.h
    src/tabou.cpp
)