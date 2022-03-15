#include "king_safety.h"
#include <assert.h>
#include <stddef.h>
#include "../chess/bitboards.h"

const int king_safety_table[100] = {
   0,   0,   1,   2,   3,   5,   7,   9,  12,  15,
  18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
  68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
 140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
 260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
 377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
 494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500
};

int king_safety(const s_board *board, int side)
{
    assert(board != NULL);
    assert(side == WHITE || side == BLACK);

    int eval = 0;
    int sq = __builtin_ctzll(board->pieces[KINGS] & board->colour[side]);
    const uint64_t surrounding = magic_moves_king(sq);

    // Positive: Nearby friendly pieces
    eval += 5 * __builtin_popcountll(surrounding & board->colour[side]);

    // Negative: Nearby enemy pieces
    //eval -= 5*__builtin_popcountll(surrounding & board->colour[!side]);

    /*
    // Positive: Friendly rooks and queens on the same lines
    eval += 5*__builtin_popcountll(magic_moves_rook(0ULL, sq) & (board->colour[side] & (board->pieces[ROOKS] | board->pieces[QUEENS])));

    // Negative: Enemy rooks and queens on the same lines
    eval -= 5*__builtin_popcountll(magic_moves_rook(0ULL, sq) & (board->colour[!side] & (board->pieces[ROOKS] | board->pieces[QUEENS])));
    */

    // Positive: Defending nearby squares
    /*
    int count = 0;
    uint64_t copy = surrounding;
    while(copy)
    {
        uint64_t pos = copy & ~(copy-1);

        count += count_attackers(board, pos, side);

        copy ^= pos;
    }
    eval += king_safety_table[count];
    */

    // Negative: Opponent attacking nearby squares
    /*
    int count = 0;
    uint64_t copy = surrounding;
    while(copy)
    {
        uint64_t pos = copy & ~(copy-1);

        count += count_attackers(board, pos, !side);

        copy ^= pos;
    }
    eval -= king_safety_table[count];
    */

    return eval;
}
