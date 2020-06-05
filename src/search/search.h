#ifndef SEARCH_H
#define SEARCH_H

#include "../chess/move.h"
#include "pv.h"
#include <time.h>

typedef struct {
    int ply;
    int null_move;
    s_pv pv;
    s_move killer_move;
} s_stack;

typedef struct {
    int *stop;
    int seldepth;
    clock_t time_start;
    clock_t time_max;
    uint64_t nodes;
    uint64_t hashtable_hits;
#ifndef NDEBUG
    int num_cutoffs[MAX_MOVES];
#endif
} s_search_info;

typedef struct {
    int wtime;
    int btime;
    int winc;
    int binc;
    int movestogo;
    int depth;
    int nodes;
    int mate;
    int movetime;
    int stop;
} s_search_settings;

typedef struct {
    s_board *board;
    s_search_settings *settings;
} s_thread_data;

void *search_root(void *n);
int pvSearch(s_search_info *info, s_stack *stack, s_board *board, int alpha, int beta, int depth);
int qsearch(s_search_info *info, s_stack *stack, s_board *board, int alpha, int beta);

#endif
