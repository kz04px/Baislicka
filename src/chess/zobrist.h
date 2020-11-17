#ifndef CHESS_ZOBRIST_H
#define CHESS_ZOBRIST_H

#include "board.h"

extern uint64_t key_piece_positions[6][2][64];
extern uint64_t key_turn;
extern uint64_t key_castling[16];
extern uint64_t key_ep_file[8];
extern uint64_t key_ksc[2];
extern uint64_t key_qsc[2];

void key_init();
uint64_t create_key_board(const s_board *board);

#endif
