@echo off
gcc main.c bitboards.c fen.c display.c perft.c move.c movegen.c movegen_white.c movegen_black.c attack.c search.c eval.c -DNDEBUG -O3 -o "Baislicka 2.0 (release).exe"
PAUSE