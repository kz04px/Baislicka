#include "defs.h"

uint64_t calculate_attacking_white(s_board* board, uint64_t pos)
{
  assert(board != NULL);
  assert(pos);
  
  int sq = u64_to_sq(pos);
  uint64_t attackers = 0;  
  
  // Pawns
  if((board->combined[wP] << 9) & (~U64_COL_H) & (pos)) {return 1;} // Up 1 left 1
  if((board->combined[wP] << 7) & (~U64_COL_A) & (pos)) {return 1;} // Up 1 right 1
  
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
  
  return 0;
}

int calculate_attacked_white(s_board* board, uint64_t pos)
{
  assert(board != NULL);
  assert(pos);
  
  int sq = u64_to_sq(pos);
  
  // Pawns
  if((board->combined[wP] << 9) & (~U64_COL_H) & (pos)) {return 1;} // Up 1 left 1
  if((board->combined[wP] << 7) & (~U64_COL_A) & (pos)) {return 1;} // Up 1 right 1
  
  // Knight
  if((board->colour[WHITE] & board->combined[KNIGHTS]) & magic_moves_knight(sq)) {return 1;}
  
  // Bishop & Queen
  if(magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[WHITE] & (board->combined[BISHOPS] | board->combined[QUEENS]))) {return 1;}
  
  // Rook & Queen
  if(magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[WHITE] & (board->combined[ROOKS] | board->combined[QUEENS]))) {return 1;}
  
  // King
  if((pos<<8) & (board->colour[WHITE] & board->combined[KINGS])) {return 1;} // Up
  if((pos>>8) & (board->colour[WHITE] & board->combined[KINGS])) {return 1;} // Down
  if((pos<<9) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} // Up 1   left 1
  if((pos<<1) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} //        left 1
  if((pos>>7) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} // Down 1 left 1
  if((pos<<7) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} // Up 1   right 1
  if((pos>>1) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} //        right 1
  if((pos>>9) & (board->colour[WHITE] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} // Down 1 right 1
  
  return 0;
}

int calculate_attacked_black(s_board* board, uint64_t pos)
{
  assert(board != NULL);
  if(!pos)
  {
    display_board(board);
    print_u64(board->colour[WHITE]);
    print_u64(board->colour[BLACK]);
    getchar();
  }
  assert(pos);
  
  int sq = u64_to_sq(pos);
  
  // Pawns
  if((board->combined[bP] >> 7) & (~U64_COL_H) & (pos)) {return 1;} // Down 1 left 1
  if((board->combined[bP] >> 9) & (~U64_COL_A) & (pos)) {return 1;} // Down 1 right 1
  
  // Knight
  if((board->colour[BLACK] & board->combined[KNIGHTS]) & magic_moves_knight(sq)) {return 1;}
  
  // Bishop & Queen
  if(magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[BLACK] & (board->combined[BISHOPS] | board->combined[QUEENS]))) {return 1;}
  
  // Rook & Queen
  if(magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[BLACK] & (board->combined[ROOKS] | board->combined[QUEENS]))) {return 1;}
  
  // King
  if((pos<<8) & (board->colour[BLACK] & board->combined[KINGS])) {return 1;} // Up
  if((pos>>8) & (board->colour[BLACK] & board->combined[KINGS])) {return 1;} // Down
  if((pos<<9) & (board->colour[BLACK] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} // Up 1   left 1
  if((pos<<1) & (board->colour[BLACK] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} //        left 1
  if((pos>>7) & (board->colour[BLACK] & board->combined[KINGS]) & (~U64_COL_H)) {return 1;} // Down 1 left 1
  if((pos<<7) & (board->colour[BLACK] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} // Up 1   right 1
  if((pos>>1) & (board->colour[BLACK] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} //        right 1
  if((pos>>9) & (board->colour[BLACK] & board->combined[KINGS]) & (~U64_COL_A)) {return 1;} // Down 1 right 1
  
  return 0;
}
