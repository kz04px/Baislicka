#include "defs.h"

int find_moves_wP(s_board* board, s_move* move_list)
{
  int num_moves = 0;
  uint64_t moves;
  uint64_t to;
  
  // ep
  if(board->ep)
  {
    // Down & Left
    if((board->ep>>7) & board->pieces[wP] & (~U64_COL_H))
    {
      move_list[num_moves] = move_add(board, board->ep>>7, board->ep, EP, wP);
      num_moves++;
    }
    
    // Down & Right
    if((board->ep>>9) & board->pieces[wP] & (~U64_COL_A))
    {
      move_list[num_moves] = move_add(board, board->ep>>9, board->ep, EP, wP);
      num_moves++;
    }
  }
  
  // Up 1
  moves = (board->pieces[wP]<<8) & (~board->pieces_all);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to>>8, to);
    
    // Up 2
    if((to&U64_ROW_3) && ((to<<8)&(~board->pieces_all)))
    {
      move_list[num_moves] = move_add(board, to>>8, to<<8, DOUBLE_PAWN, wP);
      num_moves++;
    }
    
    moves = moves^to;
  }
  // Up 1 Left 1
  moves = (board->pieces[wP]<<9) & (board->pieces_colour[BLACK]) & (~U64_COL_H);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to>>9, to);
    moves = moves^to;
  }
  // Up 1 Right 1
  moves = (board->pieces[wP]<<7) & (board->pieces_colour[BLACK]) & (~U64_COL_A);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to>>7, to);
    moves = moves^to;
  }
  
  return num_moves;
}

int find_moves_wN(s_board* board, s_move* move_list)
{
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->pieces[wN];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_knight(u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wN);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wB(s_board* board, s_move* move_list)
{
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->pieces[wB];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wB);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wR(s_board* board, s_move* move_list)
{
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->pieces[wR];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wR);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wQ(s_board* board, s_move* move_list)
{
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Queen (diagonal)
  copy = board->pieces[wQ];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wQ);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (horizontal and vertical)
  copy = board->pieces[wQ];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wQ);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wK(s_board* board, s_move* move_list)
{
  int num_moves = 0;
  uint64_t moves;
  uint64_t to;
  
  // castling
  if(board->castling[wKSC] &&
     !(board->pieces_all&U64_F1) &&
     !(board->pieces_all&U64_G1) &&
     calculate_attacked_black(board, U64_E1) != 0 &&
     calculate_attacked_black(board, U64_F1) != 0 &&
     calculate_attacked_black(board, U64_G1) != 0)
  {
    move_list[num_moves] = move_add(board, U64_E1, U64_G1, wKSC, wK);
    num_moves++;
  }
  if(board->castling[wQSC] &&
     !(board->pieces_all&U64_B1) &&
     !(board->pieces_all&U64_C1) &&
     !(board->pieces_all&U64_D1) &&
     calculate_attacked_black(board, U64_E1) != 0 &&
     calculate_attacked_black(board, U64_D1) != 0 &&
     calculate_attacked_black(board, U64_C1) != 0)
  {
    move_list[num_moves] = move_add(board, U64_E1, U64_C1, wQSC, wK);
    num_moves++;
  }
  
  moves  = (board->pieces[wK]<<8); // Up 1
  moves |= (board->pieces[wK]>>8); // Down 1
  moves |= (board->pieces[wK]<<1) & (~U64_COL_H); // Left 1
  moves |= (board->pieces[wK]>>1) & (~U64_COL_A); // Right 1
  moves |= (board->pieces[wK]<<7) & (~U64_COL_A); // Up 1 Right 1
  moves |= (board->pieces[wK]<<9) & (~U64_COL_H); // Up 1 Left 1
  moves |= (board->pieces[wK]>>7) & (~U64_COL_H); // Down 1 Left 1
  moves |= (board->pieces[wK]>>9) & (~U64_COL_A); // Down 1 Right 1
  moves &= ~board->pieces_colour[WHITE];
  while(moves)
  {
    to = moves & ~(moves-1);
    move_list[num_moves] = add_movecapture_white(board, board->pieces[wK], to, wK);
    num_moves++;
    moves = moves^to;
  }
  
  return num_moves;
}

int find_moves_wB_wQ(s_board* board, s_move* move_list)
{
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Bishop
  copy = board->pieces[wB];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wB);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (diagonal)
  copy = board->pieces[wQ];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wQ);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_wR_wQ(s_board* board, s_move* move_list)
{
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Rook
  copy = board->pieces[wR];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wR);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (horizontal and vertical)
  copy = board->pieces[wQ];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[WHITE]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_white(board, from, to, wQ);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}
