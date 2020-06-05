#ifndef CHESS_ZOBRIST_H
#define CHESS_ZOBRIST_H

#include "board.h"

uint64_t key_piece_positions[6][2][64];
uint64_t key_turn;
uint64_t key_castling[16];
uint64_t key_ep_file[8];
uint64_t key_ksc[2];
uint64_t key_qsc[2];

void key_init();
uint64_t create_key_board(const s_board *board);

#endif
