#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define ENGINE_NAME "Baislicka"
#define ENGINE_VERSION "1.0"
#define ENGINE_AUTHOR "kz04px"
#define MAX_MOVES 256
#define MAX_DEPTH 64
#define SQ_TO_FILE(x) (x%8)
#define SQ_TO_RANK(x) (x/8)
#define SQ_TO_FILE_CHAR(x) (SQ_TO_FILE(x)+'a')
#define SQ_TO_RANK_CHAR(x) (SQ_TO_RANK(x)+'1')
#define GUI_Send(...); printf(__VA_ARGS__); fflush(stdout);
#define INF 1000000
#define CONTEMPT_VALUE 0
#define HASHTABLE_SIZE_DEFAULT 128

//#define ALPHA_BETA
#define PVS

#define HASHTABLE
#define SORT_MOVES
#define QUIESCENCE_SEARCH
#define NULL_MOVE
#define KILLER_MOVES
#define ASPIRATION_WINDOW
#define LMR
//#define FUTILITY_PRUNING
//#define NULL_MOVE_REDUCTIONS

#define CAPTURE_SORT_SEE
//#define CAPTURE_SORT_MVVLVA

//#define QUIET_SORT_PST
#define QUIET_SORT_SEE
//#define QUIET_SORT_HISTORY_HEURISTIC

#define TAPERED_EVAL
#define KING_SAFETY
#define PIECE_MOBILITY
#define PAWN_CHAINS
//#define DELTA_PRUNING
//#define PASSED_PAWN_EVAL
//#define BACKWARD_PAWN_EVAL
//#define KILLER_MOVES_2

#endif // DEFS_H_INCLUDED
