#include "defs.h"

uint64_t calculate_attacking_white(s_board* board, uint64_t pos)
{
  assert(board != NULL);
  assert(pos);
  
  int sq = u64_to_sq(pos);
  uint64_t attackers = 0;  
  
  // Pawns
  if((board->pieces[wP] << 9) & (~U64_COL_H) & (pos)) {return 1;} // Up 1 left 1
  if((board->pieces[wP] << 7) & (~U64_COL_A) & (pos)) {return 1;} // Up 1 right 1
  
  // Knight
  attackers |= board->pieces[wN] & magic_moves_knight(sq);
  
  // Bishop & Queen
  attackers |= magic_moves_diagonal(board->pieces_all, sq) & (board->pieces[wB] | board->pieces[wQ]);
  
  // Rook & Queen
  attackers |= magic_moves_hor_ver(board->pieces_all, sq) & (board->pieces[wR] | board->pieces[wQ]);
  
  // King
  if((pos<<8) & (board->pieces[wK])) {return 1;} // Up
  if((pos>>8) & (board->pieces[wK])) {return 1;} // Down
  if((pos<<9) & (board->pieces[wK]) & (~U64_COL_H)) {return 1;} // Up 1   left 1
  if((pos<<1) & (board->pieces[wK]) & (~U64_COL_H)) {return 1;} //        left 1
  if((pos>>7) & (board->pieces[wK]) & (~U64_COL_H)) {return 1;} // Down 1 left 1
  if((pos<<7) & (board->pieces[wK]) & (~U64_COL_A)) {return 1;} // Up 1   right 1
  if((pos>>1) & (board->pieces[wK]) & (~U64_COL_A)) {return 1;} //        right 1
  if((pos>>9) & (board->pieces[wK]) & (~U64_COL_A)) {return 1;} // Down 1 right 1
  
  return 0;
}

int calculate_attacked_white(s_board* board, uint64_t pos)
{
  assert(board != NULL);
  assert(pos);
  
  int sq = u64_to_sq(pos);
  
  // Pawns
  if((board->pieces[wP] << 9) & (~U64_COL_H) & (pos)) {return 1;} // Up 1 left 1
  if((board->pieces[wP] << 7) & (~U64_COL_A) & (pos)) {return 1;} // Up 1 right 1
  
  // Knight
  if(board->pieces[wN] & magic_moves_knight(sq)) {return 1;}
  
  // Bishop & Queen
  if(magic_moves_diagonal(board->pieces_all, sq) & (board->pieces[wB] | board->pieces[wQ])) {return 1;}
  
  // Rook & Queen
  if(magic_moves_hor_ver(board->pieces_all, sq) & (board->pieces[wR] | board->pieces[wQ])) {return 1;}
  
  // King
  if((pos<<8) & (board->pieces[wK])) {return 1;} // Up
  if((pos>>8) & (board->pieces[wK])) {return 1;} // Down
  if((pos<<9) & (board->pieces[wK]) & (~U64_COL_H)) {return 1;} // Up 1   left 1
  if((pos<<1) & (board->pieces[wK]) & (~U64_COL_H)) {return 1;} //        left 1
  if((pos>>7) & (board->pieces[wK]) & (~U64_COL_H)) {return 1;} // Down 1 left 1
  if((pos<<7) & (board->pieces[wK]) & (~U64_COL_A)) {return 1;} // Up 1   right 1
  if((pos>>1) & (board->pieces[wK]) & (~U64_COL_A)) {return 1;} //        right 1
  if((pos>>9) & (board->pieces[wK]) & (~U64_COL_A)) {return 1;} // Down 1 right 1
  
  return 0;
}

int calculate_attacked_black(s_board* board, uint64_t pos)
{
  assert(board != NULL);
  assert(pos);
  
  int sq = u64_to_sq(pos);
  
  // Pawns
  if((board->pieces[bP] >> 7) & (~U64_COL_H) & (pos)) {return 1;} // Down 1 left 1
  if((board->pieces[bP] >> 9) & (~U64_COL_A) & (pos)) {return 1;} // Down 1 right 1
  
  // Knight
  if(board->pieces[bN] & magic_moves_knight(sq)) {return 1;}
  
  // Bishop & Queen
  if(magic_moves_diagonal(board->pieces_all, sq) & (board->pieces[bB] | board->pieces[bQ])) {return 1;}
  
  // Rook & Queen
  if(magic_moves_hor_ver(board->pieces_all, sq) & (board->pieces[bR] | board->pieces[bQ])) {return 1;}
  
  // King
  if((pos<<8) & (board->pieces[bK])) {return 1;} // Up
  if((pos>>8) & (board->pieces[bK])) {return 1;} // Down
  if((pos<<9) & (board->pieces[bK]) & (~U64_COL_H)) {return 1;} // Up 1   left 1
  if((pos<<1) & (board->pieces[bK]) & (~U64_COL_H)) {return 1;} //        left 1
  if((pos>>7) & (board->pieces[bK]) & (~U64_COL_H)) {return 1;} // Down 1 left 1
  if((pos<<7) & (board->pieces[bK]) & (~U64_COL_A)) {return 1;} // Up 1   right 1
  if((pos>>1) & (board->pieces[bK]) & (~U64_COL_A)) {return 1;} //        right 1
  if((pos>>9) & (board->pieces[bK]) & (~U64_COL_A)) {return 1;} // Down 1 right 1
  
  return 0;
}
