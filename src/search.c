#include "defs.h"
#include "search.h"
#include "attack.h"
#include "movegen.h"
#include "bitboards.h"
#include "move.h"
#include "eval.h"
#include "hashtable.h"
#include <assert.h>

#define R 2

s_board *board = NULL;
s_search_settings search_settings;

#if defined(KILLER_MOVES) || defined(KILLER_MOVES_2)
  s_move killer_moves[MAX_DEPTH];
#endif

int killers_clear()
{
  #if defined(KILLER_MOVES) || defined(KILLER_MOVES_2)
    int i;
    for(i = 0; i < MAX_DEPTH; ++i)
    {
      killer_moves[i] = NO_MOVE;
    }
  #endif
  return 0;
}

int store_irreversible(s_irreversible *info, s_board *board)
{
  assert(board);
  assert(info);

  #ifdef HASHTABLE
    info->key         = board->key;
  #endif
  info->num_halfmoves = board->num_halfmoves;
  info->ep            = board->ep;
  info->castling      = board->castling;

  return 0;
}

int restore_irreversible(s_irreversible *info, s_board *board)
{
  assert(board);
  assert(info);

  #ifdef HASHTABLE
    board->key         = info->key;
  #endif
  board->num_halfmoves = info->num_halfmoves;
  board->ep            = info->ep;
  board->castling      = info->castling;

  return 0;
}

int qsearch(s_board *board, s_search_info *info, int alpha, int beta)
{
  int stand_pat = evaluate(board);

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

    if(stand_pat < alpha - safety && !is_endgame(board))
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

  int score;
  s_pv pv = {0};

  char move_string[4096];
  int total_time = 0;

  s_search_info *info = malloc(1*sizeof(s_search_info));
  info->ply = 0;
  info->time_start = clock();
  info->nodes = 0;
  info->hashtable_hits = 0;
  info->seldepth = 0;
  #ifndef NDEBUG
    for(int i = 0; i < MAX_MOVES; ++i)
    {
      info->num_cutoffs[i] = 0;
    }
  #endif

  #ifdef QUIET_SORT_HISTORY_HEURISTIC
    reset_hh_bf(board);
  #endif

  for(int depth = 1; depth < MAX_DEPTH; ++depth)
  {
    int depth_score = 0;
    s_pv depth_pv = {0};

    #ifdef ASPIRATION_WINDOW
      if(depth < 3)
      {
        #ifdef ALPHA_BETA
          depth_score = alpha_beta(board, info, -INF, INF, depth, 1, &depth_pv);
        #elif defined(PVS)
          depth_score = pvSearch(board, info, -INF, INF, depth, 1, &depth_pv);
        #endif
      }
      else
      {
        #define NUM_BOUNDS 3
        const int bounds[NUM_BOUNDS] = {50, 200, INF};

        for(int n = 0; n < NUM_BOUNDS; ++n)
        {
          #ifdef ALPHA_BETA
            depth_score = alpha_beta(board, info, -bounds[n], bounds[n], depth, 1, &depth_pv);
          #elif defined(PVS)
            depth_score = pvSearch(board, info, -bounds[n], bounds[n], depth, 1, &depth_pv);
          #endif

          if(-bounds[n] < depth_score && depth_score < bounds[n])
          {
            break;
          }
        }
      }
    #else
      #ifdef ALPHA_BETA
        depth_score = alpha_beta(board, info, -INF, INF, depth, 1, &depth_pv);
      #elif defined(PVS)
        depth_score = pvSearch(board, info, -INF, INF, depth, 1, &depth_pv);
      #endif
    #endif

    // Check time
    clock_t time_spent = 0;
    time_spent = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;
    if(time_spent >= search_settings.time_max)
    {
      break;
    }

    // Updates
    score = depth_score;
    pv = depth_pv;
    total_time += time_spent;

    int index = 0;
    move_string[0] = '\0';
    for(int n = 0; n < depth_pv.num_moves; ++n)
    {
      move_string[index] = ' ';
      index++;
      index += move_to_string(&move_string[index], &depth_pv.moves[n]);
    }

    if(score > INF-MAX_DEPTH)
    {
      GUI_Send("info depth %i score mate %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, INF-score, info->nodes, total_time, info->seldepth, move_string);
    }
    else if(score < -INF+MAX_DEPTH)
    {
      GUI_Send("info depth %i score mate %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, -score-INF, info->nodes, total_time, info->seldepth, move_string);
    }
    else
    {
      GUI_Send("info depth %i score cp %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, score, info->nodes, total_time, info->seldepth, move_string);
    }

    if(score < -INF+MAX_DEPTH || score > INF-MAX_DEPTH)
    {
      break;
    }

    if(4*total_time >= search_settings.time_max)
    {
      break;
    }
  }

  #ifndef NDEBUG
    int total_cutoffs = 0;
    for(int i = 0; i < MAX_MOVES; ++i)
    {
      total_cutoffs += info->num_cutoffs[i];
    }
    for(int i = 0; i < 8; ++i)
    {
      if(total_cutoffs == 0)
      {
        printf("%i) %i -\n", i, info->num_cutoffs[i]);
      }
      else
      {
        printf("%i) %i %.2f%%\n", i, info->num_cutoffs[i], 100.0*info->num_cutoffs[i]/total_cutoffs);
      }
    }
  #endif

  move_to_string(move_string, &pv.moves[0]);
  GUI_Send("bestmove %s\n", move_string);

  return NULL;
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
    return evaluate(board);
  }

  clock_t time_spent = 0;
  if(info->nodes%4096 == 0)
  {
    // Check time
    time_spent = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;
    if(time_spent >= search_settings.time_max)
    {
      return 0;
    }
  }

  if(info->nodes%524288 == 0)
  {
    // Update GUI on our search
    if(time_spent > 0)
    {
      GUI_Send("info nps %" PRIu64 "\n", 1000*info->nodes/time_spent);
    }
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
      return evaluate(board);
    #endif
  }

  s_pv pv_local = {0};
  pv_local.num_moves = 0;
  s_move_generator gen = {0};
  gen.stage = 0;
  #ifdef HASHTABLE
    gen.hash_move = NO_MOVE;
  #endif
  #ifdef KILLER_MOVES
    gen.killer_move = NO_MOVE;
  #endif
  #ifdef KILLER_MOVES_2
    gen.killer_move_2 = NO_MOVE;
  #endif
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
      }
    }
  #endif

  #ifdef FUTILITY_PRUNING
    if(depth == 1 && !in_check && !is_endgame(board))
    {
      int static_eval = evaluate(board);
      if(static_eval + 350 < alpha)
      {
        //return static_eval;
        return qsearch(board, info, alpha, beta);
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

  #ifdef NULL_MOVE_REDUCTIONS
    if(null_move && !in_check)
    {
      int new_r = depth > 6 ? 4 : 3;

      // Make nullmove
      null_make(board);

      info->ply++;
      score = -alpha_beta(board, info, -beta, -beta+1, depth-1-new_r, 0, &pv_local);
      info->ply--;

      // Unmake nullmove
      null_undo(board);

      if(score >= beta)
      {
        depth -= 4;
        if(depth <= 0)
        {
          return qsearch(board, info, alpha, beta);
        }
      }
    }
  #endif

  #ifdef KILLER_MOVES
    gen.killer_move = killer_moves[info->ply];
  #endif
  #ifdef KILLER_MOVES_2
    if(info->ply-2 >= 0)
    {
      gen.killer_move_2 = killer_moves[info->ply-2];
    }
  #endif

  int best_score = -INF;

  s_move move = NO_MOVE;
  s_move best_move = NO_MOVE;
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
    info->ply++;

    #ifdef LMR
      if(move_num < 4 || depth < 3 || in_check || move_get_type(move) == CAPTURE || move_get_type(move) == QUEEN_PROMO || move_get_type(move) == QUEEN_PROMO_CAPTURE)
      // || square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], board->turn))
      {
        score = -alpha_beta(board, info, -beta, -alpha, depth-1, 1, &pv_local);
      }
      else
      {
        score = -alpha_beta(board, info, -beta, -alpha, depth-2, 1, &pv_local);

        // Re-search if failed high
        /*
        if(score > alpha)
        {
          score = -alpha_beta(board, info, -beta, -alpha, depth-1, 1, &pv_local);
        }
        */
      }
    #else
      score = -alpha_beta(board, info, -beta, -alpha, depth-1, 1, &pv_local);
    #endif

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
          if(move_get_type(move) == QUIET)
          {
            killer_moves[info->ply] = move;
          }
        #endif

        #ifdef QUIET_SORT_HISTORY_HEURISTIC
          if(!is_capture_move(move))
          {
            board->hh_score[move_get_from(move)][move_get_to(move)] += depth*depth;
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

      #ifdef QUIET_SORT_HISTORY_HEURISTIC
        if(!is_capture_move(move))
        {
          board->bf_score[move_get_from(move)][move_get_to(move)] += 1;
        }
      #endif
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
    return evaluate(board);
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
      return evaluate(board);
    #endif
  }

  clock_t time_spent = 0;
  if(info->nodes%4096 == 0)
  {
    // Check time
    time_spent = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;
    if(time_spent >= search_settings.time_max)
    {
      return 0;
    }
  }

  if(info->nodes%524288 == 0)
  {
    // Update GUI on our search
    if(time_spent > 0)
    {
      GUI_Send("info nps %" PRIu64 "\n", 1000*info->nodes/time_spent);
    }
  }

  s_pv pv_local = {0};
  pv_local.num_moves = 0;
  s_move_generator gen = {0};
  gen.stage = 0;
  #ifdef HASHTABLE
    gen.hash_move = NO_MOVE;
  #endif
  #ifdef KILLER_MOVES
    gen.killer_move = NO_MOVE;
  #endif
  #ifdef KILLER_MOVES_2
    gen.killer_move_2 = NO_MOVE;
  #endif
  int score = -INF;
  pv->num_moves = 0;

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

  int pvnode = (beta - alpha > 1);

  #ifdef FUTILITY_PRUNING
    if(!pvnode && null_move && depth == 1 && !in_check && !is_endgame(board))
    {
      int static_eval = evaluate(board);
      if(static_eval - 350 >= beta)
      {
        return static_eval - 350;
        //return qsearch(board, info, alpha, beta);
      }

      /*
      int static_eval = evaluate(board);
      if(static_eval + 350 < alpha)
      {
        //return static_eval;
        return qsearch(board, info, alpha, beta);
      }
      */
    }
  #endif

  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);

  #ifdef NULL_MOVE
    if(null_move && !pvnode && depth > 2 && !in_check && !is_endgame(board))
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

  #ifdef NULL_MOVE_REDUCTIONS
    if(null_move && !in_check && depth > 4)
    {
      int new_r = depth > 6 ? 4 : 3;

      // Make nullmove
      null_make(board);

      info->ply++;
      score = -pvSearch(board, info, -beta, -beta+1, depth-1-new_r, 0, &pv_local);
      info->ply--;

      // Unmake nullmove
      null_undo(board);

      if(score >= beta)
      {
        depth -= 4;
        if(depth <= 0)
        {
          return qsearch(board, info, alpha, beta);
        }
      }
    }
  #endif

  #ifdef IID
    if(pvnode && gen.hash_move == NO_MOVE && depth > 3)
    {
      score = -pvSearch(board, info, alpha, beta, depth - 3, 1, &pv_local);
      gen.hash_move = pv_local.moves[0];
    }
  #endif

  #ifdef KILLER_MOVES
    gen.killer_move = killer_moves[info->ply];
  #endif
  #ifdef KILLER_MOVES_2
    if(info->ply-2 >= 0)
    {
      gen.killer_move_2 = killer_moves[info->ply-2];
    }
  #endif

  s_move move = NO_MOVE;
  s_move best_move = NO_MOVE;
  int best_score = -INF;
  int move_num = 0;

  // Try the first move
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

    move_num++;
    info->nodes++;
    info->ply++;
    best_score = -pvSearch(board, info, -beta, -alpha, depth - 1, 1, &pv_local);
    info->ply--;

    // Restore old permissions
    restore_irreversible(&permissions, board);
    move_undo(board, &move);

    if(best_score > alpha)
    {
      if(best_score >= beta)
      {
        #ifdef KILLER_MOVES
          if(move_get_type(move) == QUIET)
          {
            killer_moves[info->ply] = move;
          }
        #endif

        #ifndef NDEBUG
          info->num_cutoffs[0]++;
        #endif

        #ifdef HASHTABLE
          hashtable_add(hashtable, LOWERBOUND, board->key, depth, eval_to_tt(best_score, info->ply), move);
        #endif

        return best_score;
      }
      alpha = best_score;
    }

    // Store pv line
    pv->moves[0] = move;
    int i;
    for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH - 1; ++i)
    {
      pv->moves[i+1] = pv_local.moves[i];
    }
    pv->num_moves = pv_local.num_moves + 1;

    #ifdef HASHTABLE
      best_move = move;
    #endif

    break;
  }

  // Try the rest of the moves
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

    move_num++;
    info->nodes++;
    info->ply++;

    #ifdef LMR
      if(move_num < 4 || depth < 3 || in_check || move_get_type(move) == CAPTURE || move_get_type(move) == QUEEN_PROMO || move_get_type(move) == QUEEN_PROMO_CAPTURE)
      // || square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], board->turn))
      {
        score = -pvSearch(board, info, -alpha-1, -alpha, depth - 1, 1, &pv_local);
      }
      else
      {
        score = -pvSearch(board, info, -alpha-1, -alpha, depth - 2, 1, &pv_local);
      }

      // Re-search
      if(score > alpha && score < beta)
      {
        score = -pvSearch(board, info, -beta, -alpha, depth - 1, 1, &pv_local);
        if(score > alpha)
        {
          alpha = score;
        }
      }
    #else
      score = -pvSearch(board, info, -alpha-1, -alpha, depth - 1, 1, &pv_local);

      if(score > alpha && score < beta)
      {
        score = -pvSearch(board, info, -beta, -alpha, depth - 1, 1, &pv_local);
        if(score > alpha)
        {
          alpha = score;
        }
      }
    #endif
    info->ply--;

    // Restore old permissions
    restore_irreversible(&permissions, board);
    move_undo(board, &move);

    if(score > best_score)
    {
      if(score >= beta)
      {
        #ifdef KILLER_MOVES
          if(move_get_type(move) == QUIET)
          {
            killer_moves[info->ply] = move;
          }
        #endif

        #ifdef HASHTABLE
          hashtable_add(hashtable, LOWERBOUND, board->key, depth, eval_to_tt(best_score, info->ply), move);
        #endif

        #ifndef NDEBUG
          info->num_cutoffs[move_num-1]++;
        #endif

        return score;
      }

      #ifdef HASHTABLE
        best_move = move;
      #endif

      // Store pv line
      pv->moves[0] = move;
      int i;
      for(i = 0; i < pv_local.num_moves && i < MAX_DEPTH - 1; ++i)
      {
        pv->moves[i+1] = pv_local.moves[i];
      }
      pv->num_moves = pv_local.num_moves + 1;

      best_score = score;
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
    int flag;
    if(best_score == alpha_original)
    {
      flag = UPPERBOUND;
    }
    else
    {
      flag = EXACT;
    }
    hashtable_add(hashtable, flag, board->key, depth, eval_to_tt(best_score, info->ply), best_move);
  #endif

  return best_score; // fail-hard
}
