#include <stdlib.h>
#include <assert.h>
#include "defs.h"
#include "hashtable.h"

s_hashtable_entry *hashtable_poll(s_hashtable *table, const uint64_t key)
{
    assert(table != NULL);

    const int index = key%(table->max_entries);

    return &table->entries[index];
}

s_hashtable_entry *hashtable_add(s_hashtable *table, const int flags, const uint64_t key, const int depth, const int eval, const s_move pv)
{
    assert(table != NULL);
    assert(key != 0);
    assert(depth > 0);
    assert(depth <= MAX_DEPTH);

    const int index = key%(table->max_entries);

    if(table->entries[index].key == 0)
    {
        table->num_entries++;
    }

    table->entries[index].flags = flags;
    table->entries[index].key = key;
    table->entries[index].depth = depth;
    table->entries[index].eval = eval;
    table->entries[index].pv = pv;

    return NULL;
}

int hashtable_resize(s_hashtable *table, int size_megabytes)
{
    assert(table != NULL);
    assert(size_megabytes > 0);

    // The hashtable is already this size
    if (table->size_bytes == size_megabytes*1024*1024)
    {
        return 0;
    }

    if(table->entries != NULL)
    {
        free(table->entries);
    }

    if(size_megabytes > HASHTABLE_SIZE_MAX)
    {
        size_megabytes = HASHTABLE_SIZE_MAX;
    }

    table->size_bytes = size_megabytes*1024*1024;
    table->num_entries = 0;
    table->max_entries = table->size_bytes/sizeof(s_hashtable_entry);
    table->size_bytes = table->max_entries*sizeof(s_hashtable_entry);
    table->entries = calloc(table->max_entries, sizeof(s_hashtable_entry));

    if(table->entries == NULL)
    {
        table->size_bytes = 0;
        table->num_entries = 0;
        table->max_entries = 0;
        return 0;
    }
    else
    {
        return 1;
    }
}

void hashtable_clear(s_hashtable *table)
{
    assert(table != NULL);

    table->num_entries = 0;

    for(int i = 0; i < table->max_entries; ++i)
    {
        table->entries[i].flags = 0;
        table->entries[i].depth = 0;
        table->entries[i].eval = 0;
        table->entries[i].key = 0;
        table->entries[i].pv = NO_MOVE;
    }
}

void hashtable_free(s_hashtable *table)
{
    assert(table != NULL);

    if(table->entries) {free(table->entries);}
}

int eval_to_tt(const int eval, const int ply)
{
    if(eval >  INF-MAX_DEPTH) {return eval + ply;}
    if(eval < -INF+MAX_DEPTH) {return eval - ply;}
    return eval;
}

int eval_from_tt(const int eval, const int ply)
{
    if(eval >  INF-MAX_DEPTH) {return eval - ply;}
    if(eval < -INF+MAX_DEPTH) {return eval + ply;}
    return eval;
}

s_hashtable hashtable;
