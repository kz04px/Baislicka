#ifndef EVAL_H
#define EVAL_H

#include "../chess/board.h"

extern const int piece_values[6];

int evaluate(const s_board *board);
int piece_value(int piece);
int pst_value(int piece, int sq, int endgame);
int king_safety(const s_board *board, int side);
int piece_mobility(const s_board *board, int side);

#endif
