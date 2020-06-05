#ifndef CHESS_MOVEGEN_H
#define CHESS_MOVEGEN_H

#include "move.h"
#include "board.h"

enum {GEN_HASHMOVE, GEN_CAPTURES, GEN_QUIETS};

typedef struct
{
    int stage;
    int move_num;
    int num_moves;
    int capture_piece;
    s_move hash_move;
    s_move killer_move;
#ifdef KILLER_MOVES_2
    s_move killer_move_2;
#endif
    s_move moves[MAX_MOVES];
    int scores[MAX_MOVES];
} s_move_generator;

int next_move(s_board *board, s_move_generator *generator, s_move *move, const int *values);

int find_moves_pawn_ep(const s_board *board, s_move *move_list);
int find_moves_pawn_captures(const s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_wP_quiet(const s_board *board, s_move *move_list);
int find_moves_bP_quiet(const s_board *board, s_move *move_list);
int find_moves_knights(const s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_bishops(const s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_rooks(const s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_queens(const s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_bishops_queens(const s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_rooks_queens(const s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_kings(const s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_kings_castles(const s_board *board, s_move *move_list);
int find_moves_all(const s_board *board, s_move *move_list, int colour);
int find_moves_captures(const s_board *board, s_move *move_list, int colour);
int find_moves_quiet(const s_board *board, s_move *move_list, int colour);
int can_castle(const s_board *board, int turn, int side);

#endif
