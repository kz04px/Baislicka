@echo off
gcc main.c bitboards.c fen.c display.c perft.c move.c movegen.c movegen_white.c movegen_black.c test.c attack.c search.c eval.c -O3 -o "Baislicka 2.0.exe"
PAUSE