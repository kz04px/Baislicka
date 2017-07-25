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

int search_settings_set(s_search_settings settings)
{
  search_settings = settings;
  return 0;
}

int reduction(const int move_num, const int depth, const int in_check, const int move_type)
{
  if(move_num < 4 || depth < 3 || in_check || move_type == CAPTURE || move_type == QUEEN_PROMO || move_type == QUEEN_PROMO_CAPTURE)
  {
    return 0;
  }
  return 1;
}

void *search_root(void *n)
{
  assert(n != NULL);

  board = (*(s_thread_data*)n).board;

  assert(board != NULL);
  int target_depth = MAX_DEPTH;
  char move_string[4096];
  int total_time = 0;
  s_pv pv = {0};

  // Depth limited search
  if(search_settings.depth != 0)
  {
    search_settings.time_max = INT_MAX;
    target_depth = search_settings.depth;
  }
  // Movetime limited search
  else if(search_settings.movetime != 0)
  {
    search_settings.time_max = search_settings.movetime;
  }
  // Regular search
  else
  {
    if(board->turn == WHITE)
    {
      search_settings.time_max = search_settings.wtime/search_settings.movestogo + search_settings.winc;
    }
    else
    {
      search_settings.time_max = search_settings.btime/search_settings.movestogo + search_settings.binc;
    }
  }

  // Info
  s_search_info info;
  info.time_start = clock();
  info.nodes = 0;
  info.hashtable_hits = 0;
  info.seldepth = 0;
  #ifndef NDEBUG
    for(int i = 0; i < MAX_MOVES; ++i)
    {
      info.num_cutoffs[i] = 0;
    }
  #endif

  // Stack
  s_stack stack[MAX_DEPTH];
  for(int i = 0; i < MAX_DEPTH; ++i)
  {
    stack[i].ply = i+1;
    stack[i].null_move = 1;
    stack[i].pv.num_moves = 0;
    //stack[i].killer_move = NO_MOVE;
  }

  #ifdef QUIET_SORT_HISTORY_HEURISTIC
    reset_hh_bf(board);
  #endif

  for(int depth = 1; depth <= target_depth; ++depth)
  {
    int score = 0;

    #ifdef ASPIRATION_WINDOW
      if(depth < 3)
      {
        score = pvSearch(&info, stack, board, -INF, INF, depth);
      }
      else
      {
        #define NUM_BOUNDS 3
        const int bounds[NUM_BOUNDS] = {50, 200, INF};

        for(int n = 0; n < NUM_BOUNDS; ++n)
        {
          score = pvSearch(&info, stack, board, -bounds[n], bounds[n], depth);

          if(-bounds[n] < score && score < bounds[n])
          {
            break;
          }
        }
      }
    #else
      score = pvSearch(&info, stack, board, -INF, INF, depth);
    #endif

    clock_t time_spent = 0;
    time_spent = (double)(clock() - info.time_start) * 1000.0 / CLOCKS_PER_SEC;

    total_time += time_spent;

    // Ignore the result if we ran out of time
    if(total_time >= search_settings.time_max)
    {
      break;
    }

    // Updates
    pv = stack[0].pv;

    int index = 0;
    move_string[0] = '\0';
    for(int n = 0; n < stack[0].pv.num_moves; ++n)
    {
      move_string[index] = ' ';
      index++;
      index += move_to_string(&move_string[index], &stack[0].pv.moves[n]);
    }

    if(score > INF-MAX_DEPTH)
    {
      GUI_Send("info depth %i score mate %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, INF-score, info.nodes, total_time, info.seldepth, move_string);
    }
    else if(score < -INF+MAX_DEPTH)
    {
      GUI_Send("info depth %i score mate %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, -score-INF, info.nodes, total_time, info.seldepth, move_string);
    }
    else
    {
      GUI_Send("info depth %i score cp %i nodes %" PRIu64 " time %i seldepth %i pv%s\n", depth, score, info.nodes, total_time, info.seldepth, move_string);
    }

    // Stop searching if we found a checkmate
    if(score < -INF+MAX_DEPTH || score > INF-MAX_DEPTH)
    {
      break;
    }

    // Try and predict if we'll finish the next search iteration
    if(search_settings.movetime == 0 && search_settings.depth == 0 && total_time + 4*time_spent >= search_settings.time_max)
    {
      break;
    }
  }

  #ifndef NDEBUG
    int total_cutoffs = 0;
    for(int i = 0; i < MAX_MOVES; ++i)
    {
      total_cutoffs += info.num_cutoffs[i];
    }
    for(int i = 0; i < 8; ++i)
    {
      if(total_cutoffs == 0)
      {
        printf("%i) %i -\n", i, info.num_cutoffs[i]);
      }
      else
      {
        printf("%i) %i %.2f%%\n", i, info.num_cutoffs[i], 100.0*info.num_cutoffs[i]/total_cutoffs);
      }
    }
  #endif

  move_to_string(move_string, &pv.moves[0]);
  GUI_Send("bestmove %s\n", move_string);

  return NULL;
}

int qsearch(s_search_info *info, s_stack *stack, s_board *board, int alpha, int beta)
{
  int stand_pat = evaluate(board);

  if(stack->ply > info->seldepth)
  {
    info->seldepth = stack->ply-1;
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

  if(stack->ply >= MAX_DEPTH)
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

  for(int m = 0; m < num_moves; ++m)
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

    score = -qsearch(info, stack+1, board, -beta, -alpha);

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

int pvSearch(s_search_info *info, s_stack *stack, s_board *board, int alpha, int beta, int depth)
{
  assert(info != NULL);
  assert(stack != NULL);
  assert(board != NULL);
  assert(beta >= alpha);
  assert(depth >= 0);

  // Evaluate draws
  if(is_threefold(board) || is_fifty_move_draw(board))
  {
    return CONTEMPT_VALUE;
  }

  // Stop search at maximum depth
  if(stack->ply >= MAX_DEPTH)
  {
    return evaluate(board);
  }

  int in_check = square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn);

  // Check extensions
  if(in_check)
  {
    depth++;
  }

  if(depth == 0)
  {
    #ifdef QUIESCENCE_SEARCH
      return qsearch(info, stack, board, alpha, beta);
    #else
      return evaluate(board);
    #endif
  }

  clock_t time_spent = 0;
  time_spent = (clock() - info->time_start) * 1000 / CLOCKS_PER_SEC;

  // Check time
  if(time_spent >= search_settings.time_max)
  {
    return 0;
  }

  if(info->nodes%524288 == 0)
  {
    // Update GUI on our search
    if(time_spent > 0)
    {
      GUI_Send("info nps %" PRIu64 "\n", 1000*info->nodes/time_spent);
    }
  }

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
  stack->pv.num_moves = 0;

  #ifdef HASHTABLE
    int alpha_original = alpha;

    s_hashtable_entry entry = *hashtable_poll(hashtable, board->key);
    int entry_valid = (board->key == entry.key);
    int entry_eval = eval_from_tt(entry.eval, stack->ply);

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
    if(!pvnode && stack->null_move && depth == 1 && !in_check && !is_endgame(board))
    {
      int static_eval = evaluate(board);
      if(static_eval - 350 >= beta)
      {
        return static_eval - 350;
        //return qsearch(info, stack, board, alpha, beta);
      }

      /*
      int static_eval = evaluate(board);
      if(static_eval + 350 < alpha)
      {
        //return static_eval;
        return qsearch(info, stack, board, alpha, beta);
      }
      */
    }
  #endif

  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);

  #ifdef NULL_MOVE
    if(stack->null_move && !pvnode && depth > 2 && !in_check && !is_endgame(board))
    {
      // Make nullmove
      null_make(board);

      (stack+1)->null_move = 0;
      score = -pvSearch(info, stack+1, board, -beta, -beta+1, depth-1-R);

      // Unmake nullmove
      null_undo(board);

      // Restore old permissions
      restore_irreversible(&permissions, board);

      // Test
      stack->pv.num_moves = 0;

      if(score >= beta)
      {
        return score;
      }
    }
  #endif

  #ifdef NULL_MOVE_REDUCTIONS
    if(stack->null_move && !in_check && depth > 4)
    {
      int new_r = depth > 6 ? 4 : 3;

      // Make nullmove
      null_make(board);

      (stack+1)->null_move = 0;
      score = -pvSearch(info, stack+1, board, -beta, -beta+1, depth-1-new_r);

      // Unmake nullmove
      null_undo(board);

      if(score >= beta)
      {
        depth -= 4;
        if(depth <= 0)
        {
          return qsearch(info, stack, board, alpha, beta);
        }
      }
    }
  #endif

  #ifdef IID
    if(pvnode && gen.hash_move == NO_MOVE && depth > 3)
    {
      score = -pvSearch(info, stack, board, alpha, beta, depth - 3);
      gen.hash_move = stack->pv.moves[0];
    }
  #endif

  #ifdef KILLER_MOVES
    gen.killer_move = stack->killer_move;
  #endif
  #ifdef KILLER_MOVES_2
    if(stack->ply-2 >= 0)
    {
      gen.killer_move_2 = (stack-2)->killer_move;
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

    (stack+1)->null_move = 1;
    best_score = -pvSearch(info, stack+1, board, -beta, -alpha, depth - 1);

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
            stack->killer_move = move;
          }
        #endif

        #ifndef NDEBUG
          info->num_cutoffs[0]++;
        #endif

        #ifdef HASHTABLE
          hashtable_add(hashtable, LOWERBOUND, board->key, depth, eval_to_tt(best_score, stack->ply), move);
        #endif

        return best_score;
      }
      alpha = best_score;
    }

    // Store pv line
    stack->pv.moves[0] = move;
    for(int i = 0; i < (stack+1)->pv.num_moves && i < MAX_DEPTH - 1; ++i)
    {
      stack->pv.moves[i+1] = (stack+1)->pv.moves[i];
    }
    stack->pv.num_moves = (stack+1)->pv.num_moves + 1;

    #ifdef HASHTABLE
      best_move = move;
    #endif

    break;
  }

  (stack+1)->null_move = 1;
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

    #ifdef LMR
      int r = reduction(move_num, depth, in_check, move_get_type(move));
      score = -pvSearch(info, stack+1, board, -alpha-1, -alpha, depth - 1 - r);

      // Re-search
      if(score > alpha && score < beta)
      {
        score = -pvSearch(info, stack+1, board, -beta, -alpha, depth - 1);
        if(score > alpha)
        {
          alpha = score;
        }
      }
    #else
      score = -pvSearch(info, stack+1, board, -alpha-1, -alpha, depth - 1);

      if(score > alpha && score < beta)
      {
        score = -pvSearch(info, stack, board, -beta, -alpha, depth - 1);
        if(score > alpha)
        {
          alpha = score;
        }
      }
    #endif

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
            stack->killer_move = move;
          }
        #endif

        #ifdef HASHTABLE
          hashtable_add(hashtable, LOWERBOUND, board->key, depth, eval_to_tt(best_score, stack->ply), move);
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
      stack->pv.moves[0] = move;
      for(int i = 0; i < (stack+1)->pv.num_moves && i < MAX_DEPTH - 1; ++i)
      {
        stack->pv.moves[i+1] = (stack+1)->pv.moves[i];
      }
      stack->pv.num_moves = (stack+1)->pv.num_moves + 1;

      best_score = score;
    }
  }

  // If we haven't played a move, then there are none
  if(best_score == -INF)
  {
    if(in_check)
    {
      // Checkmate
      return -INF + stack->ply;
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
    hashtable_add(hashtable, flag, board->key, depth, eval_to_tt(best_score, stack->ply), best_move);
  #endif

  return best_score; // fail-hard
}
