#include "defs.h"

int find_moves(s_board* board, s_move* move_list, int colour)
{
  assert(board != NULL);
  assert(move_list != NULL);
  assert(colour == WHITE || colour == BLACK);
  
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

int find_moves_white(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  assert(board->pieces_colour[WHITE]);
  
  int num_moves = 0;
  num_moves += find_moves_wB_wQ(board, &move_list[num_moves]);
  num_moves += find_moves_wR_wQ(board, &move_list[num_moves]);
  num_moves += find_moves_wN(board, &move_list[num_moves]);
  num_moves += find_moves_wP(board, &move_list[num_moves]);
  num_moves += find_moves_wK(board, &move_list[num_moves]);
  
  //num_moves += find_moves_wB(board, &move_list[num_moves]);
  //num_moves += find_moves_wR(board, &move_list[num_moves]);
  //num_moves += find_moves_wQ(board, &move_list[num_moves]);
  
  assert(num_moves < MAX_MOVES);
  assert(num_moves >= 0);
  return num_moves;
}

int find_moves_black(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  assert(board->pieces_colour[BLACK]);
  
  int num_moves = 0;
  num_moves += find_moves_bB_bQ(board, &move_list[num_moves]);
  num_moves += find_moves_bR_bQ(board, &move_list[num_moves]);
  num_moves += find_moves_bN(board, &move_list[num_moves]);
  num_moves += find_moves_bP(board, &move_list[num_moves]);
  num_moves += find_moves_bK(board, &move_list[num_moves]);
  
  //num_moves += find_moves_bB(board, &move_list[num_moves]);
  //num_moves += find_moves_bR(board, &move_list[num_moves]);
  //num_moves += find_moves_bQ(board, &move_list[num_moves]);
  
  assert(num_moves < MAX_MOVES);
  assert(num_moves >= 0);
  return num_moves;
}