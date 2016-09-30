#include "defs.h"

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
 -30,-10,-10,-10,-10,-10,-10,-30, // 1
   0, 30,  0, 30, 30,  0, 30,  0, // 2
   0,  0,  0, 30, 30,  0,  0,  0, // 3
   0,  0,  0,  0,  0,  0,  0,  0, // 4
   0,  0,  0,  0,  0,  0,  0,  0, // 5
   0,  0,  0,  0,  0,  0,  0,  0, // 6
   0,  0,  0,  0,  0,  0,  0,  0, // 7
 -30,-10,-10,-10,-10,-10,-10,-30  // 8
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
 -30,-10,-10,-10,-10,-10,-10,-30, // 8
   0,  0,  0,  0,  0,  0,  0,  0, // 7
   0,  0,  0,  0,  0,  0,  0,  0, // 6
   0,  0,  0,  0,  0,  0,  0,  0, // 5
   0,  0,  0,  0,  0,  0,  0,  0, // 4
   0,  0,  0, 30, 30,  0,  0,  0, // 3
   0, 30,  0, 30, 30,  0, 30,  0, // 2
 -30,-10,-10,-10,-10,-10,-10,-30  // 1
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

const int bishop_pair_value = 25;
const int knight_pair_value = 15;

const int doubled_pawn_value  = -25;
const int isolated_pawn_value = -50;

const int open_file_value = 25;

int eval(s_board* board)
{
  int score = 0;
  
  // Piece pairs
  if((board->combined[BISHOPS] & board->colour[WHITE]) & ((board->combined[BISHOPS] & board->colour[WHITE])-1)) {score += bishop_pair_value;}
  if((board->combined[BISHOPS] & board->colour[BLACK]) & ((board->combined[BISHOPS] & board->colour[BLACK])-1)) {score -= bishop_pair_value;}
  if((board->combined[KNIGHTS] & board->colour[WHITE]) & ((board->combined[KNIGHTS] & board->colour[WHITE])-1)) {score += knight_pair_value;}
  if((board->combined[KNIGHTS] & board->colour[BLACK]) & ((board->combined[KNIGHTS] & board->colour[BLACK])-1)) {score -= knight_pair_value;}
  
  uint64_t white_pawns_col_a = U64_COL_A & (board->combined[wP]);
  uint64_t white_pawns_col_b = U64_COL_B & (board->combined[wP]);
  uint64_t white_pawns_col_c = U64_COL_C & (board->combined[wP]);
  uint64_t white_pawns_col_d = U64_COL_D & (board->combined[wP]);
  uint64_t white_pawns_col_e = U64_COL_E & (board->combined[wP]);
  uint64_t white_pawns_col_f = U64_COL_F & (board->combined[wP]);
  uint64_t white_pawns_col_g = U64_COL_G & (board->combined[wP]);
  uint64_t white_pawns_col_h = U64_COL_H & (board->combined[wP]);
  
  uint64_t black_pawns_col_a = U64_COL_A & (board->combined[bP]);
  uint64_t black_pawns_col_b = U64_COL_B & (board->combined[bP]);
  uint64_t black_pawns_col_c = U64_COL_C & (board->combined[bP]);
  uint64_t black_pawns_col_d = U64_COL_D & (board->combined[bP]);
  uint64_t black_pawns_col_e = U64_COL_E & (board->combined[bP]);
  uint64_t black_pawns_col_f = U64_COL_F & (board->combined[bP]);
  uint64_t black_pawns_col_g = U64_COL_G & (board->combined[bP]);
  uint64_t black_pawns_col_h = U64_COL_H & (board->combined[bP]);
  
  if(white_pawns_col_a)
  {
    // Check if doubled
    if(white_pawns_col_a ^ (white_pawns_col_a & ~(white_pawns_col_a-1)))
    {
      score += doubled_pawn_value;
    }
    
    // Check if isolated
    if(!white_pawns_col_b)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(white_pawns_col_b)
  {
    // Check if doubled
    if(white_pawns_col_b ^ (white_pawns_col_b & ~(white_pawns_col_b-1)))
    {
      score += doubled_pawn_value;
    }
    
    // Check if isolated
    if(!white_pawns_col_a && !white_pawns_col_c)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(white_pawns_col_c)
  {
    // Check if doubled
    if(white_pawns_col_c ^ (white_pawns_col_c & ~(white_pawns_col_c-1)))
    {
      score += doubled_pawn_value;
    }
    
    // Check if isolated
    if(!white_pawns_col_b && !white_pawns_col_d)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(white_pawns_col_d)
  {
    // Check if doubled
    if(white_pawns_col_d ^ (white_pawns_col_d & ~(white_pawns_col_d-1)))
    {
      score += doubled_pawn_value;
    }
    
    // Check if isolated
    if(!white_pawns_col_c && !white_pawns_col_e)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(white_pawns_col_e)
  {
    // Check if doubled
    if(white_pawns_col_e ^ (white_pawns_col_e & ~(white_pawns_col_e-1)))
    {
      score += doubled_pawn_value;
    }
    
    // Check if isolated
    if(!white_pawns_col_d && !white_pawns_col_f)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(white_pawns_col_f)
  {
    // Check if doubled
    if(white_pawns_col_f ^ (white_pawns_col_f & ~(white_pawns_col_f-1)))
    {
      score += doubled_pawn_value;
    }
    
    // Check if isolated
    if(!white_pawns_col_e && !white_pawns_col_g)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(white_pawns_col_g)
  {
    // Check if doubled
    if(white_pawns_col_g ^ (white_pawns_col_g & ~(white_pawns_col_g-1)))
    {
      score += doubled_pawn_value;
    }
    
    // Check if isolated
    if(!white_pawns_col_f && !white_pawns_col_h)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(white_pawns_col_h)
  {
    // Check if doubled
    if(white_pawns_col_h ^ (white_pawns_col_h & ~(white_pawns_col_h-1)))
    {
      score += doubled_pawn_value;
    }
    
    // Check if isolated
    if(!white_pawns_col_g)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(black_pawns_col_a)
  {
    // Check if doubled
    if(black_pawns_col_a ^ (black_pawns_col_a & ~(black_pawns_col_a-1)))
    {
      score -= doubled_pawn_value;
    }
    
    // Check if isolated
    if(!black_pawns_col_b)
    {
      score += isolated_pawn_value;
    }
  }
  
  if(black_pawns_col_b)
  {
    // Check if doubled
    if(black_pawns_col_b ^ (black_pawns_col_b & ~(black_pawns_col_b-1)))
    {
      score -= doubled_pawn_value;
    }
    
    // Check if isolated
    if(!black_pawns_col_a && !black_pawns_col_c)
    {
      score -= isolated_pawn_value;
    }
  }
  
  if(black_pawns_col_c)
  {
    // Check if doubled
    if(black_pawns_col_c ^ (black_pawns_col_c & ~(black_pawns_col_c-1)))
    {
      score -= doubled_pawn_value;
    }
    
    // Check if isolated
    if(!black_pawns_col_b && !black_pawns_col_d)
    {
      score -= isolated_pawn_value;
    }
  }
  
  if(black_pawns_col_d)
  {
    // Check if doubled
    if(black_pawns_col_d ^ (black_pawns_col_d & ~(black_pawns_col_d-1)))
    {
      score -= doubled_pawn_value;
    }
    
    // Check if isolated
    if(!black_pawns_col_c && !black_pawns_col_e)
    {
      score -= isolated_pawn_value;
    }
  }
  
  if(black_pawns_col_e)
  {
    // Check if doubled
    if(black_pawns_col_e ^ (black_pawns_col_e & ~(black_pawns_col_e-1)))
    {
      score -= doubled_pawn_value;
    }
    
    // Check if isolated
    if(!black_pawns_col_d && !black_pawns_col_f)
    {
      score -= isolated_pawn_value;
    }
  }
  
  if(black_pawns_col_f)
  {
    // Check if doubled
    if(black_pawns_col_f ^ (black_pawns_col_f & ~(black_pawns_col_f-1)))
    {
      score -= doubled_pawn_value;
    }
    
    // Check if isolated
    if(!black_pawns_col_e && !black_pawns_col_g)
    {
      score -= isolated_pawn_value;
    }
  }
  
  if(black_pawns_col_g)
  {
    // Check if doubled
    if(black_pawns_col_g ^ (black_pawns_col_g & ~(black_pawns_col_g-1)))
    {
      score -= doubled_pawn_value;
    }
    
    // Check if isolated
    if(!black_pawns_col_f && !black_pawns_col_h)
    {
      score -= isolated_pawn_value;
    }
  }
  
  if(black_pawns_col_h)
  {
    // Check if doubled
    if(black_pawns_col_h ^ (black_pawns_col_h & ~(black_pawns_col_h-1)))
    {
      score -= doubled_pawn_value;
    }
    
    // Check if isolated
    if(!black_pawns_col_g)
    {
      score -= isolated_pawn_value;
    }
  }
  
  // Rooks & Queens on open files
  if(!white_pawns_col_a && !black_pawns_col_a)
  {
    if(U64_COL_A & (board->combined[ROOKS] & board->colour[WHITE]) || U64_COL_A & (board->combined[QUEENS] & board->colour[WHITE])) {score += open_file_value;}
    if(U64_COL_A & (board->combined[ROOKS] & board->colour[BLACK]) || U64_COL_A & (board->combined[QUEENS] & board->colour[BLACK])) {score -= open_file_value;}
  }
  if(!white_pawns_col_b && !black_pawns_col_b)
  {
    if(U64_COL_B & (board->combined[ROOKS] & board->colour[WHITE]) || U64_COL_B & (board->combined[QUEENS] & board->colour[WHITE])) {score += open_file_value;}
    if(U64_COL_B & (board->combined[ROOKS] & board->colour[BLACK]) || U64_COL_B & (board->combined[QUEENS] & board->colour[BLACK])) {score -= open_file_value;}
  }
  if(!white_pawns_col_c && !black_pawns_col_c)
  {
    if(U64_COL_C & (board->combined[ROOKS] & board->colour[WHITE]) || U64_COL_C & (board->combined[QUEENS] & board->colour[WHITE])) {score += open_file_value;}
    if(U64_COL_C & (board->combined[ROOKS] & board->colour[BLACK]) || U64_COL_C & (board->combined[QUEENS] & board->colour[BLACK])) {score -= open_file_value;}
  }
  if(!white_pawns_col_d && !black_pawns_col_d)
  {
    if(U64_COL_D & (board->combined[ROOKS] & board->colour[WHITE]) || U64_COL_D & (board->combined[QUEENS] & board->colour[WHITE])) {score += open_file_value;}
    if(U64_COL_D & (board->combined[ROOKS] & board->colour[BLACK]) || U64_COL_D & (board->combined[QUEENS] & board->colour[BLACK])) {score -= open_file_value;}
  }
  if(!white_pawns_col_e && !black_pawns_col_e)
  {
    if(U64_COL_E & (board->combined[ROOKS] & board->colour[WHITE]) || U64_COL_E & (board->combined[QUEENS] & board->colour[WHITE])) {score += open_file_value;}
    if(U64_COL_E & (board->combined[ROOKS] & board->colour[BLACK]) || U64_COL_E & (board->combined[QUEENS] & board->colour[BLACK])) {score -= open_file_value;}
  }
  if(!white_pawns_col_f && !black_pawns_col_f)
  {
    if(U64_COL_F & (board->combined[ROOKS] & board->colour[WHITE]) || U64_COL_F & (board->combined[QUEENS] & board->colour[WHITE])) {score += open_file_value;}
    if(U64_COL_F & (board->combined[ROOKS] & board->colour[BLACK]) || U64_COL_F & (board->combined[QUEENS] & board->colour[BLACK])) {score -= open_file_value;}
  }
  if(!white_pawns_col_g && !black_pawns_col_g)
  {
    if(U64_COL_G & (board->combined[ROOKS] & board->colour[WHITE]) || U64_COL_G & (board->combined[QUEENS] & board->colour[WHITE])) {score += open_file_value;}
    if(U64_COL_G & (board->combined[ROOKS] & board->colour[BLACK]) || U64_COL_G & (board->combined[QUEENS] & board->colour[BLACK])) {score -= open_file_value;}
  }
  if(!white_pawns_col_h && !black_pawns_col_h)
  {
    if(U64_COL_H & (board->combined[ROOKS] & board->colour[WHITE]) || U64_COL_H & (board->combined[QUEENS] & board->colour[WHITE])) {score += open_file_value;}
    if(U64_COL_H & (board->combined[ROOKS] & board->colour[BLACK]) || U64_COL_H & (board->combined[QUEENS] & board->colour[BLACK])) {score -= open_file_value;}
  }
  
//       0   1   2   3   4   5   6   7   8   9  10  11
//enum {wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK, EMPTY};
  
  int sq;
  for(sq = 0; sq < 64; ++sq)
  {
    uint64_t pos = (uint64_t)1<<sq;
    
    if(board->colour[WHITE]&pos)
    {
      if(board->combined[wP]&pos)
      {
        score += piece_values[0];
        score += piece_location_bonus[0][sq];
      }
      else if(board->combined[KNIGHTS]&pos)
      {
        score += piece_values[1];
        score += piece_location_bonus[1][sq];
      }
      else if(board->combined[BISHOPS]&pos)
      {
        score += piece_values[2];
        score += piece_location_bonus[2][sq];
      }
      else if(board->combined[ROOKS]&pos)
      {
        score += piece_values[3];
        score += piece_location_bonus[3][sq];
      }
      else if(board->combined[QUEENS]&pos)
      {
        score += piece_values[4];
        score += piece_location_bonus[4][sq];
      }
      else if(board->combined[KINGS]&pos)
      {
        score += piece_values[5];
        score += piece_location_bonus[5][sq];
      }
    }
    else
    {
      int sq_reverse = 8*(7-(sq/8)) + sq%8;
     
      assert(sq_reverse >= 0);
      assert(sq_reverse <= 63);
      
      if(board->combined[bP]&pos)
      {
        score += piece_values[6];
        score -= piece_location_bonus[0][sq_reverse];
      }
      else if(board->combined[KNIGHTS]&pos)
      {
        score += piece_values[7];
        score -= piece_location_bonus[1][sq_reverse];
      }
      else if(board->combined[BISHOPS]&pos)
      {
        score += piece_values[8];
        score -= piece_location_bonus[2][sq_reverse];
      }
      else if(board->combined[ROOKS]&pos)
      {
        score += piece_values[9];
        score -= piece_location_bonus[3][sq_reverse];
      }
      else if(board->combined[QUEENS]&pos)
      {
        score += piece_values[10];
        score -= piece_location_bonus[4][sq_reverse];
      }
      else if(board->combined[KINGS]&pos)
      {
        score += piece_values[11];
        score -= piece_location_bonus[5][sq_reverse];
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
