#ifndef TESTS_H
#define TESTS_H

#include <chess/board.h>

int test_move_legality();
int test_make_undo();
int test_eval_mirroring();
int test_passed_pawns();
int test_backward_pawns();
int board_flip(s_board *board);
int test_perft();

#endif
