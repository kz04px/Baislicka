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
    if(((board->pieces[PAWNS]&board->colour[WHITE]) << 9) & (~U64_FILE_A) & pos) {return 1;} // Up 1 right 1
    if(((board->pieces[PAWNS]&board->colour[WHITE]) << 7) & (~U64_FILE_H) & pos) {return 1;} // Up 1 left 1
  }
  else
  {
    if(((board->pieces[PAWNS]&board->colour[BLACK]) >> 7) & (~U64_FILE_A) & pos) {return 1;} // Down 1 right 1
    if(((board->pieces[PAWNS]&board->colour[BLACK]) >> 9) & (~U64_FILE_H) & pos) {return 1;} // Down 1 left 1
  }
  
  // Knights
  if((board->colour[side] & board->pieces[KNIGHTS]) & magic_moves_knight(sq)) {return 1;}
  
  // Bishops & Queens
  if(magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & (board->pieces[BISHOPS] | board->pieces[QUEENS]))) {return 1;}
  
  // Rooks & Queens
  if(magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & (board->pieces[ROOKS] | board->pieces[QUEENS]))) {return 1;}
  
  // King
  if(magic_moves_king(sq) & board->pieces[KINGS] & board->colour[side]) {return 1;}
  
  return 0;
}

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
    if(((board->pieces[PAWNS]&board->colour[WHITE]) << 9) & (~U64_FILE_A) & (pos)) {attackers |= pos>>9;} // Up 1 right 1
    if(((board->pieces[PAWNS]&board->colour[WHITE]) << 7) & (~U64_FILE_H) & (pos)) {attackers |= pos>>7;} // Up 1 left 1
  }
  else
  {
    if(((board->pieces[PAWNS]&board->colour[BLACK]) >> 7) & (~U64_FILE_A) & (pos)) {attackers |= pos<<7;} // Down 1 right 1
    if(((board->pieces[PAWNS]&board->colour[BLACK]) >> 9) & (~U64_FILE_H) & (pos)) {attackers |= pos<<9;} // Down 1 left 1
  }
  
  // Knights
  attackers |= board->colour[side] & board->pieces[KNIGHTS] & magic_moves_knight(sq);
  
  // Bishops & Queens
  attackers |= magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE] & (board->pieces[BISHOPS] | board->pieces[QUEENS]);
  
  // Rooks & Queens
  attackers |= magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE] & (board->pieces[ROOKS] | board->pieces[QUEENS]);
  
  // King
  attackers |= magic_moves_king(sq) & board->pieces[KINGS] & board->colour[side];
  
  return attackers;
}
