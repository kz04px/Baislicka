#ifndef SEARCH_HASHTABLE_H
#define SEARCH_HASHTABLE_H

#include "../chess/move.h"

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

extern s_hashtable hashtable;

int hashtable_resize(s_hashtable *table, int size_megabytes);
void hashtable_clear(s_hashtable *table);
void hashtable_free(s_hashtable *table);
s_hashtable_entry *hashtable_poll(s_hashtable *table, const uint64_t key);
s_hashtable_entry *hashtable_add(s_hashtable *table, const int flags, const uint64_t key, const int depth, const int eval, const s_move pv);
int eval_to_tt(const int eval, const int ply);
int eval_from_tt(const int eval, const int ply);

#endif
