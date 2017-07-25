#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include "board.h"

int evaluate(s_board *board);
int piece_value(int piece);
int is_endgame(s_board *board);
int is_fifty_move_draw(s_board *board);
int is_threefold(s_board *board);
int pst_value(int piece, int sq, int endgame);
int king_safety(s_board *board, int side);
int piece_mobility(s_board *board, int side);

#endif
