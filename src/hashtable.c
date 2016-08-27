#include "defs.h"
#define RAND_64 ((uint64_t)rand() | \
                 (uint64_t)rand() << 15 | \
                 (uint64_t)rand() << 30 | \
                 (uint64_t)rand() << 45 | \
                 ((uint64_t)rand() & 0xf) << 60 )

uint64_t create_key_board(s_board *board)
{
  assert(board != NULL);

  uint64_t key = 0;
  
  int sq;
  for(sq = 0; sq < 64; ++sq)
  {
    int p;
    
    // White pieces
    for(p = 0; p < 12; ++p)
    {
      if((board->pieces[p]>>sq)&1)
      {
        key ^= key_piece_positions[p][sq];
        break;
      }
    }
  }

  if(board->turn == WHITE)
  {
    key ^= key_turn;
  }

  if(board->ep)
  {
    key ^= key_ep_col[u64_col(board->ep)];
  }

  if(board->castling[wKSC]) {key ^= key_castling[wKSC];}
  if(board->castling[wQSC]) {key ^= key_castling[wQSC];}
  if(board->castling[bKSC]) {key ^= key_castling[bKSC];}
  if(board->castling[bQSC]) {key ^= key_castling[bQSC];}
  
  return key;
}

void key_init()
{
  int piece;
  for(piece = wP; piece <= bK; ++piece)
  {
    int p;
    for(p = 0; p < 64; ++p)
    {
      key_piece_positions[piece][p] = RAND_64;
    }
  }

  key_turn = RAND_64;

  int i;
  for(i = 0; i < 4; ++i)
  {
    key_castling[i] = RAND_64;
  }

  for(i = 0; i < 8; ++i)
  {
    key_ep_col[i] = RAND_64;
  }
}

s_hashtable_entry *hashtable_poll(s_hashtable *hashtable, uint64_t key)
{
  assert(hashtable != NULL);

  return &hashtable->entries[key%(hashtable->max_entries)];
}

s_hashtable_entry *hashtable_add(s_hashtable *hashtable, int flags, uint64_t key, int depth, int eval, int pv)
{
  assert(hashtable != NULL);
  assert(depth > 0);
  assert(depth <= MAX_DEPTH);

  if(hashtable->entries[key%(hashtable->max_entries)].key == 0)
  {
    hashtable->num_entries++;
  }

  hashtable->entries[key%(hashtable->max_entries)].flags = flags;
  hashtable->entries[key%(hashtable->max_entries)].key = key;
  hashtable->entries[key%(hashtable->max_entries)].depth = depth;
  hashtable->entries[key%(hashtable->max_entries)].eval = eval;
  hashtable->entries[key%(hashtable->max_entries)].pv = pv;

  return &hashtable->entries[key%(hashtable->max_entries)];
}

int hashtable_init(s_hashtable *hashtable, int size_megabytes)
{
  assert(hashtable != NULL);
  assert(size_megabytes >= 0);
  
  if(size_megabytes > 1024)
  {
    size_megabytes = 1024;
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
  
  //hashtable->entries = (s_hashtable_entry*)malloc(hashtable->size_bytes);
  //memset(hashtable->entries, 0, hashtable->size_bytes);
}

void hashtable_clear(s_hashtable *hashtable)
{
  assert(hashtable != NULL);

  hashtable->num_entries = 0;

  int i;
  for(i = 0; i < hashtable->max_entries; ++i)
  {
    hashtable->entries[i].flags = 0;
    hashtable->entries[i].depth = 0;
    hashtable->entries[i].eval = 0;
    hashtable->entries[i].key = 0;
    hashtable->entries[i].pv = 0;
  }
}

void hashtable_free(s_hashtable *hashtable)
{
  assert(hashtable != NULL);

  free(hashtable->entries);
  free(hashtable);
}
