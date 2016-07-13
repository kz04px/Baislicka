#include "defs.h"

#define INF 99999

uint64_t nodes;

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
  
  start = clock();
  
  int best_score;
  if(board->turn == WHITE)
  {
    best_score = -INF;
  }
  else
  {
    best_score = INF;
  }
  
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
    
    if(board->turn == WHITE)
    {
      if(score > best_score)
      {
        best_score = score;
        best_move = &moves[m];
      }
    }
    else
    {
      if(score < best_score)
      {
        best_score = score;
        best_move = &moves[m];
      }
    }
    
    move_undo(board, &moves[m]);
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
    printf("Eval: %i\n", best_score);
  }
  else
  {
    if(board->turn == WHITE)
    {
      printf("Eval: #%i\n", depth + ((INF - MAX_DEPTH) - best_score));
    }
    else
    {
      printf("Eval: #-%i\n", depth + (best_score + (INF - MAX_DEPTH)));
    }
  }
  printf("Time: %.3fs\n", time_taken);
  printf("Nodes: %I64u\n", nodes);
  printf("kNPS: %.3f\n", nodes/(1000.0*time_taken));
  printf("\n");
}

int alpha_beta(s_board* board, int alpha, int beta, int depth)
{
  assert(board != NULL);
  assert(depth >= 0);
  
  #ifdef QUIESCENCE_SEARCH
    if(depth == 0) {return quiesce(board, alpha, beta);}
  #else
    if(depth == 0) {return eval(board);}
  #endif
  
  int score;
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves(board, moves, board->turn);
  moves_sort(moves, num_moves);
  
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
    
    if(score >= beta)
    {
      return beta;
    }
    if(score > alpha)
    {
      alpha = score;
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
        return INF - (MAX_DEPTH-depth);
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
        return -INF + (MAX_DEPTH-depth);
      }
      else
      {
        // Stalemate
        return 0;
      }
    }
  }
  
  return alpha;
}