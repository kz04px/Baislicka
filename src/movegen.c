#include "defs.h"

int find_moves_wP(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t to;
  
  // ep
  if(board->ep)
  {
    // Down & Left
    if((board->ep>>7) & board->combined[wP] & (~U64_COL_H))
    {
      move_list[num_moves] = move_add(board, board->ep>>7, board->ep, EP, wP);
      num_moves++;
    }
    
    // Down & Right
    if((board->ep>>9) & board->combined[wP] & (~U64_COL_A))
    {
      move_list[num_moves] = move_add(board, board->ep>>9, board->ep, EP, wP);
      num_moves++;
    }
  }
  
  // Up 1
  moves = (board->combined[wP]<<8) & (~(board->colour[WHITE]|board->colour[BLACK]));
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to>>8, to);
    
    // Up 2
    if((to&U64_ROW_3) && ((to<<8)&(~(board->colour[WHITE]|board->colour[BLACK]))))
    {
      move_list[num_moves] = move_add(board, to>>8, to<<8, DOUBLE_PAWN, wP);
      num_moves++;
    }
    
    moves = moves^to;
  }
  // Up 1 Left 1
  moves = (board->combined[wP]<<9) & (board->colour[BLACK]) & (~U64_COL_H);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to>>9, to);
    moves = moves^to;
  }
  // Up 1 Right 1
  moves = (board->combined[wP]<<7) & (board->colour[BLACK]) & (~U64_COL_A);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to>>7, to);
    moves = moves^to;
  }
  
  return num_moves;
}

int find_moves_bP(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t to;
  
  // ep
  if(board->ep)
  {
    // Up & Right
    if((board->ep<<7) & (board->combined[bP]) & (~U64_COL_A))
    {
      move_list[num_moves] = move_add(board, board->ep<<7, board->ep, EP, bP);
      num_moves++;
    }
    
    // Up & Left
    if((board->ep<<9) & (board->combined[bP]) & (~U64_COL_H))
    {
      move_list[num_moves] = move_add(board, board->ep<<9, board->ep, EP, bP);
      num_moves++;
    }
  }
  
  // Down 1
  moves = (board->combined[bP]>>8) & (~(board->colour[WHITE]|board->colour[BLACK]));
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to<<8, to);
    
    // Down 2
    if((to&U64_ROW_6) && ((to>>8)&(~(board->colour[WHITE]|board->colour[BLACK]))))
    {
      move_list[num_moves] = move_add(board, to<<8, to>>8, DOUBLE_PAWN, bP);
      num_moves++;
    }
    
    moves = moves^to;
  }
  // Down 1 Left 1
  moves = (board->combined[bP]>>7) & (board->colour[WHITE]) & (~U64_COL_H);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to<<7, to);
    moves = moves^to;
  }
  // Down 1 Right 1
  moves = (board->combined[bP]>>9) & (board->colour[WHITE]) & (~U64_COL_A);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to<<9, to);
    moves = moves^to;
  }
  
  return num_moves;
}

int find_moves_kings(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t to;
  
  // castling
  if(board->turn == WHITE)
  {
    if(board->castling[wKSC] &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_F1) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_G1) &&
       calculate_attacked_black(board, U64_E1) == 0 &&
       calculate_attacked_black(board, U64_F1) == 0 &&
       calculate_attacked_black(board, U64_G1) == 0)
    {
      move_list[num_moves] = move_add(board, U64_E1, U64_G1, KSC, KINGS);
      num_moves++;
    }
    if(board->castling[wQSC] &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_B1) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_C1) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_D1) &&
       calculate_attacked_black(board, U64_E1) == 0 &&
       calculate_attacked_black(board, U64_D1) == 0 &&
       calculate_attacked_black(board, U64_C1) == 0)
    {
      move_list[num_moves] = move_add(board, U64_E1, U64_C1, QSC, KINGS);
      num_moves++;
    }
  }
  else
  {
    if(board->castling[bKSC] &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_F8) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_G8) &&
       calculate_attacked_white(board, U64_E8) == 0 &&
       calculate_attacked_white(board, U64_F8) == 0 &&
       calculate_attacked_white(board, U64_G8) == 0)
    {
      move_list[num_moves] = move_add(board, U64_E8, U64_G8, KSC, KINGS);
      num_moves++;
    }
    if(board->castling[bQSC] &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_B8) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_C8) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_D8) &&
       calculate_attacked_white(board, U64_E8) == 0 &&
       calculate_attacked_white(board, U64_D8) == 0 &&
       calculate_attacked_white(board, U64_C8) == 0)
    {
      move_list[num_moves] = move_add(board, U64_E8, U64_C8, QSC, KINGS);
      num_moves++;
    }
  }
  
  moves  = ((board->colour[board->turn] & board->combined[KINGS])<<8); // Up 1
  moves |= ((board->colour[board->turn] & board->combined[KINGS])>>8); // Down 1
  moves |= ((board->colour[board->turn] & board->combined[KINGS])<<1) & (~U64_COL_H); // Left 1
  moves |= ((board->colour[board->turn] & board->combined[KINGS])>>1) & (~U64_COL_A); // Right 1
  moves |= ((board->colour[board->turn] & board->combined[KINGS])<<7) & (~U64_COL_A); // Up 1 Right 1
  moves |= ((board->colour[board->turn] & board->combined[KINGS])<<9) & (~U64_COL_H); // Up 1 Left 1
  moves |= ((board->colour[board->turn] & board->combined[KINGS])>>7) & (~U64_COL_H); // Down 1 Left 1
  moves |= ((board->colour[board->turn] & board->combined[KINGS])>>9) & (~U64_COL_A); // Down 1 Right 1
  moves &= ~board->colour[board->turn];
  while(moves)
  {
    to = moves & ~(moves-1);
    move_list[num_moves] = add_movecapture(board, (board->colour[board->turn] & board->combined[KINGS]), to, KINGS);
    num_moves++;
    moves = moves^to;
  }
  
  return num_moves;
}

int find_moves_knights(s_board* board, s_move* move_list, uint64_t allowed)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->combined[KNIGHTS] & board->colour[board->turn];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_knight(u64_to_sq(from)) & allowed;
    
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture(board, from, to, KNIGHTS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_bishops_queens(s_board* board, s_move* move_list, uint64_t allowed)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Bishops (diagonal)
  copy = board->combined[BISHOPS] & board->colour[board->turn];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & allowed;
    
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture(board, from, to, BISHOPS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queens (diagonal)
  copy = board->combined[QUEENS] & board->colour[board->turn];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & allowed;
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture(board, from, to, QUEENS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_rooks_queens(s_board* board, s_move* move_list, uint64_t allowed)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Rook
  copy = board->combined[ROOKS] & board->colour[board->turn];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & allowed;
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture(board, from, to, ROOKS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (horizontal and vertical)
  copy = board->combined[QUEENS] & board->colour[board->turn];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), u64_to_sq(from)) & allowed;
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture(board, from, to, QUEENS);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves(s_board* board, s_move* move_list, int colour, int attacking)
{
  assert(board != NULL);
  assert(move_list != NULL);
  assert(colour == WHITE || colour == BLACK);
  
  uint64_t allowed = ~board->colour[board->turn];
  if(attacking == MOVES_CAPTURES)
  {
    allowed = board->colour[!board->turn];
  }
  
  int num_moves = 0;
  
  num_moves += find_moves_bishops_queens(board, &move_list[num_moves], allowed);
  num_moves += find_moves_rooks_queens(board, &move_list[num_moves], allowed);
  num_moves += find_moves_knights(board, &move_list[num_moves], allowed);
  
  if(colour == WHITE)
  {
    num_moves += find_moves_wP(board, &move_list[num_moves]);
  }
  else
  {
    num_moves += find_moves_bP(board, &move_list[num_moves]);
  }
  
  num_moves += find_moves_kings(board, &move_list[num_moves]);
  
  return num_moves;
}
