#include "defs.h"

#define INF 999999

const int piece_values[12] = { 100,  300,  300,  500,  900,  INF,
                              -100, -300, -300, -500, -900, -INF};

const int piece_location_bonus[12][64] = {
{
// White Pawns
// H   G   F   E   D   C   B   A
   0,  0,  0,  0,  0,  0,  0,  0, // 1
  20, 20, 20, 20, 20, 20, 20, 20, // 2
   0,  0,-10, 30, 30, 20, 20, 20, // 3
   0,  0,  0, 40, 40, 30, 30, 30, // 4
   0,  0,  0, 50, 50, 40, 40, 40, // 5
  50, 50, 50, 60, 60, 60, 60, 60, // 6
  80, 80, 80, 80, 80, 80, 80, 80, // 7
   0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// White Knights
// H   G   F   E   D   C   B   A
 -50,-30,-10,-10,-10,-10,-30,-50, // 1
 -30,-10,  0,  0,  0,  0,-10,-30, // 2
 -10,  0, 50, 40, 40, 20,  0,-10, // 3
 -10,  0, 40, 50, 50, 40,  0,-10, // 4
 -10,  0, 40, 50, 50, 40,  0,-10, // 5
 -10,  0, 20, 40, 40, 20,  0,-10, // 6
 -30,-10,  0,  0,  0,  0,-10,-30, // 7
 -50,-30,-10,-10,-10,-10,-30,-50  // 8
},{
// White Bishops
// H   G   F   E   D   C   B   A
 -10,-10,-10,-10,-10,-10,-10,-10, // 1
   0, 30,  0, 30, 30,  0, 30,  0, // 2
   0,  0,  0, 30, 30,  0,  0,  0, // 3
   0,  0,  0,  0,  0,  0,  0,  0, // 4
   0,  0,  0,  0,  0,  0,  0,  0, // 5
   0,  0,  0,  0,  0,  0,  0,  0, // 6
   0,  0,  0,  0,  0,  0,  0,  0, // 7
 -10,-10,-10,-10,-10,-10,-10,-10  // 8
},{
// White Rooks
// H   G   F   E   D   C   B   A
  10,  0, 30, 30, 30, 30, 30, 30, // 1
   0,  0,  0,  0,  0,  0,  0,  0, // 2
   0,  0,  0,  0,  0,  0,  0,  0, // 3
   0,  0,  0,  0,  0,  0,  0,  0, // 4
   0,  0,  0,  0,  0,  0,  0,  0, // 5
   0,  0,  0,  0,  0,  0,  0,  0, // 6
  40, 40, 40, 40, 40, 40, 40, 40, // 7
   0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// White Queens
// H   G   F   E   D   C   B   A
   0,  0,  0,  0,  0,  0,  0,  0, // 1
   0,  0,  0,  0,  0,  0,  0,  0, // 2
   0,  0,  0,  0,  0,  0,  0,  0, // 3
   0,  0,  0,  0,  0,  0,  0,  0, // 4
   0,  0,  0,  0,  0,  0,  0,  0, // 5
   0,  0,  0,  0,  0,  0,  0,  0, // 6
   0,  0,  0,  0,  0,  0,  0,  0, // 7
   0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// White King
// H   G   F   E   D   C   B   A
  20, 50,  0,  0, 20, 30, 20,  0, // 1
   0,  0,  0,  0,  0,  0,  0,  0, // 2
 -10,-10,-10,-20,-20,-10,-10,-10, // 3
 -10,-10,-20,-30,-30,-20,-10,-10, // 4
 -10,-10,-20,-30,-30,-20,-10,-10, // 5
 -10,-10,-10,-20,-20,-10,-10,-10, // 6
 -10,-10,-10,-10,-10,-10,-10,-10, // 7
 -10,-10,-10,-10,-10,-10,-10,-10  // 8
},{
// Black Pawns
// H   G   F   E   D   C   B   A
   0,  0,  0,  0,  0,  0,  0,  0, // 8
  80, 80, 80, 80, 80, 80, 80, 80, // 7
  50, 50, 50, 60, 60, 60, 60, 60, // 6
   0,  0,  0, 50, 50, 40, 40, 40, // 5
   0,  0,  0, 40, 40, 30, 30, 30, // 4
   0,  0,-10, 30, 30, 20, 20, 20, // 3
  20, 20, 20, 20, 20, 20, 20, 20, // 2
   0,  0,  0,  0,  0,  0,  0,  0  // 1
},{
// Black Knights
// H   G   F   E   D   C   B   A
 -50,-30,-10,-10,-10,-10,-30,-50, // 8
 -30,-10,  0,  0,  0,  0,-10,-30, // 7
 -10,  0, 20, 40, 40, 20,  0,-10, // 6
 -10,  0, 40, 50, 50, 40,  0,-10, // 5
 -10,  0, 40, 50, 50, 40,  0,-10, // 4
 -10,  0, 50, 40, 40, 20,  0,-10, // 3
 -30,-10,  0,  0,  0,  0,-10,-30, // 2
 -50,-30,-10,-10,-10,-10,-30,-50  // 1
},{
// Black Bishops
// H   G   F   E   D   C   B   A
 -10,-10,-10,-10,-10,-10,-10,-10, // 8
   0,  0,  0,  0,  0,  0,  0,  0, // 7
   0,  0,  0,  0,  0,  0,  0,  0, // 6
   0,  0,  0,  0,  0,  0,  0,  0, // 5
   0,  0,  0,  0,  0,  0,  0,  0, // 4
   0,  0,  0, 30, 30,  0,  0,  0, // 3
   0, 30,  0, 30, 30,  0, 30,  0, // 2
 -10,-10,-10,-10,-10,-10,-10,-10  // 1
},{
// Black Rooks
// H   G   F   E   D   C   B   A
   0,  0,  0,  0,  0,  0,  0,  0, // 8
  40, 40, 40, 40, 40, 40, 40, 40, // 7
   0,  0,  0,  0,  0,  0,  0,  0, // 6
   0,  0,  0,  0,  0,  0,  0,  0, // 5
   0,  0,  0,  0,  0,  0,  0,  0, // 4
   0,  0,  0,  0,  0,  0,  0,  0, // 3
   0,  0,  0,  0,  0,  0,  0,  0, // 2
  10,  0, 30, 30, 30, 30, 30, 30  // 1
},{
// Black Queens
// H   G   F   E   D   C   B   A
   0,  0,  0,  0,  0,  0,  0,  0, // 1
   0,  0,  0,  0,  0,  0,  0,  0, // 2
   0,  0,  0,  0,  0,  0,  0,  0, // 3
   0,  0,  0,  0,  0,  0,  0,  0, // 4
   0,  0,  0,  0,  0,  0,  0,  0, // 5
   0,  0,  0,  0,  0,  0,  0,  0, // 6
   0,  0,  0,  0,  0,  0,  0,  0, // 7
   0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// Black King
// H   G   F   E   D   C   B   A
 -10,-10,-10,-10,-10,-10,-10,-10, // 8
 -10,-10,-10,-10,-10,-10,-10,-10, // 7
 -10,-10,-10,-20,-20,-10,-10,-10, // 6
 -10,-10,-20,-30,-30,-20,-10,-10, // 5
 -10,-10,-20,-30,-30,-20,-10,-10, // 4
 -10,-10,-10,-20,-20,-10,-10,-10, // 3
   0,  0,  0,  0,  0,  0,  0,  0, // 2
  20, 50,  0,  0, 20, 30, 20,  0  // 1
}
};

const int bishop_pair_value = 75;
const int knight_pair_value = 50;

const int doubled_pawn_value = -50;

int eval(s_board* board)
{
  int score = 0;
  uint64_t pawns_in_col = 0;
  
  // Piece pairs
  if(board->pieces[wB] & (board->pieces[wB]-1)) {score += bishop_pair_value;}
  if(board->pieces[bB] & (board->pieces[bB]-1)) {score -= bishop_pair_value;}
  if(board->pieces[wN] & (board->pieces[wN]-1)) {score += knight_pair_value;}
  if(board->pieces[bN] & (board->pieces[bN]-1)) {score -= knight_pair_value;}
  
  // Doubled pawns - white
  if((pawns_in_col = U64_COL_A & (board->pieces[wP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score += doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_B & (board->pieces[wP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score += doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_C & (board->pieces[wP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score += doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_D & (board->pieces[wP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score += doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_E & (board->pieces[wP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score += doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_F & (board->pieces[wP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score += doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_G & (board->pieces[wP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score += doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_H & (board->pieces[wP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score += doubled_pawn_value;}
  }
  
  // Doubled pawns - black
  if((pawns_in_col = U64_COL_A & (board->pieces[bP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score -= doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_B & (board->pieces[bP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score -= doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_C & (board->pieces[bP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score -= doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_D & (board->pieces[bP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score -= doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_E & (board->pieces[bP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score -= doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_F & (board->pieces[bP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score -= doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_G & (board->pieces[bP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score -= doubled_pawn_value;}
  }
  if((pawns_in_col = U64_COL_H & (board->pieces[bP])))
  {
    if(pawns_in_col ^ (pawns_in_col & ~(pawns_in_col-1))) {score -= doubled_pawn_value;}
  }
  
  int sq;
  for(sq = 0; sq < 64; ++sq)
  {
    int p;
    
    if((board->pieces_colour[WHITE]>>sq)&1)
    {
      // White pieces
      for(p = 0; p < 6; ++p)
      {
        if((board->pieces[p]>>sq)&1)
        {
          score += piece_values[p];
          score += piece_location_bonus[p][sq];
          break;
        }
      }
    }
    else
    {
      int sq_reverse = 8*(7-(sq/8)) + sq%8;
     
      assert(sq_reverse >= 0);
      assert(sq_reverse <= 63);
      
      // Black pieces
      for(p = 6; p < 12; ++p)
      {
        if((board->pieces[p]>>sq)&1)
        {
          score += piece_values[p];
          score -= piece_location_bonus[p-6][sq_reverse];
          break;
        }
      }
    }
  }
  
  // We need this for negamax with alphabeta
  if(board->turn == WHITE)
    return score;
  else
    return -score;
  
  //return score;
}
