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

int see(int sq, int side, int captured, uint64_t colours[2], uint64_t pieces[6])
{
  int value = 0;
  int smallest_attacker = EMPTY;
  
  uint64_t attackers = 0;
  
  // Pawns
  if((attackers = magic_moves_pawns(1-side, sq) & pieces[PAWNS] & colours[side]))
  {
    smallest_attacker = PAWNS;
  }
  // Knights
  else if((attackers = colours[side] & pieces[KNIGHTS] & magic_moves_knight(sq)))
  {
    smallest_attacker = KNIGHTS;
  }
  // Bishops
  else if((attackers = colours[side] & pieces[BISHOPS] & magic_moves_bishop(colours[WHITE]|colours[BLACK], sq)))
  {
    smallest_attacker = BISHOPS;
  }
  // Rooks
  else if((attackers = colours[side] & pieces[ROOKS] & magic_moves_rook(colours[WHITE]|colours[BLACK], sq)))
  {
    smallest_attacker = ROOKS;
  }
  // Queens
  else if((attackers = colours[side] & pieces[QUEENS] & (magic_moves_bishop(colours[WHITE]|colours[BLACK], sq) | magic_moves_rook(colours[WHITE]|colours[BLACK], sq))))
  {
    smallest_attacker = QUEENS;
  }
  // Kings
  else if((attackers = colours[side] & pieces[KINGS] & magic_moves_king(sq)))
  {
    smallest_attacker = KINGS;
  }
  else
  {
    // skip if the square isn't attacked anymore by this side
    return value;
  }
  
  int from_sq = __builtin_ctzll(attackers);
  uint64_t from_bb = (uint64_t)1<<from_sq;
  
  // Make move
  pieces[smallest_attacker] ^= from_bb;
  colours[side] ^= from_bb;
  
  value = piece_value(captured) - see(sq, 1-side, smallest_attacker, colours, pieces);
  
  if(value < 0)
  {
    value = 0;
  }
  
  // Undo move
  pieces[smallest_attacker] ^= from_bb;
  colours[side] ^= from_bb;
  
  return value;
}

int see_capture(s_board *board, s_move move)
{
  uint64_t from_bb = (uint64_t)1<<move.from;
  
  // Make move
  board->pieces[move.piece_type] ^= from_bb;
  board->colour[board->turn] ^= from_bb;
  
  int value = piece_value(move.taken) - see(move.to, 1-board->turn, move.piece_type, board->colour, board->pieces);
  
  // Undo move
  board->pieces[move.piece_type] ^= from_bb;
  board->colour[board->turn] ^= from_bb;
  
  return value;
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
  
  #ifdef DELTA_PRUNING
    const int safety = 900; // The value of a queen
    
    if(stand_pat < alpha - safety)
    {
      return alpha;
    }
  #endif
  
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
    moves_sort_see(board, moves, num_moves);
  #endif
  int score;
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    int val = see_capture(board, moves[m]);
    if(val < -50)
    {
      break;
    }
    
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
      #ifndef NDEBUG
        info->num_cutoffs[m]++;
      #endif
      
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
  int total_time = 0;
  
  int i;
  for(i = 1; i <= target_depth && i < MAX_DEPTH; ++i)
  {
    #ifdef ASPIRATION_WINDOW
      if(i < 3)
      {
        results = search(board, i, -INF, INF);
        total_time += results.time_taken;
      }
      else
      {
        results = search(board, i, -50, 50);
        total_time += results.time_taken;
        
        int val = results.evals[results.best_move_num];
        if(results.out_of_time == 1) {break;}
        
        if(val <= -50 || val >= 50)
        {
          results = search(board, i, -INF, INF);
          total_time += results.time_taken;
        }
      }
    #else
      results = search(board, i, -INF, INF);
      total_time += results.time_taken;
    #endif
    
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
      GUI_Send("info depth %i score mate %i nodes %"PRIdPTR" time %i seldepth %i pv%s\n", i, INF-bestmove_eval, results.nodes, total_time, results.seldepth, move_string);
    }
    else if(bestmove_eval < -INF+MAX_DEPTH)
    {
      GUI_Send("info depth %i score mate %i nodes %"PRIdPTR" time %i seldepth %i pv%s\n", i, -bestmove_eval-INF, results.nodes, total_time, results.seldepth, move_string);
    }
    else
    {
      GUI_Send("info depth %i score cp %i nodes %"PRIdPTR" time %i seldepth %i pv%s\n", i, bestmove_eval, results.nodes, total_time, results.seldepth, move_string);
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
    
    if(4*total_time < search_settings.time_max)
    //if(4*results.time_taken < search_settings.time_max)
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
  s_pv pv_local = {0};
  
  results.num_moves = find_moves_captures(board, results.moves, board->turn);
  #ifdef SORT_MOVES
    moves_sort_see(board, results.moves, results.num_moves);
  #endif
  results.num_moves += find_moves_quiet(board, &results.moves[results.num_moves], board->turn);
  
  s_search_info *info = malloc(1*sizeof(s_search_info));
  info->ply = 0;
  info->time_start = clock();
  info->nodes = 0;
  info->hashtable_hits = 0;
  info->seldepth = 0;
  #ifndef NDEBUG
    int i;
    for(i = 0; i < MAX_MOVES; ++i)
    {
      info->num_cutoffs[i] = 0;
    }
  #endif
  
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
      clock_t time_spent = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;
      if(time_spent < search_settings.time_max)
      {
        info->ply++;
        
        #ifdef ALPHA_BETA
          score = -alpha_beta(board, info, alpha, beta, depth-1, 1, &pv_local);
        #elif defined(PVS)
          score = -pvSearch(board, info, alpha, beta, depth-1, 1, &pv_local);
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
  
  #ifndef NDEBUG
    int total_cutoffs = 0;
    for(i = 0; i < MAX_MOVES; ++i)
    {
      total_cutoffs += info->num_cutoffs[i];
    }
    for(i = 0; i < 8; ++i)
    {
      printf("%i) %i %.2f%%\n", i, info->num_cutoffs[i], 100.0*info->num_cutoffs[i]/total_cutoffs);
    }
  #endif
  results.nodes = info->nodes;
  results.seldepth = info->seldepth;
  results.time_taken = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;
  
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
  
  int in_check = square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn);
  
  if(in_check)
  {
    depth++;
  }
  
  if(depth <= 0)
  {
    #ifdef QUIESCENCE_SEARCH
      return qsearch(board, info, alpha, beta);
    #else
      return eval(board);
    #endif
  }
  
  s_pv pv_local = {0};
  pv_local.num_moves = 0;
  s_move_generator gen = {0};
  gen.stage = 0;
  gen.hash_move = NO_MOVE;
  gen.killer_move = NO_MOVE;
  int score;
  pv->num_moves = 0;
  
  #ifdef HASHTABLE
    int flag = -1;
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
        if(entry.flags == LOWERBOUND)
        {
          if(entry_eval >= beta)
          {
            return entry_eval;
          }
        }
        else if(entry.flags == UPPERBOUND)
        {
          if(entry_eval <= alpha)
          {
            return entry_eval;
          }
        }
        
        /*
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
        */
      }
    }
  #endif
  
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
    gen.killer_move = killer_moves[info->ply];
  #endif
  
  int best_score = -INF;
  
  s_move move;
  s_move best_move;
  int move_num = 0;
  
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
    move_num++;
    
    clock_t time_spent = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;
    if(time_spent < search_settings.time_max)
    {
      info->ply++;
      
      #ifdef LMR
        if(move_num < 4 || depth < 3 || in_check || move.type == CAPTURE || move.type == PROMOTE)
        {
          score = -alpha_beta(board, info, -beta, -alpha, depth-1, 1, &pv_local);
        }
        else
        {
          score = -alpha_beta(board, info, -beta, -alpha, depth-2, 1, &pv_local);
        }
      #else
        score = -alpha_beta(board, info, -beta, -alpha, depth-1, 1, &pv_local);
      #endif
      
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
        
        #ifdef HASHTABLE
          flag = LOWERBOUND;
        #endif
        
        #ifndef NDEBUG
          info->num_cutoffs[move_num-1]++;
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
    if(flag == -1)
    {
      if(alpha == alpha_original)
      {
        flag = UPPERBOUND;
      }
      else
      {
        flag = EXACT;
      }
    }
    
    /*
    int flag = EXACT;
    if(best_score < alpha_original)
    {
      flag = UPPERBOUND;
    }
    */
    
    hashtable_add(hashtable, flag, board->key, depth, eval_to_tt(best_score, info->ply), best_move);
  #endif
  
  return best_score;
}

int pvSearch(s_board *board, s_search_info *info, int alpha, int beta, int depth, int null_move, s_pv *pv)
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
  
  int in_check = square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn);
  
  if(in_check)
  {
    depth++;
  }
  
  if(depth == 0)
  {
    #ifdef QUIESCENCE_SEARCH
      return qsearch(board, info, alpha, beta);
    #else
      return eval(board);
    #endif
  }
  
  // Check time
  clock_t time_spent = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;
  if(time_spent >= search_settings.time_max)
  {
    return 0;
  }
  
  s_pv pv_local = {0};
  pv_local.num_moves = 0;
  s_move_generator gen = {0};
  gen.stage = 0;
  gen.hash_move = NO_MOVE;
  gen.killer_move = NO_MOVE;
  int score = -INF;
  int bSearchPv = 1;
  pv->num_moves = 0;
  
  #ifdef HASHTABLE
    int flag = -1;
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
        if(entry.flags == LOWERBOUND)
        {
          if(entry_eval >= beta)
          {
            return entry_eval;
          }
        }
        else if(entry.flags == UPPERBOUND)
        {
          if(entry_eval <= alpha)
          {
            return entry_eval;
          }
        }
      }
    }
  #endif
  
  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);
  
  #ifdef NULL_MOVE
    if(null_move && alpha != beta && !is_endgame(board) && depth > 2 && !in_check)
    {
      // Make nullmove
      null_make(board);
      
      info->ply++;
      score = -pvSearch(board, info, -beta, -beta+1, depth-1-R, 0, &pv_local);
      info->ply--;
      
      // Unmake nullmove
      null_undo(board);
      
      // Restore old permissions
      restore_irreversible(&permissions, board);
      
      // Test
      pv_local.num_moves = 0;
      
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
    
    info->nodes++;
    info->ply++;
    
    if(bSearchPv)
    {
      score = -pvSearch(board, info, -beta, -alpha, depth - 1, 1, &pv_local);
    }
    else
    {
      #ifdef LMR
        if(gen.move_num < 4 || depth < 3 || in_check || move.type == CAPTURE || move.type == PROMOTE)
        {
          score = -pvSearch(board, info, -alpha-1, -alpha, depth-1, 1, &pv_local);
        }
        else
        {
          score = -pvSearch(board, info, -alpha-1, -alpha, depth-2, 1, &pv_local);
        }
      #else
        score = -pvSearch(board, info, -alpha-1, -alpha, depth-1, 1, &pv_local);
      #endif
      
      if(alpha < score && score < beta)
      {
        score = -pvSearch(board, info, -beta, -alpha, depth - 1, 1, &pv_local);
      }
    }
    info->ply--;
    
    // Restore old permissions
    restore_irreversible(&permissions, board);
    
    move_undo(board, &move);
    
    if(score > best_score)
    {
      best_score = score;
      bSearchPv = 0;
      
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
        
        #ifdef HASHTABLE
          flag = LOWERBOUND;
        #endif
        
        #ifndef NDEBUG
          info->num_cutoffs[gen.move_num-1]++;
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
    if(flag == -1)
    {
      if(best_score == alpha_original)
      {
        flag = UPPERBOUND;
      }
      else
      {
        flag = EXACT;
      }
    }
    
    hashtable_add(hashtable, flag, board->key, depth, eval_to_tt(best_score, info->ply), best_move);
  #endif
  
  return best_score; // fail-hard
}
