#include "tests.h"
#include "chess/bitboards.h"
#include "chess/board.h"
#include "chess/move.h"

int test_passed_pawns()
{
    const char positions[16][256] = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"},
        {"k7/8/8/4p3/4P3/8/2P5/K7 w - -"},
        {"k7/1p3p1p/8/8/4P3/5P2/2P5/K7 w - -"}
    };
    const uint64_t passed_white[16] = {0, U64_C2, 0};
    const uint64_t passed_black[16] = {0, 0, U64_H7};
    s_board board;

    for(int i = 0; i < 16; ++i)
    {
        int r = set_fen(&board, positions[i]);
        if(r) {continue;}

        uint64_t found_white = 0;
        uint64_t found_black = 0;
        uint64_t copy;

        // White pawns
        copy = board.pieces[PAWNS] & board.colour[WHITE];
        while(copy)
        {
            int sq = __builtin_ctzll(copy);
            uint64_t pos = (copy & ~(copy-1));

            if(is_passed_pawn(WHITE, sq, board.pieces[PAWNS]|board.colour[BLACK]))
            {
                found_white ^= pos;
            }

            copy ^= pos;
        }

        // Black pawns
        copy = board.pieces[PAWNS] & board.colour[BLACK];
        while(copy)
        {
            int sq = __builtin_ctzll(copy);
            uint64_t pos = (copy & ~(copy-1));

            if(is_passed_pawn(BLACK, sq, board.pieces[PAWNS]|board.colour[WHITE]))
            {
                found_black ^= pos;
            }

            copy ^= pos;
        }

        if(found_white != passed_white[i] || found_black != passed_black[i])
        {
            return i+1;
        }
    }

    return 0;
}
