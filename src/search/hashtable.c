#include <assert.h>
#include "defs.h"
#include "hashtable.h"

s_hashtable_entry *hashtable_poll(s_hashtable *hashtable, const uint64_t key)
{
    assert(hashtable != NULL);

    const int index = key%(hashtable->max_entries);

    return &hashtable->entries[index];
}

s_hashtable_entry *hashtable_add(s_hashtable *hashtable, const int flags, const uint64_t key, const int depth, const int eval, const s_move pv)
{
    assert(hashtable != NULL);
    assert(key != 0);
    assert(depth > 0);
    assert(depth <= MAX_DEPTH);

    const int index = key%(hashtable->max_entries);

    if(hashtable->entries[index].key == 0)
    {
        hashtable->num_entries++;
    }

    hashtable->entries[index].flags = flags;
    hashtable->entries[index].key = key;
    hashtable->entries[index].depth = depth;
    hashtable->entries[index].eval = eval;
    hashtable->entries[index].pv = pv;

    return NULL;
}

int hashtable_init(s_hashtable *hashtable, int size_megabytes)
{
    assert(hashtable != NULL);
    assert(size_megabytes > 0);

    if(hashtable->entries != NULL)
    {
        free(hashtable->entries);
    }

    if(size_megabytes > HASHTABLE_SIZE_MAX)
    {
        size_megabytes = HASHTABLE_SIZE_MAX;
    }

    hashtable->size_bytes = size_megabytes*1024*1024;
    hashtable->num_entries = 0;
    hashtable->max_entries = hashtable->size_bytes/sizeof(s_hashtable_entry);
    hashtable->size_bytes = hashtable->max_entries*sizeof(s_hashtable_entry);

    hashtable->entries = calloc(hashtable->max_entries, sizeof(s_hashtable_entry));

    if(hashtable->entries == NULL)
    {
        hashtable->size_bytes = 0;
        hashtable->num_entries = 0;
        hashtable->max_entries = 0;
        return -1;
    }
    else
    {
        return size_megabytes;
    }
}

void hashtable_clear(s_hashtable *hashtable)
{
    assert(hashtable != NULL);

    hashtable->num_entries = 0;

    for(int i = 0; i < hashtable->max_entries; ++i)
    {
        hashtable->entries[i].flags = 0;
        hashtable->entries[i].depth = 0;
        hashtable->entries[i].eval = 0;
        hashtable->entries[i].key = 0;
        hashtable->entries[i].pv = NO_MOVE;
    }
}

void hashtable_free(s_hashtable *hashtable)
{
    assert(hashtable != NULL);

    if(hashtable->entries) {free(hashtable->entries);}
    free(hashtable);
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
