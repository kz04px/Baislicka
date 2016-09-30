#include "defs.h"

int find_moves_wN(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->combined[KNIGHTS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_knight(u64_to_sq(from)) & (~board->colour[WHITE]);
    
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, KNIGHTS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wB(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->combined[BISHOPS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & (~board->colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, BISHOPS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wR(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->combined[ROOKS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & (~board->colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, ROOKS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wQ(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Queen (diagonal)
  copy = board->combined[QUEENS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & (~board->colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, QUEENS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (horizontal and vertical)
  copy = board->combined[QUEENS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & (~board->colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, QUEENS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wB_wQ(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Bishop
  copy = board->combined[BISHOPS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & (~board->colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, BISHOPS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (diagonal)
  copy = board->combined[QUEENS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & (~board->colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, QUEENS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wR_wQ(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Rook
  copy = board->combined[ROOKS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & (~board->colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, ROOKS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (horizontal and vertical)
  copy = board->combined[QUEENS] & board->colour[WHITE];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & (~board->colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, QUEENS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}
