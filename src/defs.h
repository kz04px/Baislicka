#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define ENGINE_NAME "Baislicka"
#define ENGINE_VERSION "1.0"
#define ENGINE_AUTHOR "kz04px"
#define MAX_MOVES 256
#define MAX_DEPTH 128
#define SQ_TO_FILE(x) (x%8)
#define SQ_TO_RANK(x) (x/8)
#define SQ_TO_FILE_CHAR(x) (SQ_TO_FILE(x)+'a')
#define SQ_TO_RANK_CHAR(x) (SQ_TO_RANK(x)+'1')
#define INF 1000000
#define CONTEMPT_VALUE 0
#define HASHTABLE_SIZE_DEFAULT 128

//#define ALPHA_BETA
#define PVS

#define CAPTURE_SORT_SEE
//#define CAPTURE_SORT_MVVLVA

//#define QUIET_SORT_PST
#define QUIET_SORT_SEE
//#define QUIET_SORT_HISTORY_HEURISTIC

#define SORT_MOVES

// Search
#define HASHTABLE
#define QUIESCENCE_SEARCH
#define NULL_MOVE
#define ASPIRATION_WINDOW
#define LMR
//#define FUTILITY_PRUNING
//#define NULL_MOVE_REDUCTIONS
//#define IID
//#define DELTA_PRUNING

// Move sorting
#define KILLER_MOVES
//#define KILLER_MOVES_2

// Eval
#define TAPERED_EVAL
#define KING_SAFETY
#define PIECE_MOBILITY
#define PAWN_CHAINS
//#define PASSED_PAWN_EVAL
//#define BACKWARD_PAWN_EVAL
//#define PAWN_BLOCKING
//#define KNIGHT_OUTPOSTS
//#define PIECE_OPEN_SCALING
//#define PINNED_PIECE_EVAL

#endif // DEFS_H_INCLUDED
