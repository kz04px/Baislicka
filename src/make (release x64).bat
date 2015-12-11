@echo off
gcc main.c bitboards.c fen.c display.c perft.c move.c movegen.c -O3 -pthread -o "Baislicka 2.0.exe"
PAUSE