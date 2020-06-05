#include "bitboards.h"
#include "board.h"
#include "move.h"
#include "movegen.h"
#include "tests.h"

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
        int r = set_fen(&board, positions[i]);
        if(r) {continue;}

        s_board nboard = board;

        // Set old permissions
        s_irreversible permissions;
        store_irreversible(&permissions, &board);

        s_move moves[MAX_MOVES];
        int num_moves = find_moves_all(&board, &moves[0], board.turn);

        for(int m = 0; m < num_moves; ++m)
        {
            move_make(&board, &moves[m]);

            if(board_equality(&board, &nboard))
            {
                return -1;
            }

            // Restore old permissions
            restore_irreversible(&permissions, &board);

            move_undo(&board, &moves[m]);
        }

        if(!board_equality(&board, &nboard))
        {
            return -2;
        }
    }

    s_board nboard = board;

    // Set old permissions
    s_irreversible permissions;
    store_irreversible(&permissions, &board);

    null_make(&board);

    if(board_equality(&board, &nboard))
    {
        return -3;
    }

    null_undo(&board);

    // Restore old permissions
    restore_irreversible(&permissions, &board);

    if(!board_equality(&board, &nboard))
    {
        return -4;
    }

    return 0;
}
