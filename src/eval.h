#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include "board.h"

int evaluate(const s_board *board);
int piece_value(int piece);
int is_endgame(const s_board *board);
int is_fifty_move_draw(const s_board *board);
int is_threefold(const s_board *board);
int pst_value(int piece, int sq, int endgame);
int king_safety(const s_board *board, int side);
int piece_mobility(const s_board *board, int side);

#endif
