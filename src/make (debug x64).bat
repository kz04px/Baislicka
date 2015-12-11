@echo off
gcc main.c bitboards.c fen.c display.c perft.c move.c movegen.c -g -Wall -Wextra -pthread -o "Baislicka 2.0 (debug).exe"
PAUSE