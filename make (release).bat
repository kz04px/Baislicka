@echo off
gcc src/main.c src/bitboards.c src/fen.c src/display.c src/perft.c src/move.c src/movegen.c src/attack.c src/search.c src/eval.c src/hashtable.c src/uci.c src/test.c -DNDEBUG -O3 -static -pthread -o "bin/Baislicka-2.0.exe"
PAUSE
