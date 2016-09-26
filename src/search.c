#include "defs.h"

#define INF 99999

s_board *board = NULL;
s_search_info *info = NULL;

clock_t time_start = 0;
uint64_t nodes = 0;
uint64_t hashtable_hits = 0;

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
  int score;
  
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
      
    score = -quiesce(board, -beta, -alpha);
      
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

void *search_base(void *n)
{
  assert(n != NULL);
  
  board = (*(s_thread_data*)n).board;
  info = (*(s_thread_data*)n).info;
  //s_board *board = (*(s_thread_data*)n).board;
  //s_search_info *info = (*(s_thread_data*)n).info;
  
  assert(board != NULL);
  assert(info != NULL);
  
  s_search_results results;
  
  if(board->turn == WHITE)
  {
    info->time_max = info->wtime/30;
  }
  else
  {
    info->time_max = info->btime/30;
  }
  
  int target_depth = 1;
  char move_string[4096];
  char temp[16];
  
  s_move bestmove;
  s_move ponder;
  
  int i;
  for(i = 1; i <= target_depth && i < MAX_DEPTH; ++i)
  {
    results.out_of_time = 0;
    results.eval = 0;
    results.time_taken = 0;
    results.pv.num_moves = 0;
    
    int r = search(board, &results, i);
    
    if(r != 0)
    {
      break;
    }
    else
    {
      move_string[0] = '\0';
      int n;
      for(n = 0; n < results.pv.num_moves; ++n)
      {
        move_to_string(temp, &results.pv.moves[n]);
        sprintf(move_string, "%s %s", move_string, temp);
      }
      
      GUI_Send("info depth %i score cp %i nodes %"PRIdPTR" time %i pv%s\n", i, results.eval, nodes, results.time_taken, move_string);
      if(results.time_taken > 0)
      {
        GUI_Send("info nps %"PRIdPTR"\n", nodes/results.time_taken);
      }
      
      /*
      GUI_Send("info depth %i\n", i);
      GUI_Send("info score cp %i\n", results.eval);
      GUI_Send("info nodes %"PRIdPTR"\n", nodes);
      GUI_Send("info time %i\n", results.time_taken);
      */
      
      bestmove = results.pv.moves[0];
      if(results.pv.num_moves > 1)
      {
        ponder = results.pv.moves[1];
      }
      
      if(10*results.time_taken < info->time_max)
      {
        target_depth++;
      }
    }
  }
  
  move_to_string(move_string, &bestmove);
  GUI_Send("bestmove %s\n", move_string);
  
  /*
  if(results.pv.num_moves == 1)
  {
    move_to_string(move_string, &bestmove);
    GUI_Send("bestmove %s\n", move_string);
  }
  else
  {
    move_to_string(temp, &results.pv.moves[0]);
    sprintf(move_string, "bestmove %s", temp);
    move_to_string(temp, &results.pv.moves[1]);
    sprintf(move_string, "%s ponder %s\n", move_string, temp);
    GUI_Send(move_string);
  }
  */
  
  return NULL;
}

int search(s_board* board, s_search_results *results, int depth)
{
  assert(board != NULL);
  assert(results != NULL);
  assert(depth > 0);
  
  time_t start;
  nodes = 0;
  #ifdef HASHTABLE
    hashtable_hits = 0;
  #endif
  
  start = clock();
  time_start = clock();
  int score;
  
  int best_score = -INF;
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves(board, moves, board->turn);
  moves_sort(moves, num_moves);
  #ifdef GET_PV
    results->pv.num_moves = 0;
    s_pv pv_local;
    pv_local.num_moves = 0;
  #endif
  
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
    
    time_t time_spent = clock() - time_start;
    if(time_spent < info->time_max)
    {
      board->turn = 1-(board->turn);
      
      #ifdef GET_PV
        score = -alpha_beta(board, -INF, INF, depth-1, &pv_local);
      #else
        score = -alpha_beta(board, -INF, INF, depth-1, NULL);
      #endif
      
      board->turn = 1-(board->turn);
    }
    else
    {
      score = eval(board);
      results->out_of_time = 1;
    }
    
    move_undo(board, &moves[m]);
    
    if(score > best_score)
    {
      best_score = score;
      
      #ifdef GET_PV
        results->pv.moves[0] = moves[m];
        int i;
        for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH-1; ++i)
        {
          results->pv.moves[i+1] = pv_local.moves[i];
        }
        results->pv.num_moves = pv_local.num_moves + 1;
      #endif
    }
  }
  
  results->time_taken = clock() - start;
  results->eval = best_score;
  
  return 0;
}

int alpha_beta(s_board* board, int alpha, int beta, int depth, s_pv *pv)
{
  assert(board != NULL);
  assert(depth >= 0);
  #ifdef GET_PV
    assert(pv != NULL);
  #else
    assert(pv == NULL);
  #endif
  
  #ifdef HASHTABLE
    int alpha_original = alpha;
    
    s_hashtable_entry *entry = NULL;
    
    if(hashtable)
    {
      entry = hashtable_poll(hashtable, board->key);
      if(board->key == entry->key && entry->depth >= depth)
      {
        hashtable_hits++;
        if(entry->flags == EXACT)
        {
          #ifdef GET_PV
            pv->num_moves = 0;
          #endif
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
          #ifdef GET_PV
            pv->num_moves = 0;
          #endif
          return entry->eval;
        }
      }
    }
  #endif
  
  #ifdef GET_PV
    s_pv pv_local;
    pv_local.num_moves = 0;
  #endif
  
  #ifdef QUIESCENCE_SEARCH
    if(depth == 0)
    {
      #ifdef GET_PV
        pv->num_moves = 0;
      #endif
      
      return quiesce(board, alpha, beta);
    }
  #else
    if(depth == 0)
    {
      #ifdef GET_PV
        pv->num_moves = 0;
      #endif
      
      return eval(board);
    }
  #endif
  
  int score;
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves(board, moves, board->turn);
  moves_sort(moves, num_moves);
  
  #ifdef HASHTABLE
    int best_move_num = 0;
    if(hashtable)
    {
      if(board->key == entry->key)
      {
        assert(entry->pv >= 0);
        assert(entry->pv < num_moves);

        s_move temp = moves[0];
        moves[0] = moves[entry->pv];
        moves[entry->pv] = temp;
      }
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
    
    time_t time_spent = clock() - time_start;
    if(time_spent < info->time_max)
    {
      board->turn = 1-(board->turn);
      
      #ifdef GET_PV
        score = -alpha_beta(board, -beta, -alpha, depth-1, &pv_local);
      #else
        score = -alpha_beta(board, -beta, -alpha, depth-1, NULL);
      #endif
      
      board->turn = 1-(board->turn);
    }
    else
    {
      score = eval(board);
    }
    
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
      
      #ifdef GET_PV
        assert(pv_local.num_moves >= 0);
        assert(pv_local.num_moves < MAX_DEPTH - 1);
        pv->moves[0] = moves[m];
        int i;
        for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH - 1; ++i)
        {
          pv->moves[i+1] = pv_local.moves[i];
        }
        pv->num_moves = pv_local.num_moves + 1;
      #endif
    }
    if(alpha >= beta)
    {
      break;
    }
  }
  
  // If we haven't played a move, then there are none
  if(!played)
  {
    #ifdef GET_PV
      assert(pv_local.num_moves == 0);
    #endif
    
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
    if(hashtable)
    {
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
    }
  #endif
  
  return best_score;
}