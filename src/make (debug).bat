@echo off
gcc main.c bitboards.c fen.c display.c perft.c move.c movegen.c movegen_white.c movegen_black.c test.c attack.c -g -gdwarf-2 -Wall -Wextra -o "Baislicka 2.0 (debug).exe"
PAUSE