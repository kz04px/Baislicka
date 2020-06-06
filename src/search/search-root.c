#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "defs.h"
#include "search.h"

void* search_root(void *n)
{
    assert(n != NULL);

    s_board *board = ((s_thread_data*)n)->board;
    s_search_settings *settings = ((s_thread_data*)n)->settings;

    assert(board != NULL);
    assert(settings != NULL);

    int target_depth = MAX_DEPTH;
    char move_string[4096];
    int total_time = 0;
    s_pv pv = {0};

    // Info
    s_search_info info;
    info.stop = &settings->stop;
    info.time_start = clock();
    info.nodes = 0;
    info.hashtable_hits = 0;
    info.seldepth = 0;
#ifndef NDEBUG
    for(int i = 0; i < MAX_MOVES; ++i)
    {
        info.num_cutoffs[i] = 0;
    }
#endif

    // Stack
    s_stack stack[MAX_DEPTH];
    for(int i = 0; i < MAX_DEPTH; ++i)
    {
        stack[i].ply = i+1; // FIXME: ply = 0;
        stack[i].null_move = 1;
        stack[i].pv.num_moves = 0;
        //stack[i].killer_move = NO_MOVE;
    }

    // Depth limited search
    if(settings->depth != 0)
    {
        info.time_max = INT_MAX;
        target_depth = settings->depth;
    }
    // Movetime limited search
    else if(settings->movetime != 0)
    {
        info.time_max = settings->movetime;
    }
    // Regular search
    else
    {
        if(board->turn == WHITE)
        {
            info.time_max = settings->wtime/settings->movestogo + settings->winc;
        }
        else
        {
            info.time_max = settings->btime/settings->movestogo + settings->binc;
        }
    }

    for(int depth = 1; depth <= target_depth; ++depth)
    {
        int score = 0;

        if(depth < 3)
        {
            score = pvSearch(&info, stack, board, -INF, INF, depth);
        }
        else
        {
            #define NUM_BOUNDS 3
            const int bounds[NUM_BOUNDS] = {50, 200, INF};

            for(int b = 0; b < NUM_BOUNDS; ++b)
            {
                score = pvSearch(&info, stack, board, -bounds[b], bounds[b], depth);

                if(-bounds[b] < score && score < bounds[b])
                {
                    break;
                }
            }
        }

        clock_t time_spent = 0;
        time_spent = (double)(clock() - info.time_start) * 1000.0 / CLOCKS_PER_SEC;

        total_time += time_spent;

        // Ignore the result if we had to stop
        if(total_time >= info.time_max || *info.stop != 0)
        {
            break;
        }

        // Updates
        pv = stack[0].pv;

        assert(pv.num_moves > 0);
        assert(pv.num_moves < MAX_DEPTH);
        assert(legal_pv(board, &pv) != 0);

        int index = 0;
        move_string[0] = '\0';
        for(int i = 0; i < pv.num_moves; ++i)
        {
            move_string[index] = ' ';
            index++;
            index += move_to_string(&move_string[index], &pv.moves[i]);
        }

        if(score > INF-MAX_DEPTH)
        {
            printf("info depth %i score mate %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, INF-score, info.nodes, total_time, info.seldepth, move_string);
        }
        else if(score < -INF+MAX_DEPTH)
        {
            printf("info depth %i score mate %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, -score-INF, info.nodes, total_time, info.seldepth, move_string);
        }
        else
        {
            printf("info depth %i score cp %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, score, info.nodes, total_time, info.seldepth, move_string);
        }

        // Stop searching if we found a checkmate
        if(score < -INF+MAX_DEPTH || score > INF-MAX_DEPTH)
        {
            break;
        }

        // Try and predict if we'll finish the next search iteration
        if(settings->movetime == 0 && settings->depth == 0 && total_time + 4*time_spent >= info.time_max)
        {
            break;
        }
    }

#ifndef NDEBUG
    int total_cutoffs = 0;
    for(int i = 0; i < MAX_MOVES; ++i)
    {
        total_cutoffs += info.num_cutoffs[i];
    }
    for(int i = 0; i < 8; ++i)
    {
        if(total_cutoffs == 0)
        {
            printf("%i) %i -\n", i, info.num_cutoffs[i]);
        }
        else
        {
            printf("%i) %i %.2f%%\n", i, info.num_cutoffs[i], 100.0*info.num_cutoffs[i]/total_cutoffs);
        }
    }
#endif

    move_to_string(move_string, &pv.moves[0]);
    printf("bestmove %s\n", move_string);
    return NULL;
}
