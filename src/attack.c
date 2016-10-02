#include "defs.h"

int square_attacked(s_board* board, uint64_t pos, int side)
{
  assert(board != NULL);
  assert(pos);
  assert(side == WHITE || side == BLACK);
  
  int sq = __builtin_ctzll(pos);
  
  // Pawns
  if(side == WHITE)
  {
    if((board->combined[wP] << 9) & (~U64_COL_H) & (pos)) {return 1;} // Up 1 left 1
    if((board->combined[wP] << 7) & (~U64_COL_A) & (pos)) {return 1;} // Up 1 right 1
  }
  else
  {
    if((board->combined[bP] >> 7) & (~U64_COL_H) & (pos)) {return 1;} // Down 1 left 1
    if((board->combined[bP] >> 9) & (~U64_COL_A) & (pos)) {return 1;} // Down 1 right 1
  }
  
  // Knight
  if((board->colour[side] & board->combined[KNIGHTS]) & magic_moves_knight(sq)) {return 1;}
  
  // Bishop & Queen
  if(magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & (board->combined[BISHOPS] | board->combined[QUEENS]))) {return 1;}
  
  // Rook & Queen
  if(magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & (board->combined[ROOKS] | board->combined[QUEENS]))) {return 1;}
  
  // King
  if((pos<<8) & (board->colour[side] & board->combined[KINGS])) {return 1;} // Up
  if((pos>>8) & (board->colour[side] & board->combined[KINGS])) {return 1;} // Down
  if((pos<<9) & (board->colour[side] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} // Up 1   left 1
  if((pos<<1) & (board->colour[side] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} //        left 1
  if((pos>>7) & (board->colour[side] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} // Down 1 left 1
  if((pos<<7) & (board->colour[side] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} // Up 1   right 1
  if((pos>>1) & (board->colour[side] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} //        right 1
  if((pos>>9) & (board->colour[side] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} // Down 1 right 1
  
  return 0;
}

// Unfinished
uint64_t calculate_attacking(s_board* board, uint64_t pos, int side)
{
  assert(board != NULL);
  assert(pos);
  assert(side == WHITE || side == BLACK);
  
  int sq = __builtin_ctzll(pos);
  uint64_t attackers = 0;
  
  // Pawns
  if(side == WHITE)
  {
    if((board->combined[wP] << 9) & (~U64_COL_H) & (pos)) {return 1;} // Up 1 left 1
    if((board->combined[wP] << 7) & (~U64_COL_A) & (pos)) {return 1;} // Up 1 right 1
  }
  else
  {
    if((board->combined[bP] >> 7) & (~U64_COL_H) & (pos)) {return 1;} // Down 1 left 1
    if((board->combined[bP] >> 9) & (~U64_COL_A) & (pos)) {return 1;} // Down 1 right 1
  }
  
  // Knight
  attackers |= (board->colour[WHITE] & board->combined[KNIGHTS]) & magic_moves_knight(sq);
  
  // Bishop & Queen
  attackers |= magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[WHITE] & (board->combined[BISHOPS] | board->combined[QUEENS]));
  
  // Rook & Queen
  attackers |= magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[WHITE] & (board->combined[ROOKS] | board->combined[QUEENS]));
  
  // King
  if((pos<<8) & (board->colour[WHITE] & board->combined[KINGS])) {return 1;} // Up
  if((pos>>8) & (board->colour[WHITE] & board->combined[KINGS])) {return 1;} // Down
  if((pos<<9) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} // Up 1   left 1
  if((pos<<1) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} //        left 1
  if((pos>>7) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} // Down 1 left 1
  if((pos<<7) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} // Up 1   right 1
  if((pos>>1) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} //        right 1
  if((pos>>9) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} // Down 1 right 1
  
  return attackers;
}
