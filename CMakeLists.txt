cmake_minimum_required(VERSION 3.10)
project(mtm_ex3_map C)

set(CMAKE_C_STANDARD 99)

add_executable(mtm_ex3_map main.c map.c map.h node.h node.c)