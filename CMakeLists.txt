cmake_minimum_required(VERSION 3.10)

# Project
project(Baislicka VERSION 1.0 LANGUAGES C)

# Packages
find_package(Threads REQUIRED)

# Base directory relative includes
include_directories(.)
include_directories(./src/)

# Flags
set(CMAKE_C_FLAGS "-Wall -Wextra -Wshadow")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3 -flto -march=native -DNDEBUG")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add engine executable
add_executable(
    baislicka
    src/main.c
    src/uci.c

    src/chess/attack.c
    src/chess/bitboards.c
    src/chess/board.c
    src/chess/display.c
    src/chess/move.c
    src/chess/movegen.c
    src/chess/perft.c
    src/chess/zobrist.c

    src/search/hashtable.c
    src/search/pv.c
    src/search/qsearch.c
    src/search/search-root.c
    src/search/search.c

    src/eval/eval.c
    src/eval/king_safety.c
    src/eval/mobility.c
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
    tests/perft.c

    src/chess/attack.c
    src/chess/bitboards.c
    src/chess/board.c
    src/chess/move.c
    src/chess/movegen.c
    src/chess/perft.c
    src/chess/zobrist.c

    src/search/hashtable.c

    src/eval/eval.c
    src/eval/king_safety.c
    src/eval/mobility.c
)

target_link_libraries(baislicka Threads::Threads)

set_property(TARGET baislicka PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
