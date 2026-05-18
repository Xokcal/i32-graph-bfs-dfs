cmake_minimum_required(VERSION 3.10)
project(Graph)

add_executable(
        main main.c
        graph/graph.c
        graph/graph.h
        graph/typedef.h
)