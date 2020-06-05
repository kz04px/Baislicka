#ifndef ATTACK_H_INCLUDED
#define ATTACK_H_INCLUDED

#include "board.h"

int square_attacked(const s_board *board, uint64_t pos, int side);
int get_smallest_attacker(const s_board *board, int sq, int side);
int count_attackers(const s_board *board, uint64_t pos, int side);

#endif
