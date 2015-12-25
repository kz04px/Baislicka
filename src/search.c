#include "defs.h"

#define INF 99999

void search(s_board* board, int depth)
{
  ASSERT(board != NULL);
  ASSERT(depth > 0);
  
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
  s_move move_list[MAX_MOVES];
  int num_moves = find_moves(board, move_list, board->turn);
  int m;
  for(m = 0; m < num_moves; ++m)
  {    
    move_make(board, &move_list[m]);
    
    if(board->turn == WHITE)
    {
      if(calculate_attacked_black(board, board->pieces[wK]))
      {
        move_undo(board, &move_list[m]);
        continue;
      }
    }
    else
    {
      if(calculate_attacked_white(board, board->pieces[bK]))
      {
        move_undo(board, &move_list[m]);
        continue;
      }
    }
    
    board->turn = 1-(board->turn);
    
    #ifdef MINIMAX
      int score = minimax(board, depth-1);
    #elif ALPHA_BETA
      //int score = alpha_beta();
    #else
      // insert default search method
    #endif
    
    board->turn = 1-(board->turn);
    
    if(board->turn == WHITE)
    {
      if(score > best_score)
      {
        best_score = score;
        best_move = &move_list[m];
      }
    }
    else
    {
      if(score < best_score)
      {
        best_score = score;
        best_move = &move_list[m];
      }
    }
    
    move_undo(board, &move_list[m]);
  }
  
  time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
  
  // Results
  printf("Depth: %i\n", depth);
  print_move(*best_move);
  printf("Eval: %i\n", best_score);
  printf("Time: %.3fs\n", time_taken);
  printf("\n");
}

int minimax(s_board* board, int depth)
{
  ASSERT(board != NULL);
  ASSERT(depth >= 0);
  
  if(depth == 0)
  {
    return eval(board);
  }
  
  int best_score;
  if(board->turn == WHITE)
  {
    best_score = -INF;
  }
  else
  {
    best_score = INF;
  }
  
  s_move move_list[MAX_MOVES];
  int num_moves = find_moves(board, move_list, board->turn);
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {    
    move_make(board, &move_list[m]);
    
    if(board->turn == WHITE)
    {
      if(calculate_attacked_black(board, board->pieces[wK]))
      {
        move_undo(board, &move_list[m]);
        continue;
      }
    }
    else
    {
      if(calculate_attacked_white(board, board->pieces[bK]))
      {
        move_undo(board, &move_list[m]);
        continue;
      }
    }
    
    board->turn = 1-(board->turn);
    
    int score = minimax(board, depth-1);
    
    board->turn = 1-(board->turn);
    
    if(board->turn == WHITE)
    {
      if(score > best_score)
      {
        best_score = score;
      }
    }
    else
    {
      if(score < best_score)
      {
        best_score = score;
      }
    }
    
    move_undo(board, &move_list[m]);
  }
  
  return best_score;
}