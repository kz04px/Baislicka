#include <limits.h>
#include <assert.h>
#include "defs.h"
#include "search.h"
#include "../chess/attack.h"
#include "../chess/movegen.h"
#include "../chess/bitboards.h"
#include "../chess/move.h"
#include "../eval/eval.h"

int qsearch(s_search_info *info, s_stack *stack, s_board *board, int alpha, int beta)
{
    assert(info != NULL);
    assert(stack != NULL);
    assert(board != NULL);
    assert(alpha < beta);

    int stand_pat = evaluate(board);

    if (stack->ply > info->seldepth)
    {
        info->seldepth = stack->ply - 1;
    }

    if (stand_pat >= beta)
    {
        return beta;
    }

    /*
    // Delta pruning
    const int safety = 900; // The value of a queen

    if (stand_pat < alpha - safety && !is_endgame(board))
    {
        return alpha;
    }
    */

    if (stand_pat > alpha)
    {
        alpha = stand_pat;
    }

    if (stack->ply >= MAX_DEPTH)
    {
        return stand_pat;
    }

    s_move moves[MAX_MOVES];
    int num_moves = find_moves_captures(board, moves, board->turn);
    moves_sort_see(board, moves, num_moves, piece_values);

    for (int m = 0; m < num_moves; ++m)
    {
        int val = see_capture(board, moves[m], piece_values);
        if (val < -50)
        {
            break;
        }

        move_make(board, &moves[m]);

        if (square_attacked(board, board->pieces[KINGS] & board->colour[!board->turn], board->turn))
        {
            move_undo(board, &moves[m]);
            continue;
        }

        info->nodes++;

        int score = -qsearch(info, stack + 1, board, -beta, -alpha);

        move_undo(board, &moves[m]);

        if (score >= beta)
        {
#ifndef NDEBUG
            info->num_cutoffs[m]++;
#endif
            return beta;
        }

        if (score > alpha)
        {
            alpha = score;
        }
    }

    return alpha;
}
