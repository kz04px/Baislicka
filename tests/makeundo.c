#include "tests.h"
#include "chess/attack.h"
#include "chess/bitboards.h"
#include "chess/board.h"
#include "chess/move.h"
#include "chess/movegen.h"

int test_make_undo()
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
        if(set_fen(&board, positions[i])) {continue;}
        uint64_t old_key = board.key;

        s_move moves[MAX_MOVES];
        int num_moves = find_moves_all(&board, &moves[0], board.turn);
        int in_check = square_attacked(&board, board.pieces[KINGS] & board.colour[board.turn], !board.turn);

        // Test nullmove
        if (!in_check)
        {
            null_make(&board);

            // Make sure the hash has changed
            if (board.key == old_key)
            {
                return -2;
            }

            null_undo(&board);

            // Make sure the key is back
            if (board.key != old_key)
            {
                return -2;
            }
        }

        // Test pseudolegal moves
        for(int m = 0; m < num_moves; ++m)
        {
            // Make the move
            move_make(&board, &moves[m]);
            if(square_attacked(&board, board.pieces[KINGS]&board.colour[!board.turn], board.turn))
            {
                move_undo(&board, &moves[m]);
                continue;
            }

            // Make sure the hash has changed
            if (board.key == old_key)
            {
                return -2;
            }

            move_undo(&board, &moves[m]);

            // Make sure the key is back
            if (board.key != old_key)
            {
                return -2;
            }
        }
    }

    return 0;
}
