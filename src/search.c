#include "defs.h"

#define INF 99999

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
    
    board->turn = 1-(board->turn);
      
    int score = alpha_beta(board, -INF, INF, depth-1);
    
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
  printf("Depth: %i\n", depth);
  print_move(*best_move);
  printf("Eval: %i\n", best_score);
  printf("Time: %.3fs\n", time_taken);
  printf("\n");
}

int alpha_beta(s_board* board, int alpha, int beta, int depth)
{
  assert(board != NULL);
  assert(depth >= 0);
  
  if(depth == 0) {return eval(board);}
  //if(depth == 0) {return quiesce(board, alpha, beta);}
  
  int score;
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves(board, moves, board->turn);
  
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
    
    board->turn = 1-(board->turn);
    
    score = -alpha_beta(board, -beta, -alpha, depth-1);
    
    board->turn = 1-(board->turn);
    
    if(score >= beta)
    {
      move_undo(board, &moves[m]);
      return beta;
    }
    if(score > alpha)
    {
      alpha = score;
    }
    
    move_undo(board, &moves[m]);
  }
  
  return alpha;
}