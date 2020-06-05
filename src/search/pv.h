#ifndef SEARCH_PV_H
#define SEARCH_PV_H

#include "../chess/board.h"
#include "../chess/move.h"

typedef struct {
  int num_moves;
  s_move moves[MAX_DEPTH];
} s_pv;

int legal_pv(s_board *board, const s_pv *pv);

#endif
