#include "defs.h"

#define R 2

s_board *board = NULL;
s_search_info search_info;

clock_t time_start = 0;
uint64_t nodes = 0;
uint64_t hashtable_hits = 0;
int search_depth = 0;

#ifdef KILLER_MOVES
  uint64_t killer_moves_key[MAX_DEPTH];
  s_move killer_moves[MAX_DEPTH];
#endif

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
  int num_moves = find_moves_captures(board, moves, board->turn);
  #ifdef SORT_MOVES
    moves_sort(moves, num_moves);
  #endif
  int score;
  
  // Set old permissions
  #ifdef HASHTABLE
    uint64_t key_old = board->key;
  #endif
  uint8_t num_halfmoves_old = board->num_halfmoves;
  uint8_t ep_old = board->ep;
  uint8_t castling_old = board->castling;
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &moves[m]);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn))
    {
      // Restore old permissions
      #ifdef HASHTABLE
        board->key = key_old;
      #endif
      board->ep = ep_old;
      board->num_halfmoves = num_halfmoves_old;
      board->castling = castling_old;
      
      move_undo(board, &moves[m]);
      continue;
    }
    
    nodes++;
    
    board->turn = 1-(board->turn);
    
    score = -quiesce(board, -beta, -alpha);
    
    board->turn = 1-(board->turn);
    
    // Restore old permissions
    #ifdef HASHTABLE
      board->key = key_old;
    #endif
    board->ep = ep_old;
    board->num_halfmoves = num_halfmoves_old;
    board->castling = castling_old;
    
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

int search_info_set(s_search_info info)
{
  search_info = info;
  return 0;
}

void *search_base(void *n)
{
  assert(n != NULL);
  
  board = (*(s_thread_data*)n).board;
  
  assert(board != NULL);
  
  if(board->turn == WHITE)
  {
    search_info.time_max = search_info.wtime/search_info.movestogo + search_info.winc;
  }
  else
  {
    search_info.time_max = search_info.btime/search_info.movestogo + search_info.binc;
  }
  
  int target_depth = 1;
  char move_string[4096];
  char temp[16];
  s_move ponder;
  s_search_results results;
  
  s_move *bestmove = NULL;
  s_pv *bestmove_pv = NULL;
  int *bestmove_eval = NULL;
  
  int i;
  for(i = 1; i <= target_depth && i < MAX_DEPTH; ++i)
  {
    results = search(board, i);
    
    if(results.out_of_time == 1) {break;}
    
    assert(results.best_move_num >= 0);
    assert(results.best_move_num < results.num_moves);
    
    bestmove = &results.moves[results.best_move_num];
    bestmove_pv = &results.pvs[results.best_move_num];
    bestmove_eval = &results.evals[results.best_move_num];
    
    move_string[0] = '\0';
    int n;
    for(n = 0; n < bestmove_pv->num_moves; ++n)
    {
      move_to_string(temp, &bestmove_pv->moves[n]);
      sprintf(move_string, "%s %s", move_string, temp);
    }
    
    if(board->turn == WHITE)
    {
      if(*bestmove_eval > INF-MAX_DEPTH)
      {
        GUI_Send("info depth %i score mate %i nodes %"PRIdPTR" time %i pv%s\n", i, INF-(*bestmove_eval), nodes, results.time_taken, move_string);
      }
      else if(*bestmove_eval < -INF+MAX_DEPTH)
      {
        GUI_Send("info depth %i score mate %i nodes %"PRIdPTR" time %i pv%s\n", i, -(*bestmove_eval)-INF, nodes, results.time_taken, move_string);
      }
      else
      {
        GUI_Send("info depth %i score cp %i nodes %"PRIdPTR" time %i pv%s\n", i, *bestmove_eval, nodes, results.time_taken, move_string);
      }
    }
    else
    {
      if(*bestmove_eval < -INF+MAX_DEPTH)
      {
        GUI_Send("info depth %i score mate %i nodes %"PRIdPTR" time %i pv%s\n", i, -INF-(*bestmove_eval), nodes, results.time_taken, move_string);
      }
      else if(*bestmove_eval > INF-MAX_DEPTH)
      {
        GUI_Send("info depth %i score mate %i nodes %"PRIdPTR" time %i pv%s\n", i, -(*bestmove_eval)+INF, nodes, results.time_taken, move_string);
      }
      else
      {
        GUI_Send("info depth %i score cp %i nodes %"PRIdPTR" time %i pv%s\n", i, *bestmove_eval, nodes, results.time_taken, move_string);
      }
    }
    
    if(results.time_taken > 0)
    {
      GUI_Send("info nps %"PRIdPTR"\n", 1000*nodes/results.time_taken);
    }
    
    if(bestmove_pv->num_moves > 1)
    {
      ponder = bestmove_pv->moves[1];
    }
    
    if(*bestmove_eval < -INF+MAX_DEPTH || *bestmove_eval > INF-MAX_DEPTH)
    {
      break;
    }
    
    if(5*results.time_taken < search_info.time_max)
    {
      target_depth++;
    }
  }
  
  if(i == 1 && results.out_of_time == 1)
  {
    printf("Warning: Didn't complete depth 1 search in time\n");
    bestmove = &results.moves[0];
  }
  
  move_to_string(move_string, bestmove);
  GUI_Send("bestmove %s\n", move_string);
  
  /*
  // Test
  int m;
  for(m = 0; m < results.num_moves; ++m)
  {
    if(results.evals[m] >= 0)
    {
      printf(" %i  ", results.evals[m]);
    }
    else
    {
      printf("%i  ", results.evals[m]);
    }
    print_move(results.moves[m]);
  }
  */
  
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

s_search_results search(s_board* board, int depth)
{
  assert(board != NULL);
  assert(depth > 0);
  
  s_search_results results;
  results.out_of_time = 0;
  results.time_taken = 0;
  
  search_depth = depth;
  nodes = 0;
  #ifdef HASHTABLE
    hashtable_hits = 0;
  #endif
  
  time_start = clock();
  int score;
  
  int best_score = -INF;
  
  results.num_moves = find_moves_captures(board, results.moves, board->turn);
  #ifdef SORT_MOVES
    moves_sort(results.moves, results.num_moves);
  #endif
  results.num_moves += find_moves_quiet(board, &results.moves[results.num_moves], board->turn);
  
  // Set old permissions
  #ifdef HASHTABLE
    uint64_t key_old = board->key;
  #endif
  uint8_t num_halfmoves_old = board->num_halfmoves;
  uint8_t ep_old = board->ep;
  uint8_t castling = board->castling;
  
  s_pv pv_local;
  pv_local.num_moves = 0;
  
  int m;
  for(m = 0; m < results.num_moves; ++m)
  {
    results.pvs[m].num_moves = 0;
    move_make(board, &results.moves[m]);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn))
    {
      // Restore old permissions
      #ifdef HASHTABLE
        board->key = key_old;
      #endif
      board->ep = ep_old;
      board->num_halfmoves = num_halfmoves_old;
      board->castling = castling;
      
      move_undo(board, &results.moves[m]);
      continue;
    }
    
    nodes++;
    
    if(is_threefold(board) || is_fifty_move_draw(board))
    {
      score = -CONTEMPT_VALUE;
    }
    else
    {
      time_t time_spent = clock() - time_start;
      if(time_spent < search_info.time_max)
      {
        board->turn = 1-(board->turn);
        
        score = -alpha_beta(board, -INF, INF, depth-1, 1, &pv_local);
        
        board->turn = 1-(board->turn);
      }
      else
      {
        score = eval(board);
        results.out_of_time = 1;
      }
    }
    
    // Restore old permissions
    #ifdef HASHTABLE
      board->key = key_old;
    #endif
    board->ep = ep_old;
    board->num_halfmoves = num_halfmoves_old;
    board->castling = castling;
    
    move_undo(board, &results.moves[m]);
    
    results.pvs[m].moves[0] = results.moves[m];
    int i;
    for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH-1; ++i)
    {
      results.pvs[m].moves[i+1] = pv_local.moves[i];
    }
    results.pvs[m].num_moves = pv_local.num_moves + 1;
    
    results.evals[m] = score;
    
    if(score > best_score)
    {
      best_score = score;
      results.best_move_num = m;
    }
  }
  
  results.nodes = nodes;
  results.time_taken = clock() - time_start;
  
  return results;
}

int alpha_beta(s_board* board, int alpha, int beta, int depth, int null_allowed, s_pv *pv)
{
  assert(board != NULL);
  assert(depth >= 0);
  assert(pv != NULL);
  
  // Test
  if(is_threefold(board) || is_fifty_move_draw(board))
  {
    return CONTEMPT_VALUE;
  }
  
  int score;
  s_pv pv_local;
  pv_local.num_moves = 0;
  
  #ifdef HASHTABLE
    int alpha_original = alpha;
    
    s_hashtable_entry *entry = hashtable_poll(hashtable, board->key);
    
    if(board->key == entry->key && entry->depth >= depth)
    {
      hashtable_hits++;
      
      /*
      // TEST
      // Update whether the entry found is a threefold repetition or not
      if(move_is_legal(board, &entry->pv))
      {
        // Set old permissions
        #ifdef HASHTABLE
          uint64_t key_old = board->key;
        #endif
        uint8_t num_halfmoves_old = board->num_halfmoves;
        uint8_t ep_old = board->ep;
        uint8_t castling_old = board->castling;
        
        move_make(board, &entry->pv);
        
        if(is_threefold(board))
        {
          entry->eval = -CONTEMPT_VALUE;
        }
        
        // Restore old permissions
        #ifdef HASHTABLE
          board->key = key_old;
        #endif
        board->ep = ep_old;
        board->num_halfmoves = num_halfmoves_old;
        board->castling = castling_old;
        
        move_undo(board, &entry->pv);
      }
      */
      
      if(entry->flags == EXACT)
      {
        pv->num_moves = 0;
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
        pv->num_moves = 0;
        return entry->eval;
      }
    }
  #endif
  
  if(depth == 0)
  {
    pv->num_moves = 0;
    #ifdef QUIESCENCE_SEARCH
      return quiesce(board, alpha, beta);
    #else
      return eval(board);
    #endif
  }
  
  #ifdef NULL_MOVE
    if(!is_endgame(board) && null_allowed && depth > 2 && !square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn))
    {
      // Store
      uint8_t ep_old = board->ep;
      uint8_t num_halfmoves_old = board->num_halfmoves;
      
      // Make null move
      board->ep = 0;
      board->key ^= key_turn;
      board->num_halfmoves = 0;
      board->turn = 1-(board->turn);
      board->key_history[board->history_size] = board->key;
      board->history_size++;
      
      score = -alpha_beta(board, -beta, -beta+1, depth-1-R, 0, &pv_local);
      
      // Undo null move
      board->ep = ep_old;
      board->key ^= key_turn;
      board->num_halfmoves = num_halfmoves_old;
      board->turn = 1-(board->turn);
      board->history_size--;
      
      if(score >= beta)
      {
        pv->num_moves = 0;
        return score;
      }
    }
  #endif
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves_captures(board, &moves[0], board->turn);
  #ifdef SORT_MOVES
    moves_sort(moves, num_moves);
  #endif
  num_moves += find_moves_quiet(board, &moves[num_moves], board->turn);
  
  #ifdef HASHTABLE
    int best_move_num = 0;
    if(board->key == entry->key)
    {
      /*
      if(move_is_legal(board, &entry->pv))
      {
        s_move store = entry->pv;
        move_make(board, &store);
        
        if(!square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn))
        {
          nodes++;
          
          board->turn = 1-(board->turn);
          score = -alpha_beta(board, -beta, -alpha, depth-1, 1, &pv_local);
          board->turn = 1-(board->turn);
          
          if(score > alpha)
          {
            alpha = score;
            
            assert(pv_local.num_moves >= 0);
            assert(pv_local.num_moves < MAX_DEPTH - 1);
            pv->moves[0] = store;
            int i;
            for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH - 1; ++i)
            {
              pv->moves[i+1] = pv_local.moves[i];
            }
            pv->num_moves = pv_local.num_moves + 1;
          }
          if(alpha >= beta)
          {
            move_undo(board, &store);
            return score;
          }
        }
        
        move_undo(board, &store);
      }
      */
      
      int i;
      for(i = 0; i < num_moves; ++i)
      {
        if(moves[i].from == entry->pv.from &&
           moves[i].to   == entry->pv.to &&
           moves[i].type == entry->pv.type)
        {
          s_move temp = moves[0];
          moves[0] = moves[i];
          moves[i] = temp;
          break;
        }
      }
    }
  #endif
  
  #ifdef KILLER_MOVES
    int i;
    for(i = 1; i < num_moves; ++i)
    {
      if(moves[i].from == killer_moves[search_depth-depth].from &&
         moves[i].to   == killer_moves[search_depth-depth].to &&
         moves[i].type == killer_moves[search_depth-depth].type)
      {
        s_move temp = moves[1];
        moves[1] = moves[i];
        moves[i] = temp;
        break;
      }
    }
  #endif
  
  /*
  s_move moves[MAX_MOVES];
  int num_moves = find_moves_captures(board, &moves[0], board->turn);
  #ifdef SORT_MOVES
    moves_sort(moves, num_moves);
  #endif
  num_moves += find_moves_quiet(board, &moves[num_moves], board->turn);
  */
  
  // Set old permissions
  #ifdef HASHTABLE
    uint64_t key_old = board->key;
  #endif
  uint8_t num_halfmoves_old = board->num_halfmoves;
  uint8_t ep_old = board->ep;
  uint8_t castling_old = board->castling;
  
  int best_score = -INF;
  int played = 0;
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &moves[m]);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn))
    {
      // Restore old permissions
      #ifdef HASHTABLE
        board->key = key_old;
      #endif
      board->ep = ep_old;
      board->num_halfmoves = num_halfmoves_old;
      board->castling = castling_old;
      
      move_undo(board, &moves[m]);
      continue;
    }
    
    nodes++;
    played = 1;
    
    time_t time_spent = clock() - time_start;
    if(time_spent < search_info.time_max)
    {
      board->turn = 1-(board->turn);
      
      score = -alpha_beta(board, -beta, -alpha, depth-1, 1, &pv_local);
      
      board->turn = 1-(board->turn);
    }
    else
    {
      score = eval(board);
    }
    
    // Restore old permissions
    #ifdef HASHTABLE
      board->key = key_old;
    #endif
    board->ep = ep_old;
    board->num_halfmoves = num_halfmoves_old;
    board->castling = castling_old;
    
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
      
      assert(pv_local.num_moves >= 0);
      assert(pv_local.num_moves < MAX_DEPTH - 1);
      pv->moves[0] = moves[m];
      int i;
      for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH - 1; ++i)
      {
        pv->moves[i+1] = pv_local.moves[i];
      }
      pv->num_moves = pv_local.num_moves + 1;
    }
    if(alpha >= beta)
    {
      break;
    }
  }
  
  // If we haven't played a move, then there are none
  if(!played)
  {
    assert(pv_local.num_moves == 0);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn))
    {
      // Checkmate
      assert(search_depth - depth > 0);
      return -INF + (search_depth - depth);
    }
    else
    {
      // Stalemate
      return 0;
    }
  }
  
  #ifdef KILLER_MOVES
    killer_moves_key[search_depth-depth] = board->key;
    killer_moves[search_depth-depth] = moves[best_move_num];
  #endif
  
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
    
    // Mate eval test
    if(best_score > INF-MAX_DEPTH)
    {
      flag = LOWERBOUND;
      hashtable_add(hashtable, flag, board->key, depth, INF-MAX_DEPTH, moves[best_move_num]);
    }
    else if(best_score < -INF+MAX_DEPTH)
    {
      flag = UPPERBOUND;
      hashtable_add(hashtable, flag, board->key, depth, -INF+MAX_DEPTH, moves[best_move_num]);
    }
    else
    {
      hashtable_add(hashtable, flag, board->key, depth, best_score, moves[best_move_num]);
    }
    
    //hashtable_add(hashtable, flag, board->key, depth, best_score, moves[best_move_num]);
  #endif
  
  return best_score;
}