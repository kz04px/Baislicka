#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <inttypes.h>
#include "board.h"

#define NO_MOVE ((s_move) {0})

typedef uint32_t s_move;

enum {QUIET, DOUBLE_PAWN, KSC, QSC, CAPTURE, EP, FILLER1, FILLER2, KNIGHT_PROMO, BISHOP_PROMO, ROOK_PROMO, QUEEN_PROMO, KNIGHT_PROMO_CAPTURE, BISHOP_PROMO_CAPTURE, ROOK_PROMO_CAPTURE, QUEEN_PROMO_CAPTURE};

void null_make(s_board *board);
void null_undo(s_board *board);
s_move move_add(const s_board *board, int from, int to, int type, int piece_type);
int move_add_pawn(const s_board *board, s_move *move_list, int from, int to);
s_move add_promotion_move(const s_board *board, int from, int to, int type);
void move_make(s_board *board, const s_move *move);
void move_undo(s_board *board, const s_move *move);
int moves_sort_see(s_board *board, s_move *moves, int num_moves, const int *values);
int move_make_ascii(s_board *board, const char *move_string);
int move_to_string(char *string, const s_move *move);

int is_legal_move(const s_board *board, const s_move *move);
int is_promo_move(const s_move move);
int is_capture_move(const s_move move);

int move_get_to(s_move move);
int move_get_from(s_move move);
int move_get_type(s_move move);
int move_get_captured(s_move move);
int move_get_piece(s_move move);

void move_set_to(s_move *move, uint8_t to);
void move_set_from(s_move *move, uint8_t from);
void move_set_type(s_move *move, uint8_t type);
void move_set_captured(s_move *move, uint8_t piece);
void move_set_piece(s_move *move, uint8_t piece);

int see(int sq, int side, int captured, uint64_t colours[2], uint64_t pieces[6], const int *values);
int see_capture(s_board *board, const s_move move, const int *values);
int see_quiet(s_board *board, const s_move move, const int *values);

int is_same_move(const s_move move_1, const s_move move_2);

#endif
