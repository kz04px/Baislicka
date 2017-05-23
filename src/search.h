#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "move.h"
#include <time.h>

typedef struct
{
  int num_moves;
  s_move moves[MAX_DEPTH];
} s_pv;

typedef struct
{
  int ply;
  int seldepth;
  clock_t time_start;
  uint64_t nodes;
  uint64_t hashtable_hits;
  #ifndef NDEBUG
    int num_cutoffs[MAX_MOVES];
  #endif
} s_search_info;

typedef struct
{
  int out_of_time;
  int time_taken;
  int seldepth;
  uint64_t nodes;
  uint64_t hashtable_hits;

  int best_move_num;
  int num_moves;
  s_move moves[MAX_MOVES];
  int evals[MAX_MOVES];
  s_pv pvs[MAX_MOVES];
} s_search_results;

typedef struct
{
  int wtime;
  int btime;
  int winc;
  int binc;
  int movestogo;
  int depth;
  int nodes;
  int mate;
  int movetime;
  clock_t time_max;
} s_search_settings;

typedef struct
{
  s_board *board;
  s_search_settings *settings;
} s_thread_data;

int search_settings_set(s_search_settings settings);
int store_irreversible(s_irreversible *info, s_board *board);
int restore_irreversible(s_irreversible *info, s_board *board);
void *search_root(void *n);
s_search_results search(s_board *board, int depth, int alpha, int beta);
int alpha_beta(s_board *board, s_search_info *info, int alpha, int beta, int depth, int null_move, s_pv *pv);
int pvSearch(s_board *board, s_search_info *info, int alpha, int beta, int depth, int null_move, s_pv *pv);
int killers_clear();

#endif
