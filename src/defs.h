#ifndef DEFS_H
#define DEFS_H

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

//#define KILLER_MOVES_2

// Eval
//#define PASSED_PAWN_EVAL
//#define BACKWARD_PAWN_EVAL
//#define PAWN_BLOCKING
//#define KNIGHT_OUTPOSTS
//#define PIECE_OPEN_SCALING
//#define PINNED_PIECE_EVAL

#endif
