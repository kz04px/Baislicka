#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <inttypes.h>
#include "defs.h"

#define HISTORY_SIZE_MAX 1024

#define U64_FILE_A 0x0101010101010101ULL
#define U64_FILE_B 0x0202020202020202ULL
#define U64_FILE_C 0x0404040404040404ULL
#define U64_FILE_D 0x0808080808080808ULL
#define U64_FILE_E 0x1010101010101010ULL
#define U64_FILE_F 0x2020202020202020ULL
#define U64_FILE_G 0x4040404040404040ULL
#define U64_FILE_H 0x8080808080808080ULL
#define U64_RANK_1 0x00000000000000FFULL
#define U64_RANK_2 0x000000000000FF00ULL
#define U64_RANK_3 0x0000000000FF0000ULL
#define U64_RANK_4 0x00000000FF000000ULL
#define U64_RANK_5 0x000000FF00000000ULL
#define U64_RANK_6 0x0000FF0000000000ULL
#define U64_RANK_7 0x00FF000000000000ULL
#define U64_RANK_8 0xFF00000000000000ULL

#define U64_A1 (U64_FILE_A & U64_RANK_1)
#define U64_B1 (U64_FILE_B & U64_RANK_1)
#define U64_C1 (U64_FILE_C & U64_RANK_1)
#define U64_D1 (U64_FILE_D & U64_RANK_1)
#define U64_E1 (U64_FILE_E & U64_RANK_1)
#define U64_F1 (U64_FILE_F & U64_RANK_1)
#define U64_G1 (U64_FILE_G & U64_RANK_1)
#define U64_H1 (U64_FILE_H & U64_RANK_1)

#define U64_A2 (U64_FILE_A & U64_RANK_2)
#define U64_B2 (U64_FILE_B & U64_RANK_2)
#define U64_C2 (U64_FILE_C & U64_RANK_2)
#define U64_D2 (U64_FILE_D & U64_RANK_2)
#define U64_E2 (U64_FILE_E & U64_RANK_2)
#define U64_F2 (U64_FILE_F & U64_RANK_2)
#define U64_G2 (U64_FILE_G & U64_RANK_2)
#define U64_H2 (U64_FILE_H & U64_RANK_2)

#define U64_A3 (U64_FILE_A & U64_RANK_3)
#define U64_B3 (U64_FILE_B & U64_RANK_3)
#define U64_C3 (U64_FILE_C & U64_RANK_3)
#define U64_D3 (U64_FILE_D & U64_RANK_3)
#define U64_E3 (U64_FILE_E & U64_RANK_3)
#define U64_F3 (U64_FILE_F & U64_RANK_3)
#define U64_G3 (U64_FILE_G & U64_RANK_3)
#define U64_H3 (U64_FILE_H & U64_RANK_3)

#define U64_A4 (U64_FILE_A & U64_RANK_4)
#define U64_B4 (U64_FILE_B & U64_RANK_4)
#define U64_C4 (U64_FILE_C & U64_RANK_4)
#define U64_D4 (U64_FILE_D & U64_RANK_4)
#define U64_E4 (U64_FILE_E & U64_RANK_4)
#define U64_F4 (U64_FILE_F & U64_RANK_4)
#define U64_G4 (U64_FILE_G & U64_RANK_4)
#define U64_H4 (U64_FILE_H & U64_RANK_4)

#define U64_A5 (U64_FILE_A & U64_RANK_5)
#define U64_B5 (U64_FILE_B & U64_RANK_5)
#define U64_C5 (U64_FILE_C & U64_RANK_5)
#define U64_D5 (U64_FILE_D & U64_RANK_5)
#define U64_E5 (U64_FILE_E & U64_RANK_5)
#define U64_F5 (U64_FILE_F & U64_RANK_5)
#define U64_G5 (U64_FILE_G & U64_RANK_5)
#define U64_H5 (U64_FILE_H & U64_RANK_5)

#define U64_A6 (U64_FILE_A & U64_RANK_6)
#define U64_B6 (U64_FILE_B & U64_RANK_6)
#define U64_C6 (U64_FILE_C & U64_RANK_6)
#define U64_D6 (U64_FILE_D & U64_RANK_6)
#define U64_E6 (U64_FILE_E & U64_RANK_6)
#define U64_F6 (U64_FILE_F & U64_RANK_6)
#define U64_G6 (U64_FILE_G & U64_RANK_6)
#define U64_H6 (U64_FILE_H & U64_RANK_6)

#define U64_A7 (U64_FILE_A & U64_RANK_7)
#define U64_B7 (U64_FILE_B & U64_RANK_7)
#define U64_C7 (U64_FILE_C & U64_RANK_7)
#define U64_D7 (U64_FILE_D & U64_RANK_7)
#define U64_E7 (U64_FILE_E & U64_RANK_7)
#define U64_F7 (U64_FILE_F & U64_RANK_7)
#define U64_G7 (U64_FILE_G & U64_RANK_7)
#define U64_H7 (U64_FILE_H & U64_RANK_7)

#define U64_A8 (U64_FILE_A & U64_RANK_8)
#define U64_B8 (U64_FILE_B & U64_RANK_8)
#define U64_C8 (U64_FILE_C & U64_RANK_8)
#define U64_D8 (U64_FILE_D & U64_RANK_8)
#define U64_E8 (U64_FILE_E & U64_RANK_8)
#define U64_F8 (U64_FILE_F & U64_RANK_8)
#define U64_G8 (U64_FILE_G & U64_RANK_8)
#define U64_H8 (U64_FILE_H & U64_RANK_8)

#define U64_CENTER ((U64_FILE_C | U64_FILE_D | U64_FILE_E | U64_FILE_F) & (U64_RANK_3 | U64_RANK_4 | U64_RANK_5 | U64_RANK_6))
#define U64_EDGE   ((U64_FILE_A | U64_FILE_B | U64_FILE_G | U64_FILE_H) | (U64_RANK_1 | U64_RANK_2 | U64_RANK_7 | U64_RANK_8))

#define wKSC 1
#define bKSC 2
#define wQSC 4
#define bQSC 8

enum {A8=56, B8, C8, D8, E8, F8, G8, H8};
enum {A7=48, B7, C7, D7, E7, F7, G7, H7};
enum {A6=40, B6, C6, D6, E6, F6, G6, H6};
enum {A5=32, B5, C5, D5, E5, F5, G5, H5};
enum {A4=24, B4, C4, D4, E4, F4, G4, H4};
enum {A3=16, B3, C3, D3, E3, F3, G3, H3};
enum {A2= 8, B2, C2, D2, E2, F2, G2, H2};
enum {A1= 0, B1, C1, D1, E1, F1, G1, H1};

enum {WHITE, BLACK, BOTH};
enum {PAWNS, KNIGHTS, BISHOPS, ROOKS, QUEENS, KINGS, EMPTY};

typedef struct
{
    uint8_t num_halfmoves;
    uint8_t ep;
    uint8_t castling;
    uint64_t key;
} s_irreversible;

typedef struct
{
    int turn;
    uint8_t castling;
    uint8_t ep;
    uint64_t pieces[6];
    uint64_t colour[2];
    uint64_t key;
    uint8_t num_halfmoves;
    int history_size;
    s_irreversible history[HISTORY_SIZE_MAX];
} s_board;

int set_fen(s_board *board, const char *fen);
int board_equality(const s_board *a, const s_board *b);
int is_endgame(const s_board *board);
int is_fifty_move_draw(const s_board *board);
int is_threefold(const s_board *board);

#endif
