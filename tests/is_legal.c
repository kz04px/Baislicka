#include "tests.h"
#include "chess/bitboards.h"
#include "chess/board.h"
#include "chess/move.h"
#include "chess/movegen.h"

int test_move_legality()
{
    const char positions[16][256] = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"},
        {"k7/8/8/4p3/4P3/8/2P5/K7 w - -"},
        {"k7/1p3p1p/8/8/4P3/5P2/2P5/K7 w - -"},
        {"5B2/6P1/1p6/8/1N6/kP6/2K5/8 w - - 0 1"}
    };
    s_board board;

    for(int i = 0; i < 16; ++i)
    {
        int r = set_fen(&board, positions[i]);
        if(r) {continue;}

        s_move moves[MAX_MOVES];
        int num_moves = find_moves_all(&board, &moves[0], board.turn);

        for(int m = 0; m < num_moves; ++m)
        {
            int legal = is_legal_move(&board, &moves[m]);
            if(!legal) {return -2;}
        }
    }

    return 0;
}
