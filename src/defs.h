#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define ENGINE_NAME "Baislicka 3.0"
#define ENGINE_AUTHOR "Twipply"
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TEST1_FEN "k7/5n2/8/8/5R2/8/3b1P2/K7 w - - 0 0"
#define TEST2_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define TEST3_FEN "8/3K4/8/3k4/8/8/8/7N w - - 0 0"
#define TEST4_FEN "8/1kp1p3/6N1/4B3/8/8/R3K2R/8 w - - 0 0"
#define U64 unsigned long long int
#define SETBIT(x, n) ((x) = (x) | ((U64)1<<(n)))
#define GETBIT(x, n) (((x)>>(n))&1)
#define MAX_MOVES 256
#define POS_TO_COL_CHAR(x) ('h'-(x%8))
#define POS_TO_ROW_CHAR(x) ((x/8)+'1')

#define FALSE 0
#define TRUE !FALSE

#define U64_COL_H 0x0101010101010101
#define U64_COL_G 0x0202020202020202
#define U64_COL_F 0x0404040404040404
#define U64_COL_E 0x0808080808080808
#define U64_COL_D 0x1010101010101010
#define U64_COL_C 0x2020202020202020
#define U64_COL_B 0x4040404040404040
#define U64_COL_A 0x8080808080808080
#define U64_ROW_1 0x00000000000000FF
#define U64_ROW_2 0x000000000000FF00
#define U64_ROW_3 0x0000000000FF0000
#define U64_ROW_4 0x00000000FF000000
#define U64_ROW_5 0x000000FF00000000
#define U64_ROW_6 0x0000FF0000000000
#define U64_ROW_7 0x00FF000000000000
#define U64_ROW_8 0xFF00000000000000

#define U64_A1 (U64_COL_A & U64_ROW_1)
#define U64_B1 (U64_COL_B & U64_ROW_1)
#define U64_C1 (U64_COL_C & U64_ROW_1)
#define U64_D1 (U64_COL_D & U64_ROW_1)
#define U64_E1 (U64_COL_E & U64_ROW_1)
#define U64_F1 (U64_COL_F & U64_ROW_1)
#define U64_G1 (U64_COL_G & U64_ROW_1)
#define U64_H1 (U64_COL_H & U64_ROW_1)

#define U64_A5 (U64_COL_A & U64_ROW_5)
#define U64_B5 (U64_COL_B & U64_ROW_5)
#define U64_C5 (U64_COL_C & U64_ROW_5)
#define U64_D5 (U64_COL_D & U64_ROW_5)
#define U64_E5 (U64_COL_E & U64_ROW_5)
#define U64_F5 (U64_COL_F & U64_ROW_5)
#define U64_G5 (U64_COL_G & U64_ROW_5)
#define U64_H5 (U64_COL_H & U64_ROW_5)

#define U64_A8 (U64_COL_A & U64_ROW_8)
#define U64_B8 (U64_COL_B & U64_ROW_8)
#define U64_C8 (U64_COL_C & U64_ROW_8)
#define U64_D8 (U64_COL_D & U64_ROW_8)
#define U64_E8 (U64_COL_E & U64_ROW_8)
#define U64_F8 (U64_COL_F & U64_ROW_8)
#define U64_G8 (U64_COL_G & U64_ROW_8)
#define U64_H8 (U64_COL_H & U64_ROW_8)

#define DEBUG

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
  }
#endif

enum {WHITE, BLACK, BOTH};
enum {wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK, EMPTY};
enum {wKSC, wQSC, bKSC, bQSC, NORMAL, DOUBLE_MOVE, CAPTURE, PROMOTE, EP};

enum {H8=56, G8, F8, E8, D8, C8, B8, A8};
enum {H7=48, G7, F7, E7, D7, C7, B7, A7};
enum {H6=40, G6, F6, E6, D6, C6, B6, A6};
enum {H5=32, G5, F5, E5, D5, C5, B5, A5};
enum {H4=24, G4, F4, E4, D4, C4, B4, A4};
enum {H3=16, G3, F3, E3, D3, C3, B3, A3};
enum {H2= 8, G2, F2, E2, D2, C2, B2, A2};
enum {H1= 0, G1, F1, E1, D1, C1, B1, A1};

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
  int turn;
  int castling[4];
  U64 ep;
  U64 pieces[12];
  U64 pieces_colour[2];
  U64 pieces_all;
} s_board;

// bitboards.c
void generateOccupancyVariations(int isRook);
void generateMoveDatabase(int isRook);
int calculate_attacked(s_board *board, U64 sq, int attacking_colour);

// movegen.c
int find_moves(s_board* board, s_move* move_list, int colour);

// fen.c
int set_fen(s_board *board, const char *fen);

// perft.c
void perft(s_board* board, int max_depth, const char* fen);
void perft_split(s_board* board, int depth, const char* fen);

// move.c
s_move move_add(s_board *board, U64 from, U64 to, int type, int piece_type);
void move_make(s_board *board, s_move *move);
void move_undo(s_board *board, s_move *move);

// display.c
void print_move(s_move move);
void print_move_list(s_move* move_list, int num_moves);
void print_u64(U64 board);
void display_board(s_board *board);

#endif // DEFS_H_INCLUDED