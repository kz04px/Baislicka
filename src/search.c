#include <limits.h>
#include <assert.h>
#include "defs.h"
#include "search.h"
#include "attack.h"
#include "movegen.h"
#include "bitboards.h"
#include "move.h"
#include "eval.h"
#include "hashtable.h"

#define R 2

int reduction(const int move_num, const int depth, const int in_check, const int move_type)
{
    if(move_num < 4 || depth < 3 || in_check || move_type == CAPTURE || move_type == QUEEN_PROMO || move_type == QUEEN_PROMO_CAPTURE)
    {
        return 0;
    }
    return 1;
}

int pvSearch(s_search_info *info, s_stack *stack, s_board *board, int alpha, int beta, int depth)
{
    assert(info != NULL);
    assert(stack != NULL);
    assert(board != NULL);
    assert(alpha < beta);
    assert(depth >= 0);
    assert(depth <= MAX_DEPTH);

    stack->pv.num_moves = 0;

    // Evaluate draws
    if(is_threefold(board) || is_fifty_move_draw(board))
    {
        return CONTEMPT_VALUE;
    }

    // Stop search at maximum depth
    if(stack->ply >= MAX_DEPTH)
    {
        return evaluate(board);
    }

    int in_check = square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn);

    // Check extensions
    if(in_check)
    {
        depth++;
    }

    if(depth == 0)
    {
#ifdef QUIESCENCE_SEARCH
        return qsearch(info, stack, board, alpha, beta);
#else
        return evaluate(board);
#endif
    }

    clock_t time_spent = 0;
    time_spent = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;

    // Check time
    if(time_spent >= info->time_max || *info->stop != 0)
    {
        return 0;
    }

    if(info->nodes%524288 == 0)
    {
        // Update GUI on our search
        if(time_spent > 0)
        {
            printf("info nps %" PRIu64 "\n", 1000*info->nodes/time_spent);
        }
    }

    s_move_generator gen = {0};
    gen.stage = 0;
#ifdef HASHTABLE
    gen.hash_move = NO_MOVE;
#endif
#ifdef KILLER_MOVES
    gen.killer_move = NO_MOVE;
#endif
#ifdef KILLER_MOVES_2
    gen.killer_move_2 = NO_MOVE;
#endif
    int score = -INF;
    stack->pv.num_moves = 0;

#ifdef HASHTABLE
    int alpha_original = alpha;

    s_hashtable_entry entry = *hashtable_poll(hashtable, board->key);
    int entry_valid = (board->key == entry.key);

    if(entry_valid)
    {
        info->hashtable_hits++;
        gen.hash_move = entry.pv;
        int entry_eval = eval_from_tt(entry.eval, stack->ply);

        if(entry.depth >= depth)
        {
            switch(entry.flags)
            {
                case LOWERBOUND:
                    if(entry_eval >= beta)
                    {
                        // Store pv
                        stack->pv.num_moves = 1;
                        stack->pv.moves[0] = entry.pv;
                        return entry_eval;
                    }
                    break;
                case UPPERBOUND:
                    if(entry_eval <= alpha)
                    {
                        // Store pv
                        stack->pv.num_moves = 1;
                        stack->pv.moves[0] = entry.pv;
                        return entry_eval;
                    }
                    break;
                case EXACT:
                    break;
                default:
                    assert(0);
                    break;
            }
        }
    }
#endif

    int pvnode = (beta - alpha > 1);

#ifdef FUTILITY_PRUNING
    if(!pvnode && stack->null_move && depth == 1 && !in_check && !is_endgame(board))
    {
        int static_eval = evaluate(board);
        if(static_eval - 350 >= beta)
        {
            return static_eval - 350;
            //return qsearch(info, stack, board, alpha, beta);
        }

        /*
        int static_eval = evaluate(board);
        if(static_eval + 350 < alpha)
        {
            //return static_eval;
            return qsearch(info, stack, board, alpha, beta);
        }
        */
    }
#endif

    // Set old permissions
    s_irreversible permissions;
    store_irreversible(&permissions, board);

#ifdef NULL_MOVE
    if(stack->ply > 1 && stack->null_move && !pvnode && depth > 2 && !in_check && !is_endgame(board))
    {
        // Make nullmove
        null_make(board);

        (stack+1)->null_move = 0;
        score = -pvSearch(info, stack+1, board, -beta, -beta+1, depth-1-R);

        // Unmake nullmove
        null_undo(board);

        // Restore old permissions
        restore_irreversible(&permissions, board);

        // Test
        stack->pv.num_moves = 0;

        if(score >= beta)
        {
            return score;
        }
    }
#endif

#ifdef NULL_MOVE_REDUCTIONS
    if(stack->null_move && !in_check && depth > 4)
    {
        int new_r = depth > 6 ? 4 : 3;

        // Make nullmove
        null_make(board);

        (stack+1)->null_move = 0;
        score = -pvSearch(info, stack+1, board, -beta, -beta+1, depth-1-new_r);

        // Unmake nullmove
        null_undo(board);

        if(score >= beta)
        {
            depth -= 4;
            if(depth <= 0)
            {
                return qsearch(info, stack, board, alpha, beta);
            }
        }
    }
#endif

#ifdef IID
    if(pvnode && gen.hash_move == NO_MOVE && depth > 3)
    {
        score = -pvSearch(info, stack, board, alpha, beta, depth - 3);
        gen.hash_move = stack->pv.moves[0];
    }
#endif

#ifdef KILLER_MOVES
    gen.killer_move = stack->killer_move;
#endif
#ifdef KILLER_MOVES_2
    if(stack->ply-2 >= 0)
    {
        gen.killer_move_2 = (stack-2)->killer_move;
    }
#endif

    s_move move = NO_MOVE;
    s_move best_move = NO_MOVE;
    int best_score = -INF;
    int move_num = 0;

    // Try the first move
    while(next_move(board, &gen, &move))
    {
        move_make(board, &move);

        if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
        {
            // Restore old permissions
            restore_irreversible(&permissions, board);
            move_undo(board, &move);
            continue;
        }

        move_num++;
        info->nodes++;

        (stack+1)->null_move = 1;
        best_score = -pvSearch(info, stack+1, board, -beta, -alpha, depth - 1);

        // Restore old permissions
        restore_irreversible(&permissions, board);
        move_undo(board, &move);

        if(best_score > alpha)
        {
            if(best_score >= beta)
            {
#ifdef KILLER_MOVES
                if(move_get_type(move) == QUIET)
                {
                    stack->killer_move = move;
                }
#endif

#ifndef NDEBUG
                info->num_cutoffs[0]++;
#endif

#ifdef HASHTABLE
                hashtable_add(hashtable, LOWERBOUND, board->key, depth, eval_to_tt(best_score, stack->ply), move);
#endif

                return best_score;
            }
            alpha = best_score;
        }

        // Store pv line
        stack->pv.moves[0] = move;
        for(int i = 0; i < (stack+1)->pv.num_moves && i < MAX_DEPTH - 1; ++i)
        {
            stack->pv.moves[i+1] = (stack+1)->pv.moves[i];
        }
        stack->pv.num_moves = (stack+1)->pv.num_moves + 1;

#ifdef HASHTABLE
        best_move = move;
#endif

        break;
    }

    (stack+1)->null_move = 1;
    // Try the rest of the moves
    while(next_move(board, &gen, &move))
    {
        move_make(board, &move);

        if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
        {
            // Restore old permissions
            restore_irreversible(&permissions, board);
            move_undo(board, &move);
            continue;
        }

        move_num++;
        info->nodes++;

#ifdef LMR
        int r = reduction(move_num, depth, in_check, move_get_type(move));
        score = -pvSearch(info, stack+1, board, -alpha-1, -alpha, depth - 1 - r);

        // Re-search
        if(score > alpha && score < beta)
        {
            score = -pvSearch(info, stack+1, board, -beta, -alpha, depth - 1);
            if(score > alpha)
            {
                alpha = score;
            }
        }
#else
        score = -pvSearch(info, stack+1, board, -alpha-1, -alpha, depth - 1);

        if(score > alpha && score < beta)
        {
            score = -pvSearch(info, stack, board, -beta, -alpha, depth - 1);
            if(score > alpha)
            {
                alpha = score;
            }
        }
#endif

        // Restore old permissions
        restore_irreversible(&permissions, board);
        move_undo(board, &move);

        if(score > best_score)
        {
            if(score >= beta)
            {
#ifdef KILLER_MOVES
                if(move_get_type(move) == QUIET)
                {
                    stack->killer_move = move;
                }
#endif

#ifdef HASHTABLE
                hashtable_add(hashtable, LOWERBOUND, board->key, depth, eval_to_tt(score, stack->ply), move);
#endif

#ifndef NDEBUG
                info->num_cutoffs[move_num-1]++;
#endif

                return score;
            }

#ifdef HASHTABLE
            best_move = move;
#endif

            // Store pv line
            stack->pv.moves[0] = move;
            for(int i = 0; i < (stack+1)->pv.num_moves && i < MAX_DEPTH - 1; ++i)
            {
                stack->pv.moves[i+1] = (stack+1)->pv.moves[i];
            }
            stack->pv.num_moves = (stack+1)->pv.num_moves + 1;

            best_score = score;
        }
    }

    // If we haven't played a move, then there are none
    if(best_score == -INF)
    {
        if(in_check)
        {
            // Checkmate
            return -INF + stack->ply;
        }
        else
        {
            // Stalemate
            return 0;
        }
    }

#ifdef HASHTABLE
    int flag;
    if(best_score == alpha_original)
    {
        flag = UPPERBOUND;
    }
    else
    {
        flag = EXACT;
    }
    hashtable_add(hashtable, flag, board->key, depth, eval_to_tt(best_score, stack->ply), best_move);

#ifndef NDEBUG
    s_hashtable_entry test_entry = *hashtable_poll(hashtable, board->key);
    assert(test_entry.flags == flag);
    assert(test_entry.key == board->key);
    assert(test_entry.depth == depth);
    assert(eval_from_tt(test_entry.eval, stack->ply) == best_score);
    assert(test_entry.pv == best_move);
#endif

#endif

    return best_score; // fail-hard
}
