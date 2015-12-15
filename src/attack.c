#include "defs.h"

int calculate_attacked_white(s_board* board, U64 pos)
{
  ASSERT(board != NULL);
  ASSERT(pos);
  
  int sq = u64_to_sq(pos);
  
  // Pawns
  if((board->pieces[wP] << 9) & (~U64_COL_H) & (pos)) {return TRUE;} // Up 1 left 1
  if((board->pieces[wP] << 7) & (~U64_COL_A) & (pos)) {return TRUE;} // Up 1 right 1
  
  // Knight
  if(board->pieces[wN] & magic_moves_knight(sq)) {return TRUE;}
  
  // Bishop & Queen
  if(magic_moves_diagonal(board->pieces_all, sq) & (board->pieces[wB] | board->pieces[wQ])) {return TRUE;}
  
  // Rook & Queen
  if(magic_moves_hor_ver(board->pieces_all, sq) & (board->pieces[wR] | board->pieces[wQ])) {return TRUE;}
  
  // King
  if((pos<<8) & (board->pieces[wK])) {return TRUE;} // Up
  if((pos>>8) & (board->pieces[wK])) {return TRUE;} // Down
  if((pos<<9) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} // Up 1   left 1
  if((pos<<1) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} //        left 1
  if((pos>>7) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} // Down 1 left 1
  if((pos<<7) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} // Up 1   right 1
  if((pos>>1) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} //        right 1
  if((pos>>9) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} // Down 1 right 1
  
  return FALSE;
}

int calculate_attacked_black(s_board* board, U64 pos)
{
  ASSERT(board != NULL);
  ASSERT(pos);
  
  int sq = u64_to_sq(pos);
  
  // Pawns
  if((board->pieces[bP] >> 7) & (~U64_COL_H) & (pos)) {return TRUE;} // Down 1 left 1
  if((board->pieces[bP] >> 9) & (~U64_COL_A) & (pos)) {return TRUE;} // Down 1 right 1
  
  // Knight
  if(board->pieces[bN] & magic_moves_knight(sq)) {return TRUE;}
  
  // Bishop & Queen
  if(magic_moves_diagonal(board->pieces_all, sq) & (board->pieces[bB] | board->pieces[bQ])) {return TRUE;}
  
  // Rook & Queen
  if(magic_moves_hor_ver(board->pieces_all, sq) & (board->pieces[bR] | board->pieces[bQ])) {return TRUE;}
  
  // King
  if((pos<<8) & (board->pieces[bK])) {return TRUE;} // Up
  if((pos>>8) & (board->pieces[bK])) {return TRUE;} // Down
  if((pos<<9) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} // Up 1   left 1
  if((pos<<1) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} //        left 1
  if((pos>>7) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} // Down 1 left 1
  if((pos<<7) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} // Up 1   right 1
  if((pos>>1) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} //        right 1
  if((pos>>9) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} // Down 1 right 1
  
  return FALSE;
}
