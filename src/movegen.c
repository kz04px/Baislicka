#include "defs.h"

int find_moves(s_board* board, s_move* move_list, int colour)
{
  ASSERT(board != NULL);
  ASSERT(move_list != NULL);
  ASSERT(colour == WHITE || colour == BLACK);
  
  int num_moves = 0;
  
  if(colour == WHITE)
  {
    num_moves += find_moves_white(board, move_list);
  }
  else
  {
    num_moves += find_moves_black(board, move_list);
  }
  
  return num_moves;
}