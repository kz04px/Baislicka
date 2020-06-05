#ifndef CHESS_BITBOARDS_H
#define CHESS_BITBOARDS_H

#include "board.h"

void bitboards_init();
uint64_t magic_moves_pawns(const int side, const int sq);
uint64_t magic_moves_knight(int sq);
uint64_t magic_moves_bishop(uint64_t occ, int sq);
uint64_t magic_moves_rook(uint64_t occ, int sq);
uint64_t magic_moves_queen(uint64_t occ, int sq);
uint64_t magic_moves_king(int sq);
int u64_file(uint64_t pos);
int u64_rank(uint64_t pos);
uint64_t pinned_pieces_white(const s_board *board, int sq);
uint64_t pinned_pieces_black(const s_board *board, int sq);
int error_check(const s_board *board);
uint64_t get_file(int file);
uint64_t get_adj_files(int file);
uint64_t is_passed_pawn(int side, int sq, uint64_t blockers);
uint64_t is_outpost(int side, int sq, uint64_t enemy_pawns);
uint64_t is_backward_pawn_white(int sq, uint64_t friendly, uint64_t enemy);
uint64_t is_backward_pawn_black(int sq, uint64_t friendly, uint64_t enemy);

#endif
