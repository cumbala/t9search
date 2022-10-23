cmake_minimum_required(VERSION 3.23)
project(t9search C)

set(CMAKE_C_STANDARD 99)

add_executable(t9search t9search.c)
add_executable(t9search_recursive t9search_recursive.c)
add_executable(t9search_levenshtein t9search_levenshtein.c)
