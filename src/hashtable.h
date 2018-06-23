#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

#include "move.h"

#define HASHTABLE_SIZE_MIN        1
#define HASHTABLE_SIZE_MAX     2048

enum {EXACT, LOWERBOUND, UPPERBOUND};

typedef struct
{
    uint8_t flags;
    uint64_t key;
    uint8_t depth;
    int eval;
    s_move pv;
} s_hashtable_entry;

typedef struct
{
    s_hashtable_entry *entries;
    int num_entries;
    int max_entries;
    int size_bytes;
} s_hashtable;

s_hashtable *hashtable;

uint64_t key_piece_positions[6][2][10*12];
uint64_t key_turn;
uint64_t key_castling[16];
uint64_t key_ep_file[8];
uint64_t key_ksc[2];
uint64_t key_qsc[2];

void key_init();
uint64_t create_key_board(s_board *board);
int hashtable_init(s_hashtable *hashtable, int size_megabytes);
void hashtable_clear(s_hashtable *hashtable);
void hashtable_free(s_hashtable *hashtable);
s_hashtable_entry *hashtable_poll(s_hashtable *hashtable, const uint64_t key);
s_hashtable_entry *hashtable_add(s_hashtable *hashtable, const int flags, const uint64_t key, const int depth, const int eval, const s_move pv);
int eval_to_tt(const int eval, const int ply);
int eval_from_tt(const int eval, const int ply);

#endif
