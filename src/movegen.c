#include "defs.h"

int find_moves_wP_captures(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int to;
  
  // ep
  if(board->ep)
  {
    int ep_sq = __builtin_ctzll(board->ep);
    
    // Down & Left
    if((board->ep>>7) & board->combined[wP] & (~U64_COL_H))
    {
      move_list[num_moves] = move_add(board, ep_sq-7, ep_sq, EP, wP);
      num_moves++;
    }
    
    // Down & Right
    if((board->ep>>9) & board->combined[wP] & (~U64_COL_A))
    {
      move_list[num_moves] = move_add(board, ep_sq-9, ep_sq, EP, wP);
      num_moves++;
    }
  }
  
  // Up 1 Left 1
  moves = (board->combined[wP]<<9) & (board->colour[BLACK]) & (~U64_COL_H);
  while(moves)
  {
    to = __builtin_ctzll(moves);
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to-9, to);
    moves &= moves-1;
  }
  
  // Up 1 Right 1
  moves = (board->combined[wP]<<7) & (board->colour[BLACK]) & (~U64_COL_A);
  while(moves)
  {
    to = __builtin_ctzll(moves);
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to-7, to);
    moves &= moves-1;
  }
  
  return num_moves;
}

int find_moves_wP_quiet(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int to;
  
  // Up 1
  moves = (board->combined[wP]<<8) & (~(board->colour[WHITE]|board->colour[BLACK]));
  while(moves)
  {
    to = __builtin_ctzll(moves);
    num_moves += move_add_pawn_white(board, &move_list[num_moves], to-8, to);
    
    // Up 2
    if(to <= 23 && 16 <= to && (((uint64_t)1<<(to+8))&(~(board->colour[WHITE]|board->colour[BLACK]))))
    {
      move_list[num_moves] = move_add(board, to-8, to+8, DOUBLE_PAWN, wP);
      num_moves++;
    }
    
    moves &= moves-1;
  }
  
  return num_moves;
}

int find_moves_bP_captures(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int to;
  
  // ep
  if(board->ep)
  {
    int ep_sq = __builtin_ctzll(board->ep);
    
    // Up & Right
    if((board->ep<<7) & (board->combined[bP]) & (~U64_COL_A))
    {
      move_list[num_moves] = move_add(board, ep_sq+7, ep_sq, EP, bP);
      num_moves++;
    }
    
    // Up & Left
    if((board->ep<<9) & (board->combined[bP]) & (~U64_COL_H))
    {
      move_list[num_moves] = move_add(board, ep_sq+9, ep_sq, EP, bP);
      num_moves++;
    }
  }
  
  // Down 1 Left 1
  moves = (board->combined[bP]>>7) & (board->colour[WHITE]) & (~U64_COL_H);
  while(moves)
  {
    to = __builtin_ctzll(moves);
    
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to+7, to);
    moves &= moves-1;
  }
  
  // Down 1 Right 1
  moves = (board->combined[bP]>>9) & (board->colour[WHITE]) & (~U64_COL_A);
  while(moves)
  {
    to = __builtin_ctzll(moves);
    
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to+9, to);
    moves &= moves-1;
  }
  
  return num_moves;
}

int find_moves_bP_quiet(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int to;
  
  // Down 1
  moves = (board->combined[bP]>>8) & (~(board->colour[WHITE]|board->colour[BLACK]));
  while(moves)
  {
    to = __builtin_ctzll(moves);
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to+8, to);
    
    // Down 2
    if(40 <= to && to <= 47 && (((uint64_t)1<<(to-8))&(~(board->colour[WHITE]|board->colour[BLACK]))))
    {
      move_list[num_moves] = move_add(board, to+8, to-8, DOUBLE_PAWN, bP);
      num_moves++;
    }
    
    moves &= moves-1;
  }
  
  return num_moves;
}

int find_moves_kings_quiet(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int to;
  
  // castling
  if(board->turn == WHITE)
  {
    if(board->castling[wKSC] &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_F1) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_G1) &&
       square_attacked(board, U64_E1, BLACK) == 0 &&
       square_attacked(board, U64_F1, BLACK) == 0 &&
       square_attacked(board, U64_G1, BLACK) == 0)
    {
      move_list[num_moves] = move_add(board, E1, G1, KSC, KINGS);
      num_moves++;
    }
    if(board->castling[wQSC] &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_B1) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_C1) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_D1) &&
       square_attacked(board, U64_E1, BLACK) == 0 &&
       square_attacked(board, U64_D1, BLACK) == 0 &&
       square_attacked(board, U64_C1, BLACK) == 0)
    {
      move_list[num_moves] = move_add(board, E1, C1, QSC, KINGS);
      num_moves++;
    }
  }
  else
  {
    if(board->castling[bKSC] &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_F8) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_G8) &&
       square_attacked(board, U64_E8, WHITE) == 0 &&
       square_attacked(board, U64_F8, WHITE) == 0 &&
       square_attacked(board, U64_G8, WHITE) == 0)
    {
      move_list[num_moves] = move_add(board, E8, G8, KSC, KINGS);
      num_moves++;
    }
    if(board->castling[bQSC] &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_B8) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_C8) &&
       !((board->colour[WHITE]|board->colour[BLACK])&U64_D8) &&
       square_attacked(board, U64_E8, WHITE) == 0 &&
       square_attacked(board, U64_D8, WHITE) == 0 &&
       square_attacked(board, U64_C8, WHITE) == 0)
    {
      move_list[num_moves] = move_add(board, E8, C8, QSC, KINGS);
      num_moves++;
    }
  }
  
  uint64_t from = __builtin_ctzll(board->colour[board->turn] & board->combined[KINGS]);
  moves = magic_moves_king(from) & ~(board->colour[board->turn]|board->colour[!board->turn]);
  while(moves)
  {
    to = __builtin_ctzll(moves);
    move_list[num_moves] = add_movecapture(board, from, to, KINGS);
    num_moves++;
    moves &= moves-1;
  }
  
  return num_moves;
}

int find_moves_kings_captures(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int to;
  
  uint64_t from = __builtin_ctzll(board->colour[board->turn] & board->combined[KINGS]);
  moves = magic_moves_king(from) & board->colour[!board->turn];
  while(moves)
  {
    to = __builtin_ctzll(moves);
    move_list[num_moves] = add_movecapture(board, from, to, KINGS);
    num_moves++;
    moves &= moves-1;
  }
  
  return num_moves;
}

int find_moves_knights(s_board* board, s_move* move_list, uint64_t allowed)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int from;
  int to;
  uint64_t copy;
  
  copy = board->combined[KNIGHTS] & board->colour[board->turn];
  while(copy)
  {
    from = __builtin_ctzll(copy);
    moves = magic_moves_knight(from) & allowed;
    
    while(moves)
    {
      to = __builtin_ctzll(moves);
      move_list[num_moves] = add_movecapture(board, from, to, KNIGHTS);
      num_moves++;
      moves &= moves-1;
    }
    copy &= copy-1;
  }
  
  return num_moves;
}

int find_moves_bishops_queens(s_board* board, s_move* move_list, uint64_t allowed)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int from;
  int to;
  uint64_t copy;
  
  // Bishops
  copy = board->combined[BISHOPS] & board->colour[board->turn];
  while(copy)
  {
    from = __builtin_ctzll(copy);
    moves = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;
    
    while(moves)
    {
      to = __builtin_ctzll(moves);
      move_list[num_moves] = add_movecapture(board, from, to, BISHOPS);
      num_moves++;
      moves &= moves-1;
    }
    copy &= copy-1;
  }
  
  // Queens (diagonal)
  copy = board->combined[QUEENS] & board->colour[board->turn];
  while(copy)
  {
    from = __builtin_ctzll(copy);
    moves = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;
    
    while(moves)
    {
      to = __builtin_ctzll(moves);
      move_list[num_moves] = add_movecapture(board, from, to, QUEENS);
      num_moves++;
      moves &= moves-1;
    }
    copy &= copy-1;
  }
  
  return num_moves;
}

int find_moves_rooks_queens(s_board* board, s_move* move_list, uint64_t allowed)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  int from;
  int to;
  uint64_t copy;
  
  // Rook
  copy = board->combined[ROOKS] & board->colour[board->turn];
  while(copy)
  {
    from = __builtin_ctzll(copy);
    moves = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;
    
    while(moves)
    {
      to = __builtin_ctzll(moves);
      move_list[num_moves] = add_movecapture(board, from, to, ROOKS);
      num_moves++;
      moves &= moves-1;
    }
    copy &= copy-1;
  }
  
  // Queen (horizontal and vertical)
  copy = board->combined[QUEENS] & board->colour[board->turn];
  while(copy)
  {
    from = __builtin_ctzll(copy);
    moves = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;
    
    while(moves)
    {
      to = __builtin_ctzll(moves);
      move_list[num_moves] = add_movecapture(board, from, to, QUEENS);
      num_moves++;
      moves &= moves-1;
    }
    copy &= copy-1;
  }
  
  return num_moves;
}

int find_moves(s_board* board, s_move* move_list, int colour)
{
  assert(board != NULL);
  assert(move_list != NULL);
  assert(colour == WHITE || colour == BLACK);
  
  uint64_t allowed = ~board->colour[board->turn];
  
  int num_moves = 0;
  
  num_moves += find_moves_bishops_queens(board, &move_list[num_moves], allowed);
  num_moves += find_moves_rooks_queens(board, &move_list[num_moves], allowed);
  num_moves += find_moves_knights(board, &move_list[num_moves], allowed);
  
  if(colour == WHITE)
  {
    num_moves += find_moves_wP_quiet(board, &move_list[num_moves]);
    num_moves += find_moves_wP_captures(board, &move_list[num_moves]);
  }
  else
  {
    num_moves += find_moves_bP_quiet(board, &move_list[num_moves]);
    num_moves += find_moves_bP_captures(board, &move_list[num_moves]);
  }
  
  num_moves += find_moves_kings_quiet(board, &move_list[num_moves]);
  num_moves += find_moves_kings_captures(board, &move_list[num_moves]);
  
  return num_moves;
}

int find_moves_captures(s_board* board, s_move* move_list, int colour)
{
  assert(board != NULL);
  assert(move_list != NULL);
  assert(colour == WHITE || colour == BLACK);
  
  uint64_t allowed = board->colour[!board->turn];
  
  int num_moves = 0;
  
  num_moves += find_moves_bishops_queens(board, &move_list[num_moves], allowed);
  num_moves += find_moves_rooks_queens(board, &move_list[num_moves], allowed);
  num_moves += find_moves_knights(board, &move_list[num_moves], allowed);
  
  if(colour == WHITE)
  {
    num_moves += find_moves_wP_captures(board, &move_list[num_moves]);
  }
  else
  {
    num_moves += find_moves_bP_captures(board, &move_list[num_moves]);
  }
  
  num_moves += find_moves_kings_captures(board, &move_list[num_moves]);
  
  return num_moves;
}
