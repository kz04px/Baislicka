cmake_minimum_required(VERSION 3.10)

# Project
project(Baislicka VERSION 1.0 LANGUAGES C)

# Base directory relative includes
include_directories(.)
include_directories(./src/)

# Flags
set(CMAKE_C_FLAGS "-pthread")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add engine executable
add_executable(
    baislicka
    src/main.c
    src/attack.c
    src/bitboards.c
    src/board.c
    src/display.c
    src/eval.c
    src/hashtable.c
    src/move.c
    src/movegen.c
    src/perft.c
    src/pv.c
    src/qsearch.c
    src/search-root.c
    src/search.c
    src/uci.c
)

# Add test executable
add_executable(
    baislicka-test
    tests/main.c
    tests/backward_pawns.c
    tests/eval_mirror.c
    tests/is_legal.c
    tests/makeundo.c
    tests/passed_pawns.c

    src/attack.c
    src/bitboards.c
    src/board.c
    src/eval.c
    src/hashtable.c
    src/move.c
    src/movegen.c
)
