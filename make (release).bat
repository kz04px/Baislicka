@echo off
gcc src/main.c src/bitboards.c src/display.c src/perft.c src/move.c src/movegen.c src/attack.c src/search.c src/eval.c src/hashtable.c src/uci.c src/board.c src/test.c -DNDEBUG -O3 -static -pthread -o "bin/baislicka.exe"
PAUSE
