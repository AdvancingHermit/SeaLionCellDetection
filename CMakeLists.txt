cmake_minimum_required(VERSION 3.29)
project(SeaLionCellDetection C)

set(CMAKE_C_STANDARD 11)

add_executable(SeaLionCellDetection main.c
        cbmp.c
        cbmp.h
        dc.c
        dc.h)
