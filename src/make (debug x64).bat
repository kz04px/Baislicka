@echo off
gcc bajschess.c attacking.c display.c board.c io.c evaluate.c move.c movegen.c uci.c search.c positions.c perft.c -g -Wall -Wextra -pthread -o "Baislicka 2.0 (debug).exe"
PAUSE