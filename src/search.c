#include "defs.h"

#define R 2

s_board *board = NULL;
s_search_settings search_settings;

#ifdef KILLER_MOVES
  uint64_t killer_moves_key[MAX_DEPTH];
  s_move killer_moves[MAX_DEPTH];
#endif

int store_irreversible(s_irreversible *info, s_board *board)
{
  assert(board);
  assert(info);
  
  info->key           = board->key;
  info->num_halfmoves = board->num_halfmoves;
  info->ep            = board->ep;
  info->castling      = board->castling;
  
  return 0;
}

int restore_irreversible(s_irreversible *info, s_board *board)
{
  assert(board);
  assert(info);
  
  board->key           = info->key;
  board->num_halfmoves = info->num_halfmoves;
  board->ep            = info->ep;
  board->castling      = info->castling;
  
  return 0;
}

int qsearch(s_board *board, s_search_info *info, int alpha, int beta)
{
  int stand_pat = eval(board);
  
  if(info->ply > info->seldepth)
  {
    info->seldepth = info->ply;
  }
  
  if(stand_pat >= beta)
  {
    return beta;
  }
  
  if(stand_pat > alpha)
  {
    alpha = stand_pat;
  }
  
  if(info->ply >= MAX_DEPTH)
  {
    return stand_pat;
  }
  
  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves_captures(board, moves, board->turn);
  #ifdef SORT_MOVES
    moves_sort(moves, num_moves);
  #endif
  int score;
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &moves[m]);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
    {
      // Restore old permissions
      restore_irreversible(&permissions, board);
      
      move_undo(board, &moves[m]);
      continue;
    }
  
    info->nodes++;
    
    info->ply++;
    
    score = -qsearch(board, info, -beta, -alpha);
    
    info->ply--;
    
    // Restore old permissions
    restore_irreversible(&permissions, board);
    
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

int search_settings_set(s_search_settings settings)
{
  search_settings = settings;
  return 0;
}

void *search_root(void *n)
{
  assert(n != NULL);
  
  board = (*(s_thread_data*)n).board;
  
  assert(board != NULL);
  
  if(board->turn == WHITE)
  {
    search_settings.time_max = search_settings.wtime/search_settings.movestogo + search_settings.winc;
  }
  else
  {
    search_settings.time_max = search_settings.btime/search_settings.movestogo + search_settings.binc;
  }
  
  int target_depth = 1;
  char move_string[4096];
  char temp[16];
  s_move ponder;
  s_search_results results;
  
  s_move bestmove;
  s_pv *bestmove_pv = NULL;
  int bestmove_eval;
  
  int i;
  for(i = 1; i <= target_depth && i < MAX_DEPTH; ++i)
  {
    int alpha = -INF;
    int beta = INF;
    
    results = search(board, i, alpha, beta);
    
    if(results.out_of_time == 1) {break;}
    
    assert(results.best_move_num >= 0);
    assert(results.best_move_num < results.num_moves);
    
    bestmove = results.moves[results.best_move_num];
    bestmove_pv = &results.pvs[results.best_move_num];
    bestmove_eval = results.evals[results.best_move_num];
    
    move_string[0] = '\0';
    int n;
    for(n = 0; n < bestmove_pv->num_moves; ++n)
    {
      move_to_string(temp, &bestmove_pv->moves[n]);
      sprintf(move_string, "%s %s", move_string, temp);
    }
    
    if(bestmove_eval > INF-MAX_DEPTH)
    {
      GUI_Send("info depth %i score mate %i nodes %"PRIdPTR" time %i seldepth %i pv%s\n", i, INF-bestmove_eval, results.nodes, results.time_taken, results.seldepth, move_string);
    }
    else if(bestmove_eval < -INF+MAX_DEPTH)
    {
      GUI_Send("info depth %i score mate %i nodes %"PRIdPTR" time %i seldepth %i pv%s\n", i, -bestmove_eval-INF, results.nodes, results.time_taken, results.seldepth, move_string);
    }
    else
    {
      GUI_Send("info depth %i score cp %i nodes %"PRIdPTR" time %i seldepth %i pv%s\n", i, bestmove_eval, results.nodes, results.time_taken, results.seldepth, move_string);
    }
    
    if(results.time_taken > 0)
    {
      GUI_Send("info nps %"PRIdPTR"\n", 1000*results.nodes/results.time_taken);
    }
    
    if(bestmove_pv->num_moves > 1)
    {
      ponder = bestmove_pv->moves[1];
    }
    
    if(bestmove_eval < -INF+MAX_DEPTH || bestmove_eval > INF-MAX_DEPTH)
    {
      break;
    }
    
    if(5*results.time_taken < search_settings.time_max)
    {
      target_depth++;
    }
  }
  
  if(i == 1 && results.out_of_time == 1)
  {
    printf("Warning: Didn't complete depth 1 search in time\n");
    bestmove = results.moves[0];
  }
  
  move_to_string(move_string, &bestmove);
  GUI_Send("bestmove %s\n", move_string);
  
  return NULL;
}

s_search_results search(s_board *board, int depth, int alpha, int beta)
{
  assert(board != NULL);
  assert(depth > 0);
  
  s_search_results results;
  results.out_of_time = 0;
  results.time_taken = 0;
  
  int score;
  int best_score = -INF;
  s_pv pv_local;
  
  results.num_moves = find_moves_captures(board, results.moves, board->turn);
  #ifdef SORT_MOVES
    moves_sort(results.moves, results.num_moves);
  #endif
  results.num_moves += find_moves_quiet(board, &results.moves[results.num_moves], board->turn);
  
  s_search_info *info = malloc(1*sizeof(s_search_info));
  info->ply = 0;
  info->time_start = clock();
  info->nodes = 0;
  info->hashtable_hits = 0;
  info->seldepth = 0;
  
  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);
  
  int m;
  for(m = 0; m < results.num_moves; ++m)
  {
    results.pvs[m].num_moves = 0;
    move_make(board, &results.moves[m]);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
    {
      // Restore old permissions
      restore_irreversible(&permissions, board);
      
      move_undo(board, &results.moves[m]);
      continue;
    }
    
    info->nodes++;
    
    if(is_threefold(board) || is_fifty_move_draw(board))
    {
      score = -CONTEMPT_VALUE;
    }
    else
    {
      time_t time_spent = clock() - info->time_start;
      if(time_spent < search_settings.time_max)
      {
        info->ply++;
        
        #ifdef ALPHA_BETA
          score = -alpha_beta(board, info, alpha, beta, depth-1, 1, &pv_local);
        #elif defined(PVS)
          score = -pvSearch(board, info, alpha, beta, depth-1, 1);
        #endif
        
        info->ply--;
      }
      else
      {
        score = eval(board);
        results.out_of_time = 1;
      }
    }
    
    // Restore old permissions
    restore_irreversible(&permissions, board);
    
    move_undo(board, &results.moves[m]);
    
    results.evals[m] = score;
    
    if(score > best_score)
    {
      best_score = score;
      results.best_move_num = m;
      
      results.pvs[m].moves[0] = results.moves[m];
      int i;
      for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH-1; ++i)
      {
        results.pvs[m].moves[i+1] = pv_local.moves[i];
      }
      results.pvs[m].num_moves = pv_local.num_moves + 1;
    }
  }
  
  results.nodes = info->nodes;
  results.seldepth = info->seldepth;
  results.time_taken = clock() - info->time_start;
  
  return results;
}

int alpha_beta(s_board *board, s_search_info *info, int alpha, int beta, int depth, int null_move, s_pv *pv)
{
  assert(board != NULL);
  assert(info != NULL);
  assert(beta >= alpha);
  assert(depth >= 0);
  assert(pv);
  
  // Evaluate draws
  if(is_threefold(board) || is_fifty_move_draw(board))
  {
    return CONTEMPT_VALUE;
  }
  
  // Stop search at maximum depth
  if(info->ply >= MAX_DEPTH)
  {
    return eval(board);
  }
  
  if(depth == 0)
  {
    #ifdef QUIESCENCE_SEARCH
      return qsearch(board, info, alpha, beta);
    #else
      return eval(board);
    #endif
  }
  
  s_pv pv_local = {0};
  s_move_generator gen = {0};
  gen.stage = 0;
  gen.hash_move = NO_MOVE;
  gen.killer_move = NO_MOVE;
  int score;
  
  #ifdef HASHTABLE
    int alpha_original = alpha;
    
    s_hashtable_entry entry = *hashtable_poll(hashtable, board->key);
    int entry_valid = (board->key == entry.key);
    int entry_eval = eval_from_tt(entry.eval, info->ply);
    
    if(entry_valid)
    {
      info->hashtable_hits++;
      //if(move_is_legal(board, &entry.pv))
      //{
        gen.hash_move = entry.pv;
      //}
      
      if(entry.depth >= depth)
      {
        if(entry_eval >= beta)
        {
          if(entry.flags == LOWERBOUND)
          {
            return entry_eval;
          }
        }
        else if(entry.flags == UPPERBOUND)
        {
          return entry_eval;
        }
      }
    }
  #endif
  
  int in_check = square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn);
  
  /*
  if(in_check)
  {
    depth++;
  }
  */
  
  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);
  
  #ifdef NULL_MOVE
    if(null_move && !is_endgame(board) && depth > 2 && !in_check)
    {
      // Make nullmove
      null_make(board);
      
      info->ply++;
      score = -alpha_beta(board, info, -beta, -beta+1, depth-1-R, 0, &pv_local);
      info->ply--;
      
      // Unmake nullmove
      null_undo(board);
      
      // Restore old permissions
      restore_irreversible(&permissions, board);
      
      if(score >= beta)
      {
        return score;
      }
    }
  #endif
  
  #ifdef KILLER_MOVES
    gen.killer_move = NO_MOVE;
    //if(killer_moves_key[info->ply] == board->key && move_is_legal(board, &killer_moves[info->ply]))
    //if(move_is_legal(board, &killer_moves[info->ply]))
    //{
      gen.killer_move = killer_moves[info->ply];
    //}
  #endif
  
  int best_score = -INF;
  
  s_move move;
  s_move best_move;
  
  while(next_move(board, &gen, &move))
  {
    move_make(board, &move);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
    {
      // Restore old permissions
      restore_irreversible(&permissions, board);
      
      move_undo(board, &move);
      continue;
    }
  
    info->nodes++;
    
    time_t time_spent = clock() - info->time_start;
    if(time_spent < search_settings.time_max)
    {
      info->ply++;
      score = -alpha_beta(board, info, -beta, -alpha, depth-1, 1, &pv_local);
      info->ply--;
    }
    else
    {
      score = eval(board);
    }
    
    // Restore old permissions
    restore_irreversible(&permissions, board);
    
    move_undo(board, &move);
    
    if(score > best_score)
    {
      best_score = score;
      #ifdef HASHTABLE
        best_move = move;
      #endif
      
      pv->moves[0] = move;
      int i;
      for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH - 1; ++i)
      {
        pv->moves[i+1] = pv_local.moves[i];
      }
      pv->num_moves = pv_local.num_moves + 1;
      
      if(score > alpha)
      {
        alpha = score;
      }
      
      if(alpha >= beta)
      {
        #ifdef KILLER_MOVES
        if(move.type == QUIET)
        {
          killer_moves_key[info->ply] = board->key;
          killer_moves[info->ply] = move;
        }
        #endif
        
        break;
      }
    }
  }
  
  // If we haven't played a move, then there are none
  if(best_score == -INF)
  {
    if(in_check)
    {
      // Checkmate
      return -INF + info->ply;
    }
    else
    {
      // Stalemate
      return 0;
    }
  }
  
  #ifdef HASHTABLE
    int flag = EXACT;
    if(best_score < alpha_original)
    {
      flag = UPPERBOUND;
    }
    
    hashtable_add(hashtable, flag, board->key, depth, eval_to_tt(best_score, info->ply), best_move);
  #endif
  
  return best_score;
}

int pvSearch(s_board *board, s_search_info *info, int alpha, int beta, int depth, int null_move)
{
  info->nodes++;
  
  // Evaluate draws
  if(is_threefold(board) || is_fifty_move_draw(board))
  {
    return CONTEMPT_VALUE;
  }
  
  // Stop search at maximum depth
  if(info->ply >= MAX_DEPTH)
  {
    return eval(board);
  }
  
  if(depth == 0)
  {
    #ifdef QUIESCENCE_SEARCH
      return qsearch(board, info, alpha, beta);
    #else
      return eval(board);
    #endif
  }
  
  s_move_generator gen = {0};
  gen.stage = 0;
  gen.hash_move = NO_MOVE;
  gen.killer_move = NO_MOVE;
  int score = -INF;
  int bSearchPv = 1;
  
  #ifdef HASHTABLE
    int alpha_original = alpha;
    
    s_hashtable_entry entry = *hashtable_poll(hashtable, board->key);
    int entry_valid = (board->key == entry.key);
    int entry_eval = eval_from_tt(entry.eval, info->ply);
    
    if(entry_valid)
    {
      info->hashtable_hits++;
      gen.hash_move = entry.pv;
      
      if(entry.depth >= depth)
      {
        if(entry_eval >= beta)
        {
          if(entry.flags == LOWERBOUND)
          {
            return entry_eval;
          }
        }
        else if(entry.flags == UPPERBOUND)
        {
          return entry_eval;
        }
      }
    }
  #endif
  
  int in_check = square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn);
  
  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);
  
  #ifdef NULL_MOVE
    if(null_move && alpha != beta && !is_endgame(board) && depth > 2 && !in_check)
    {
      // Make nullmove
      null_make(board);
      
      info->ply++;
      score = -pvSearch(board, info, -beta, -beta+1, depth-1-R, 0);
      info->ply--;
      
      // Unmake nullmove
      null_undo(board);
      
      // Restore old permissions
      restore_irreversible(&permissions, board);
      
      if(score >= beta)
      {
        return score;
      }
    }
  #endif
  
  #ifdef KILLER_MOVES
    gen.killer_move = killer_moves[info->ply];
  #endif
  
  s_move move;
  s_move best_move;
  int best_score = -INF;
  
  while(next_move(board, &gen, &move))
  {
    move_make(board, &move);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
    {
      // Restore old permissions
      restore_irreversible(&permissions, board);
      
      move_undo(board, &move);
      continue;
    }
    
    info->ply++;
    if(bSearchPv)
    {
      score = -pvSearch(board, info, -beta, -alpha, depth - 1, 1);
    }
    else
    {
      score = -pvSearch(board, info, -alpha-1, -alpha, depth - 1, 1);
      if(score > alpha)
      {
        score = -pvSearch(board, info, -beta, -alpha, depth - 1, 1);
      }
    }
    info->ply--;
    
    // Restore old permissions
    restore_irreversible(&permissions, board);
    
    move_undo(board, &move);
    
    if(score > best_score)
    {
      best_score = score;
      #ifdef HASHTABLE
        best_move = move;
      #endif
      
      if(score > alpha)
      {
        alpha = score;
      }
      
      if(alpha >= beta)
      {
        #ifdef KILLER_MOVES
        if(move.type == QUIET)
        {
          killer_moves_key[info->ply] = board->key;
          killer_moves[info->ply] = move;
        }
        #endif
        
        break;
      }
    }
    
    /*
    if(score >= beta)
    {
      #ifdef KILLER_MOVES
      if(moves.type == QUIET)
      {
        killer_moves_key[info->ply] = board->key;
        killer_moves[info->ply] = moves;
      }
      #endif
      
      return beta; // fail-hard beta-cutoff
    }
    if(score > alpha)
    {
      alpha = score; // alpha acts like max in MiniMax
      bSearchPv = 0;
    }
    */
  }
  
  if(best_score == -INF)
  {
    if(in_check)
    {
      // Checkmate
      return -INF + info->ply;
    }
    else
    {
      // Stalemate
      return 0;
    }
  }
  
  #ifdef HASHTABLE
    int flag = EXACT;
    if(alpha < alpha_original)
    {
      flag = UPPERBOUND;
    }
    
    hashtable_add(hashtable, flag, board->key, depth, eval_to_tt(alpha, info->ply), best_move);
  #endif
  
  return alpha; // fail-hard
}