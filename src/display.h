#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "move.h"
#include "board.h"

void print_move(s_move move);
void print_moves(s_board *board);
void print_u64(uint64_t board);
void display_board(s_board *board);
void display_history(s_board *board);

#endif
