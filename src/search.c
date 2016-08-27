#include "defs.h"

#define INF 99999

uint64_t nodes;
uint64_t hashtable_hits;

int quiesce(s_board *board, int alpha, int beta)
{
  int stand_pat = eval(board);
  
  if(stand_pat >= beta)
  {
    return beta;
  }
  
  if(stand_pat > alpha)
  {
    alpha = stand_pat;
  }
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves(board, moves, board->turn);
  moves_sort(moves, num_moves);
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    if(moves[m].type != CAPTURE)
    {
      // Move list has to be sorted for this to use break - otherwise use continue
      break;
    }
    
    move_make(board, &moves[m]);
    
    if(board->turn == WHITE)
    {
      if(calculate_attacked_black(board, board->pieces[wK]))
      {
        move_undo(board, &moves[m]);
        continue;
      }
    }
    else
    {
      if(calculate_attacked_white(board, board->pieces[bK]))
      {
        move_undo(board, &moves[m]);
        continue;
      }
    }
    
    nodes++;
    
    board->turn = 1-(board->turn);
    
    int score = -quiesce(board, -beta, -alpha);
    
    board->turn = 1-(board->turn);
    
    move_undo(board, &moves[m]);
 
    if(score >= beta)
    {
      return beta;
    }
    if(score > alpha)
    {
      alpha = score;
    }
  }
  
  return alpha;
}

void search(s_board* board, int depth)
{
  assert(board != NULL);
  assert(depth >= 0);
  
  if(depth == 0)
  {
    // Results
    printf("Depth: %i\n", depth);
    printf("Eval: %i\n", eval(board));
    printf("\n");
    
    return;
  }
  
  time_t start;
  double time_taken;
  nodes = 0;
  #ifdef HASHTABLE
    hashtable_hits = 0;
  #endif
  
  start = clock();
  
  int best_score = -INF;
  
  s_move* best_move = NULL;
  s_move moves[MAX_MOVES];
  int num_moves = find_moves(board, moves, board->turn);
  moves_sort(moves, num_moves);
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {    
    move_make(board, &moves[m]);
    
    if(board->turn == WHITE)
    {
      if(calculate_attacked_black(board, board->pieces[wK]))
      {
        move_undo(board, &moves[m]);
        continue;
      }
    }
    else
    {
      if(calculate_attacked_white(board, board->pieces[bK]))
      {
        move_undo(board, &moves[m]);
        continue;
      }
    }
    
    nodes++;
    
    board->turn = 1-(board->turn);
    
    int score = -alpha_beta(board, -INF, INF, depth-1);
    
    board->turn = 1-(board->turn);
    
    move_undo(board, &moves[m]);
    
    if(score > best_score)
    {
      best_score = score;
      best_move = &moves[m];
    }
  }
  
  time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
  
  // Results
  if(board->turn == WHITE)
  {
    printf("Turn:  White\n");
  }
  else
  {
    printf("Turn:  Black\n");
  }
  printf("Depth: %i\n", depth);
  print_move(*best_move);
  if(abs(best_score) < INF - MAX_DEPTH)
  {
    if(board->turn == WHITE)
    {
      printf("Eval: %i\n", best_score);
    }
    else
    {
      printf("Eval: %i\n", -best_score);
    }
  }
  else
  {
    if(board->turn == WHITE)
    {
      printf("Eval: #%i\n", depth + ((INF - MAX_DEPTH) - best_score));
    }
    else
    {
      printf("Eval: #-%i\n", depth + ((INF - MAX_DEPTH) - best_score));
    }
  }
  printf("Time: %.3fs\n", time_taken);
  printf("Nodes: %I64u\n", nodes);
  #ifdef HASHTABLE
    printf("Hashtable entries: %i (%.3f%%)\n", hashtable->num_entries, 100.0*(float)hashtable->num_entries/hashtable->max_entries);
    printf("Hashtable hits: %I64u\n", hashtable_hits);
  #endif
  printf("kNPS: %.3f\n", nodes/(1000.0*time_taken));
  printf("\n");
}

int alpha_beta(s_board* board, int alpha, int beta, int depth)
{
  assert(board != NULL);
  assert(depth >= 0);
  
  #ifdef HASHTABLE
    int alpha_original = alpha;
    
    s_hashtable_entry *entry = hashtable_poll(hashtable, board->key);
    if(board->key == entry->key && entry->depth >= depth)
    {
      hashtable_hits++;
      if(entry->flags == EXACT)
      {
        return entry->eval;
      }
      else if(entry->flags == LOWERBOUND)
      {
        if(entry->eval > alpha)
        {
          alpha = entry->eval;
        }
      }
      else if(entry->flags == UPPERBOUND)
      {
        if(entry->eval < beta)
        {
          beta = entry->eval;
        }
      }

      if(alpha >= beta)
      {
        return entry->eval;
      }
    }
  #endif
  
  #ifdef QUIESCENCE_SEARCH
    if(depth == 0) {return quiesce(board, alpha, beta);}
  #else
    if(depth == 0) {return eval(board);}
  #endif
  
  int score;
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves(board, moves, board->turn);
  moves_sort(moves, num_moves);
  
  #ifdef HASHTABLE
    int best_move_num = 0;
    if(board->key == entry->key)
    {
      assert(entry->pv >= 0);
      assert(entry->pv < num_moves);

      s_move temp = moves[0];
      moves[0] = moves[entry->pv];
      moves[entry->pv] = temp;
    }
  #endif
  
  int best_score = -INF;
  
  int played = 0;
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &moves[m]);
    
    if(board->turn == WHITE)
    {
      if(calculate_attacked_black(board, board->pieces[wK]))
      {
        move_undo(board, &moves[m]);
        continue;
      }
    }
    else
    {
      if(calculate_attacked_white(board, board->pieces[bK]))
      {
        move_undo(board, &moves[m]);
        continue;
      }
    }
    
    nodes++;
    
    played = 1;
    
    board->turn = 1-(board->turn);
    
    score = -alpha_beta(board, -beta, -alpha, depth-1);
    
    board->turn = 1-(board->turn);
    
    move_undo(board, &moves[m]);
    
    if(score > best_score)
    {
      best_score = score;
      #ifdef HASHTABLE
        best_move_num = m;
      #endif
    }
    if(score > alpha)
    {
      alpha = score;
    }
    if(alpha >= beta)
    {
      break;
    }
  }
  
  // If we haven't played a move, then there are none
  if(!played)
  {
    if(board->turn == WHITE)
    {
      if(calculate_attacked_black(board, board->pieces[wK]))
      {
        // Checkmate
        return -(INF - (MAX_DEPTH-depth));
      }
      else
      {
        // Stalemate
        return 0;
      }
    }
    else
    {
      if(calculate_attacked_white(board, board->pieces[bK]))
      {
        // Checkmate
        return -(INF - (MAX_DEPTH-depth));
      }
      else
      {
        // Stalemate
        return 0;
      }
    }
  }

  #ifdef HASHTABLE
    assert(best_move_num >= 0);
    assert(best_move_num < num_moves);
    
    int flag = EXACT;
    if(best_score <= alpha_original)
    {
      flag = UPPERBOUND;
    }
    else if(best_score >= beta)
    {
      flag = LOWERBOUND;
    }

    hashtable_add(hashtable, flag, board->key, depth, best_score, best_move_num);
  
    /*
    if(best_score <= alpha_original)
    {
      entry->flags = UPPERBOUND;
    }
    else if(best_score >= beta)
    {
      entry->flags = LOWERBOUND;
    }
    else
    {
      entry->flags = EXACT;
    }
    entry->eval = best_score;
    entry->depth = depth;
    entry->key = board->key;
    assert(best_move_num >= 0);
    assert(best_move_num < num_moves);
    entry->pv = best_move_num;
    */
  #endif
  
  return best_score;
}