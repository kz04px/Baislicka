#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include "move.h"
#include "board.h"

enum {GEN_HASHMOVE, GEN_CAPTURES, GEN_QUIETS};

typedef struct
{
    int stage;
    int move_num;
    int num_moves;
    int capture_piece;
#ifdef HASHTABLE
    s_move hash_move;
#endif
#ifdef KILLER_MOVES
    s_move killer_move;
#endif
#ifdef KILLER_MOVES_2
    s_move killer_move_2;
#endif
    s_move moves[MAX_MOVES];
    int scores[MAX_MOVES];
} s_move_generator;

int next_move(s_board *board, s_move_generator *generator, s_move *move);

int find_moves_pawn_ep(s_board *board, s_move *move_list);
int find_moves_pawn_captures(s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_wP_quiet(s_board *board, s_move *move_list);
int find_moves_bP_quiet(s_board *board, s_move *move_list);
int find_moves_knights(s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_bishops(s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_rooks(s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_queens(s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_bishops_queens(s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_rooks_queens(s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_kings(s_board *board, s_move *move_list, uint64_t allowed);
int find_moves_kings_castles(s_board *board, s_move *move_list);
int find_moves_all(s_board *board, s_move *move_list, int colour);
int find_moves_captures(s_board *board, s_move *move_list, int colour);
int find_moves_quiet(s_board *board, s_move *move_list, int colour);
int can_castle(s_board *board, int turn, int side);

#endif
