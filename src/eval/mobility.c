#include "mobility.h"
#include <assert.h>
#include "../chess/bitboards.h"

int piece_mobility(const s_board *board, int side)
{
    assert(board);
    assert(side == WHITE || side == BLACK);

    uint64_t moves = 0;
    uint64_t copy = 0;
    uint64_t allowed = ~board->colour[side];
    int from = 0;

    // Knights
    copy = board->pieces[KNIGHTS] & board->colour[side];
    while (copy)
    {
        from = __builtin_ctzll(copy);
        moves |= magic_moves_knight(from) & allowed;

        copy &= copy - 1;
    }

    // Bishops & Queens
    copy = (board->pieces[BISHOPS] | board->pieces[QUEENS]) & board->colour[side];
    while (copy)
    {
        from = __builtin_ctzll(copy);
        moves |= magic_moves_bishop((board->colour[WHITE] | board->colour[BLACK]), from) & allowed;

        copy &= copy - 1;
    }

    // Rooks & Queens
    copy = (board->pieces[ROOKS] | board->pieces[QUEENS]) & board->colour[side];
    while (copy)
    {
        from = __builtin_ctzll(copy);
        moves |= magic_moves_rook((board->colour[WHITE] | board->colour[BLACK]), from) & allowed;

        copy &= copy - 1;
    }

    int count = __builtin_popcountll(moves);

    return 4 * count;
}
