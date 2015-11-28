#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

enum{A8, B8, C8, D8, E8, F8, G8, H8,
     A7, B7, C7, D7, E7, F7, G7, H7,
     A6, B6, C6, D6, E6, F6, G6, H6,
     A5, B5, C5, D5, E5, F5, G5, H5,
     A4, B4, C4, D4, E4, F4, G4, H4,
     A3, B3, C3, D3, E3, F3, G3, H3,
     A2, B2, C2, D2, E2, F2, G2, H2,
     A1, B1, C1, D1, E1, F1, G1, H1};
enum {wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK, EMPTY};
enum {wKSC, wQSC, bKSC, bQSC, NORMAL, DOUBLE_MOVE, CAPTURE, PROMOTE, EP};
enum {WHITE, BLACK, BOTH};
//enum {FALSE, TRUE};

#define TRUE  1
#define FALSE 0

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TEST1_FEN "rnbqkbnr/ppppp1pp/8/8/5p2/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TEST2_FEN "k6N/8/8/4N3/8/1K1p4/8/N7 w - - 0 0"
#define TEST3_FEN "8/3K4/8/3k4/8/8/8/7N w - - 0 0"
#define TEST4_FEN "8/1kp1p3/6N1/4B3/8/8/R3K2R/8 w - - 0 0"
#define MAX_MOVES 256
#define MAX_DEPTH 19

#define TOGGLEBIT(x, n) (x = (x) ^ (1<<(n)))
#define SETBIT(x, n) (x = (x) | ((U64)1<<(n)))
#define GETBIT(x, n) (((x)>>(n))&1)
//#define UNSETBIT(x, n) (x = (x) | (1<<(n)))

#define ROW64(n) (7-n/8)
#define COL64(n) (n%8)
#define ONBOARD64(n) (n < A8 || n > H1 ? FALSE : TRUE)
#define COLOUR(n) (n > wK ? BLACK : WHITE)
#define OFFBOARD (-1)
#define U64_COL_A 0x0101010101010101
#define U64_COL_B 0x0202020202020202
#define U64_COL_C 0x0404040404040404
#define U64_COL_D 0x0808080808080808
#define U64_COL_E 0x1010101010101010
#define U64_COL_F 0x2020202020202020
#define U64_COL_G 0x4040404040404040
#define U64_COL_H 0x8080808080808080
#define U64_ROW_8 0x00000000000000FF
#define U64_ROW_7 0x000000000000FF00
#define U64_ROW_6 0x0000000000FF0000
#define U64_ROW_5 0x00000000FF000000
#define U64_ROW_4 0x000000FF00000000
#define U64_ROW_3 0x0000FF0000000000
#define U64_ROW_2 0x00FF000000000000
#define U64_ROW_1 0xFF00000000000000

#define U64_A1 (U64_COL_A & U64_ROW_1)
#define U64_B1 (U64_COL_B & U64_ROW_1)
#define U64_C1 (U64_COL_C & U64_ROW_1)
#define U64_D1 (U64_COL_D & U64_ROW_1)
#define U64_E1 (U64_COL_E & U64_ROW_1)
#define U64_F1 (U64_COL_F & U64_ROW_1)
#define U64_G1 (U64_COL_G & U64_ROW_1)
#define U64_H1 (U64_COL_H & U64_ROW_1)

#define U64_G2 (U64_COL_G & U64_ROW_2)
#define U64_H2 (U64_COL_H & U64_ROW_2)

#define U64_F3 (U64_COL_F & U64_ROW_3)
#define U64_H3 (U64_COL_H & U64_ROW_3)

#define U64_E4 (U64_COL_E & U64_ROW_4)
#define U64_F4 (U64_COL_F & U64_ROW_4)

#define U64_A8 (U64_COL_A & U64_ROW_8)
#define U64_C8 (U64_COL_C & U64_ROW_8)
#define U64_D8 (U64_COL_D & U64_ROW_8)
#define U64_E8 (U64_COL_E & U64_ROW_8)
#define U64_F8 (U64_COL_F & U64_ROW_8)
#define U64_G8 (U64_COL_G & U64_ROW_8)
#define U64_H8 (U64_COL_H & U64_ROW_8)

//#define DEBUG
//#define HASHTABLE

#ifndef DEBUG
  #define ASSERT(n)
#else
#define ASSERT(n) \
  if(!(n)) \
  { \
    fprintf(stdout, "\n%s - Failed\n", #n); \
    fprintf(stdout, "On %s\n", __DATE__); \
    fprintf(stdout, "At %s\n", __TIME__); \
    fprintf(stdout, "In File %s\n", __FILE__); \
    fprintf(stdout, "At Line %d\n", __LINE__); \
    getchar(); \
    exit(1); \
  }
#endif

typedef unsigned long long int U64;

typedef struct
{
	U64 from;
	U64 to;
	int taken;
	int piece_type;
	int piece_num;
	int type;
	int promotion;
	
	int ep_old;
	int wKSC_old;
	int wQSC_old;
	int bKSC_old;
	int bQSC_old;
} s_move;

typedef struct
{
	s_move history[512];
	int turn;
	U64 ep;
	int castling[4];
	
	U64 pieces[12];
	U64 all_pieces[3];
	U64 attacking[12];
	U64 all_attacking[2];
	int num_pieces[12];
	int locations[12][10];
} s_board;

typedef struct
{
	U64 total;
	U64 normal;
	U64 takes;
	U64 checks;
	U64 checkmates;
	U64 eps;
	U64 promotions;
	U64 wKSCs;
	U64 wQSCs;
	U64 bKSCs;
	U64 bQSCs;
} s_moves_found;

// attacking.c
int in_check(s_board *board, int colour);
int calculate_attacked(s_board *board, U64 sq, int attacking_colour);
int white_attacking(s_board *board, U64 positions);
void Update_attacking(s_board *board);

// display.c
void display_board(s_board *board);
void display_bitboard(U64 Our_Bitboard);

// board.c
void Add_Piece(s_board *board, int type, int location);
void Init_Board(s_board *board);
int U64_to_Col(U64 pos);
int U64_to_Row(U64 pos);

// io.c
char Int_to_Char(int n);
void Print_Piecelist(s_board *board);
int set_fen(s_board *board, const char *string);

// evaluate.c
int evaluate(s_board *board);

// move.c
int move_print(s_move *our_move);
s_move add_move(s_board *board, U64 from, U64 to, int type, int piece);
s_move add_promotion_move(s_board *board, U64 from, U64 to, int piece, int promo_piece);
void make_move(s_board *board, s_move *move);
void undo_move(s_board *board, s_move *move);

// movegen.c
int find_moves(s_board *board, s_move *moves);

// search.c
void Search(s_board *board, int depth);

// attacking.c
int calculate_attacked(s_board *board, U64 sq, int colour);
void update_attacking(s_board *board);
int white_attacking(s_board *board, U64 positions);
int black_attacking(s_board *board, U64 positions);

// perft.c
void perft_threaded_search(char *fen, int depth, int num_threads);
s_moves_found perft_exact(s_board *board, int depth);
void perft_suite(s_board *board, char *filename, int max_depth);
int test_positions(s_board *board);

#endif // DEFS_H_INCLUDED