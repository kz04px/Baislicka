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
  if((board->pieces[bP] >> 7) & (~U64_COL_H) & (sq)) {return TRUE;} // Down 1 left 1
  if((board->pieces[bP] >> 9) & (~U64_COL_A) & (sq)) {return TRUE;} // Down 1 right 1
  
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

int calculate_attacked(s_board *board, U64 sq, int attacking_colour)
{
  U64 pos;
  if(attacking_colour == WHITE)
  {
    // Pawns
    if((board->pieces[wP] << 9) & (~U64_COL_H) & (sq)) {return TRUE;} // Up 1 left 1
    if((board->pieces[wP] << 7) & (~U64_COL_A) & (sq)) {return TRUE;} // Up 1 right 1
    // Knights
    if((board->pieces[wN] << 17) & (~U64_COL_H) & (sq))      {return TRUE;} // Up 2 left 1
    else if((board->pieces[wN] << 15) & (~U64_COL_A) & (sq)) {return TRUE;} // Up 2 right 1
    else if((board->pieces[wN] >> 17) & (~U64_COL_A) & (sq)) {return TRUE;} // Down 2 right 1
    else if((board->pieces[wN] >> 15) & (~U64_COL_H) & (sq)) {return TRUE;} // Down 2 left 1
    else if((board->pieces[wN] << 10) & ~(U64_COL_G|U64_COL_H) & (sq)) {return TRUE;} // Left 2 up 1
    else if((board->pieces[wN] >> 6)  & ~(U64_COL_G|U64_COL_H) & (sq)) {return TRUE;} // Left 2 down 1
    else if((board->pieces[wN] << 6)  & ~(U64_COL_A|U64_COL_B) & (sq)) {return TRUE;} // Right 2 up 1
    else if((board->pieces[wN] >> 10) & ~(U64_COL_A|U64_COL_B) & (sq)) {return TRUE;} // Right 2 down 1
    // Diagonals (Bishops & Queens)
    pos = (sq << 9) & (~U64_COL_H); // Up 1 left 1
    while(pos)
    {
      if(pos&board->pieces[wB]) {return TRUE;}
      if(pos&board->pieces[wQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos << 9) & (~U64_COL_H);
    }
    pos = (sq << 7) & (~U64_COL_A); // Up 1 right 1
    while(pos)
    {
      if(pos&board->pieces[wB]) {return TRUE;}
      if(pos&board->pieces[wQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos << 7) & (~U64_COL_A);
    }
    pos = (sq >> 7) & (~U64_COL_H); // Down 1 left 1
    while(pos)
    {
      if(pos&board->pieces[wB]) {return TRUE;}
      if(pos&board->pieces[wQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos >> 7) & (~U64_COL_H);
    }
    pos = (sq >> 9) & (~U64_COL_A); // Down 1 right 1
    while(pos)
    {
      if(pos&board->pieces[wB]) {return TRUE;}
      if(pos&board->pieces[wQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos >> 9) & (~U64_COL_A);
    }
    // Horizontal & Vertical (Rooks & Queens)
    pos = (sq >> 1) & (~U64_COL_A); // Right
    while(pos)
    {
      if(pos&board->pieces[wR]) {return TRUE;}
      if(pos&board->pieces[wQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos >> 1) & (~U64_COL_A);
    }
    pos = (sq << 1) & (~U64_COL_H); // Left
    while(pos)
    {
      if(pos&board->pieces[wR]) {return TRUE;}
      if(pos&board->pieces[wQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos << 1) & (~U64_COL_H);
    }
    pos = (sq << 8); // Up
    while(pos)
    {
      if(pos&board->pieces[wR]) {return TRUE;}
      if(pos&board->pieces[wQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos << 8);
    }
    pos = (sq >> 8); // Down
    while(pos)
    {
      if(pos&board->pieces[wR]) {return TRUE;}
      if(pos&board->pieces[wQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos >> 8);
    }
    // King
    if((sq<<8) & (board->pieces[wK])) {return TRUE;} // Up
    if((sq>>8) & (board->pieces[wK])) {return TRUE;} // Down
    if((sq<<9) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} // Up 1   left 1
    if((sq<<1) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} //        left 1
    if((sq>>7) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} // Down 1 left 1
    if((sq<<7) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} // Up 1   right 1
    if((sq>>1) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} //        right 1
    if((sq>>9) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} // Down 1 right 1
  }
  else
  {
    // Pawns
    if((board->pieces[bP] >> 7) & (~U64_COL_H) & (sq)) {return TRUE;} // Down 1 left 1
    if((board->pieces[bP] >> 9) & (~U64_COL_A) & (sq)) {return TRUE;} // Down 1 right 1
    // Knights
    if((board->pieces[bN] << 17) & (~U64_COL_H) & (sq))      {return TRUE;} // Up 2 left 1
    else if((board->pieces[bN] << 15) & (~U64_COL_A) & (sq)) {return TRUE;} // Up 2 right 1
    else if((board->pieces[bN] >> 17) & (~U64_COL_A) & (sq)) {return TRUE;} // Down 2 right 1
    else if((board->pieces[bN] >> 15) & (~U64_COL_H) & (sq)) {return TRUE;} // Down 2 left 1
    else if((board->pieces[bN] << 10) & ~(U64_COL_G|U64_COL_H) & (sq)) {return TRUE;} // Left 2 up 1
    else if((board->pieces[bN] >> 6)  & ~(U64_COL_G|U64_COL_H) & (sq)) {return TRUE;} // Left 2 down 1
    else if((board->pieces[bN] << 6)  & ~(U64_COL_A|U64_COL_B) & (sq)) {return TRUE;} // Right 2 up 1
    else if((board->pieces[bN] >> 10) & ~(U64_COL_A|U64_COL_B) & (sq)) {return TRUE;} // Right 2 down 1
    // Diagonals (Bishops & Queens)
    pos = (sq << 9) & (~U64_COL_H); // Up 1 left 1
    while(pos)
    {
      if(pos&board->pieces[bB]) {return TRUE;}
      if(pos&board->pieces[bQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos << 9) & (~U64_COL_H);
    }
    pos = (sq << 7) & (~U64_COL_A); // Up 1 right 1
    while(pos)
    {
      if(pos&board->pieces[bB]) {return TRUE;}
      if(pos&board->pieces[bQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos << 7) & (~U64_COL_A);
    }
    pos = (sq >> 7) & (~U64_COL_H); // Down 1 left 1
    while(pos)
    {
      if(pos&board->pieces[bB]) {return TRUE;}
      if(pos&board->pieces[bQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos >> 7) & (~U64_COL_H);
    }
    pos = (sq >> 9) & (~U64_COL_A); // Down 1 right 1
    while(pos)
    {
      if(pos&board->pieces[bB]) {return TRUE;}
      if(pos&board->pieces[bQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos >> 9) & (~U64_COL_A);
    }
    // Horizontal & Vertical (Rooks & Queens)
    pos = (sq >> 1) & (~U64_COL_A); // Right
    while(pos)
    {
      if(pos&board->pieces[bR]) {return TRUE;}
      if(pos&board->pieces[bQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos >> 1) & (~U64_COL_A);
    }
    pos = (sq << 1) & (~U64_COL_H); // Left
    while(pos)
    {
      if(pos&board->pieces[bR]) {return TRUE;}
      if(pos&board->pieces[bQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos << 1) & (~U64_COL_H);
    }
    pos = (sq << 8); // Up
    while(pos)
    {
      if(pos&board->pieces[bR]) {return TRUE;}
      if(pos&board->pieces[bQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos << 8);
    }
    pos = (sq >> 8); // Down
    while(pos)
    {
      if(pos&board->pieces[bR]) {return TRUE;}
      if(pos&board->pieces[bQ]) {return TRUE;}
      if(pos&board->pieces_all) {break;}
      pos = (pos >> 8);
    }
    // King
    if((sq<<8) & (board->pieces[bK])) {return TRUE;} // Up
    if((sq>>8) & (board->pieces[bK])) {return TRUE;} // Down
    if((sq<<9) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} // Up 1   left 1
    if((sq<<1) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} //        left 1
    if((sq>>7) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} // Down 1 left 1
    if((sq<<7) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} // Up 1   right 1
    if((sq>>1) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} //        right 1
    if((sq>>9) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} // Down 1 right 1
  }
  
  return FALSE;
}

int u64_to_sq(U64 pos)
{
  int result = 0;
  
       if(pos&U64_ROW_1) {result =  0;}
  else if(pos&U64_ROW_2) {result =  8;}
  else if(pos&U64_ROW_3) {result = 16;}
  else if(pos&U64_ROW_4) {result = 24;}
  else if(pos&U64_ROW_5) {result = 32;}
  else if(pos&U64_ROW_6) {result = 40;}
  else if(pos&U64_ROW_7) {result = 48;}
  else if(pos&U64_ROW_8) {result = 56;}
  
       if(pos&U64_COL_A) {result += 7;}
  else if(pos&U64_COL_B) {result += 6;}
  else if(pos&U64_COL_C) {result += 5;}
  else if(pos&U64_COL_D) {result += 4;}
  else if(pos&U64_COL_E) {result += 3;}
  else if(pos&U64_COL_F) {result += 2;}
  else if(pos&U64_COL_G) {result += 1;}
  else if(pos&U64_COL_H) {result += 0;}
  
  return result;
}