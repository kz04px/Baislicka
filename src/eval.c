#include "defs.h"

const int piece_values[7] = {100, 100, 300, 300, 500, 900, INF};

const int piece_location_bonus[7][64] = {
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
// Black Pawns
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
// Knights
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
// Bishops
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
// Rooks
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
// Queens
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
// Kings
// H   G   F   E   D   C   B   A
  20, 50,  0,  0, 20, 30, 20,  0, // 1
   0,  0,  0,  0,  0,  0,  0,  0, // 2
 -10,-10,-10,-20,-20,-10,-10,-10, // 3
 -10,-10,-20,-30,-30,-20,-10,-10, // 4
 -10,-10,-20,-30,-30,-20,-10,-10, // 5
 -10,-10,-10,-20,-20,-10,-10,-10, // 6
 -10,-10,-10,-10,-10,-10,-10,-10, // 7
 -10,-10,-10,-10,-10,-10,-10,-10  // 8
}
};

const int bishop_pair_value = 25;
const int knight_pair_value = 15;

const int doubled_pawn_value  = -25;
const int isolated_pawn_value = -50;

const int open_file_value = 25;

const uint64_t files[8] = { U64_COL_A, U64_COL_B, U64_COL_C, U64_COL_D, U64_COL_E, U64_COL_F, U64_COL_G, U64_COL_H };
const uint64_t adj_files[8] = { U64_COL_B, U64_COL_A|U64_COL_C, U64_COL_B|U64_COL_D, U64_COL_C|U64_COL_E,
                                U64_COL_D|U64_COL_F, U64_COL_E|U64_COL_G, U64_COL_F|U64_COL_H, U64_COL_G };

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

  int colour, file;
  uint64_t col, lcol, rcol;

  for(colour = WHITE; colour <= BLACK; colour++) {
    for(file = 0; file <= 7; file++) {
      // hack: wP == WHITE, bP == BLACK
      col = files[file] & board->combined[colour];

      // Check if doubled
      if(col ^ (col & ~(col-1))
      {
        score += doubled_pawn_value * (colour == WHITE) ? +1 : -1;
      }

      // Check if isolated
      if(!(adj_files[file] & board->combined[colour]))
      {
        score += isolated_pawn_value * (colour == WHITE) ? +1 : -1;
      }

      // Rooks & Queens on open files
      col = files[file] & board->combined[wP] & board->combined[bP];

      if(!col && files[file] & ((board->combined[ROOKS] | board->combined[QUEENS]) & board->colour[colour]))
      {
        score += open_file_value * (colour == WHITE) ? +1 : -1;
      }
    }
  }

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

  int sq;
  uint64_t copy;
  uint64_t from;

  int piece_type;
  for(piece_type = 0; piece_type < 7; ++piece_type)
  {
    // WHITE
    copy = board->combined[piece_type] & board->colour[WHITE];
    while(copy)
    {
      sq = __builtin_ctzll(copy);

      score += piece_values[piece_type];
      score += piece_location_bonus[piece_type][sq];

      copy &= (copy-1);
    }

    // BLACK
    copy = board->combined[piece_type] & board->colour[BLACK];
    while(copy)
    {
      sq = __builtin_ctzll(copy);
      int sq_reverse = sq^56;

      score -= piece_values[piece_type];
      score -= piece_location_bonus[piece_type][sq_reverse];

      copy &= (copy-1);
    }
  }

  if(board->turn == WHITE)
  {
    return score;
  }
  else
  {
    return -score;
  }
}
