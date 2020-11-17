#include "zobrist.h"
#include <assert.h>
#include <stdlib.h>

#define RAND_64 ((uint64_t)(rand() & 0x7FFF) ^ \
                 ((uint64_t)(rand() & 0x7FFF) << 15) ^ \
                 ((uint64_t)(rand() & 0x7FFF) << 30) ^ \
                 ((uint64_t)(rand() & 0x7FFF) << 45) ^ \
                 ((uint64_t)(rand() & 0xF) << 60))

uint64_t create_key_board(const s_board *board)
{
    assert(board != NULL);

    uint64_t key = 0;

    for(int sq = 0; sq < 64; ++sq)
    {
        for(int p = 0; p < 6; ++p)
        {
            if((board->pieces[p]>>sq)&1)
            {
                if((board->colour[WHITE]>>sq)&1)
                {
                    key ^= key_piece_positions[p][WHITE][sq];
                }
                else
                {
                    key ^= key_piece_positions[p][BLACK][sq];
                }
            }
        }
    }

    if(board->turn == WHITE)
    {
        key ^= key_turn;
    }

    if(board->ep)
    {
        key ^= key_ep_file[SQ_TO_FILE(board->ep)];
    }

    key ^= key_castling[board->castling];

    return key;
}

void key_init()
{
    for(int piece = 0; piece < 6; ++piece)
    {
        for(int sq = 0; sq < 64; ++sq)
        {
            key_piece_positions[piece][WHITE][sq] = RAND_64;
            key_piece_positions[piece][BLACK][sq] = RAND_64;
        }
    }

    key_turn = RAND_64;

    for(int i = 0; i < 16; ++i)
    {
        key_castling[i] = RAND_64;
    }

    for(int i = 0; i < 8; ++i)
    {
        key_ep_file[i] = RAND_64;
    }

    key_ksc[WHITE] = key_piece_positions[KINGS][WHITE][E1] ^ key_piece_positions[KINGS][WHITE][G1] ^ key_piece_positions[ROOKS][WHITE][F1] ^ key_piece_positions[ROOKS][WHITE][H1];
    key_ksc[BLACK] = key_piece_positions[KINGS][BLACK][E8] ^ key_piece_positions[KINGS][BLACK][G8] ^ key_piece_positions[ROOKS][BLACK][F8] ^ key_piece_positions[ROOKS][BLACK][H8];

    key_qsc[WHITE] = key_piece_positions[KINGS][WHITE][E1] ^ key_piece_positions[KINGS][WHITE][C1] ^ key_piece_positions[ROOKS][WHITE][D1] ^ key_piece_positions[ROOKS][WHITE][A1];
    key_qsc[BLACK] = key_piece_positions[KINGS][BLACK][E8] ^ key_piece_positions[KINGS][BLACK][C8] ^ key_piece_positions[ROOKS][BLACK][D8] ^ key_piece_positions[ROOKS][BLACK][A8];
}

uint64_t key_piece_positions[6][2][64];
uint64_t key_turn;
uint64_t key_castling[16];
uint64_t key_ep_file[8];
uint64_t key_ksc[2];
uint64_t key_qsc[2];
