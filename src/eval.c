#include "defs.h"

const int piece_values[6] = {100, 100, 320, 330, 500, 900};

const int king_location_bonus[2][64] = {
{
// Kings - Mid game
// H   G   F   E   D   C   B   A
   20, 30, 10,  0,  0, 10, 30, 20,
   20, 20,  0,  0,  0,  0, 20, 20,
  -10,-20,-20,-20,-20,-20,-20,-10,
  -20,-30,-30,-40,-40,-30,-30,-20,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30
},{
// Kings - End game
// H   G   F   E   D   C   B   A
-50,-30,-30,-30,-30,-30,-30,-50,
-30,-30,  0,  0,  0,  0,-30,-30,
-30,-10, 20, 30, 30, 20,-10,-30,
-30,-10, 30, 40, 40, 30,-10,-30,
-30,-10, 30, 40, 40, 30,-10,-30,
-30,-10, 20, 30, 30, 20,-10,-30,
-30,-20,-10,  0,  0,-10,-20,-30,
-50,-40,-30,-20,-20,-30,-40,-50
}
};

const int piece_location_bonus[6][64] = {
{
// White Pawns
// H   G   F   E   D   C   B   A
   0,  0,  0,  0,  0,  0,  0,  0,
   5, 10, 10,-20,-20, 10, 10,  5,
   5, -5,-10,  0,  0,-10, -5,  5,
   0,  0,  0, 20, 20,  0,  0,  0,
   5,  5, 10, 25, 25, 10,  5,  5,
  10, 10, 20, 30, 30, 20, 10, 10,
  50, 50, 50, 50, 50, 50, 50, 50,
   0,  0,  0,  0,  0,  0,  0,  0
},{
// Black Pawns
// H   G   F   E   D   C   B   A
   0,  0,  0,  0,  0,  0,  0,  0,
   5, 10, 10,-20,-20, 10, 10,  5,
   5, -5,-10,  0,  0,-10, -5,  5,
   0,  0,  0, 20, 20,  0,  0,  0,
   5,  5, 10, 25, 25, 10,  5,  5,
  10, 10, 20, 30, 30, 20, 10, 10,
  50, 50, 50, 50, 50, 50, 50, 50,
   0,  0,  0,  0,  0,  0,  0,  0
},{
// Knights
// H   G   F   E   D   C   B   A
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50
},{
// Bishops
// H   G   F   E   D   C   B   A
  -20,-10,-10,-10,-10,-10,-10,-20, // 1
  -10,  5,  0,  0,  0,  0,  5,-10, // 2
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -20,-10,-10,-10,-10,-10,-10,-20
},{
// Rooks
// H   G   F   E   D   C   B   A
    0,  0,  0,  5,  5,  0,  0,  0,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
    5, 10, 10, 10, 10, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
},{
// Queens
// H   G   F   E   D   C   B   A
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -10,  5,  5,  5,  5,  5,  0,-10,
    0,  0,  5,  5,  5,  5,  0, -5,
   -5,  0,  5,  5,  5,  5,  0, -5,
  -10,  0,  5,  5,  5,  5,  0,-10,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
}
};

const int bishop_pair_value = 25;
const int knight_pair_value = 15;

const int doubled_pawn_value  = -25;
const int isolated_pawn_value = -50;

const int open_file_value = 25;

int is_endgame(s_board* board)
{
  assert(board != NULL);
  
  if(!board->combined[QUEENS])
  {
    return 1;
  }
  
  /*
  if(__builtin_popcountll(board->colour[WHITE]) < 5 && __builtin_popcountll(board->colour[BLACK]) < 5)
  {
    return 1;
  }
  */
  
  return 0;
}

int is_fifty_moves(s_board* board)
{
  assert(board != NULL);
  
  if(board->fifty_moves >= 100)
  {
    return 1;
  }
  
  return 0;
}

int is_threefold(s_board* board)
{
  assert(board != NULL);
  
  if(board->fifty_moves < 8)
  {
    return 0;
  }
  
  int repeats = 1;
  
  int i;
  for(i = 3; i <= board->fifty_moves && i < board->history_size - 1; i += 2)
  {
    assert(board->history_size-i-1 >= 0);
    
    if(board->key_history[board->history_size-i] == board->key)
    {
      repeats++;
      
      if(repeats >= 3)
      {
        return 1;
      }
    }
  }
  
  return 0;
}

const uint64_t files[8] = {U64_COL_A, U64_COL_B, U64_COL_C, U64_COL_D, U64_COL_E, U64_COL_F, U64_COL_G, U64_COL_H};
const uint64_t adj_files[8] = {U64_COL_B, U64_COL_A|U64_COL_C, U64_COL_B|U64_COL_D, U64_COL_C|U64_COL_E,
                               U64_COL_D|U64_COL_F, U64_COL_E|U64_COL_G, U64_COL_F|U64_COL_H, U64_COL_G};

int eval(s_board* board)
{
  int score = 0;
  
  // Piece pairs
  if((board->combined[BISHOPS] & board->colour[WHITE]) & ((board->combined[BISHOPS] & board->colour[WHITE])-1)) {score += bishop_pair_value;}
  if((board->combined[BISHOPS] & board->colour[BLACK]) & ((board->combined[BISHOPS] & board->colour[BLACK])-1)) {score -= bishop_pair_value;}
  if((board->combined[KNIGHTS] & board->colour[WHITE]) & ((board->combined[KNIGHTS] & board->colour[WHITE])-1)) {score += knight_pair_value;}
  if((board->combined[KNIGHTS] & board->colour[BLACK]) & ((board->combined[KNIGHTS] & board->colour[BLACK])-1)) {score -= knight_pair_value;}
  
  int colour;
  for(colour = WHITE; colour <= BLACK; colour++)
  {
    int file;
    for(file = 0; file <= 7; file++)
    {
      uint64_t col;
      
      // Check if doubled
      col = files[file] & board->colour[colour]&(board->combined[wP]|board->combined[bP]);
      if(col ^ (col & ~(col-1)))
      {
        score += doubled_pawn_value * ((colour == WHITE) ? 1 : -1);
      }
      
      // Check if isolated
      if(!(adj_files[file] & board->colour[colour]&(board->combined[wP]|board->combined[bP])))
      {
        score += isolated_pawn_value * ((colour == WHITE) ? 1 : -1);
      }
      
      // Rooks & Queens on open files
      col = files[file] & (board->combined[wP]|board->combined[bP]);
      
      if(!col && files[file] & ((board->combined[ROOKS] | board->combined[QUEENS]) & board->colour[colour]))
      {
        score += open_file_value * ((colour == WHITE) ? 1 : -1);
      }
    }
  }
  
  int sq;
  uint64_t copy;
  
  int piece_type;
  for(piece_type = 0; piece_type < 6; ++piece_type)
  {
    // WHITE
    copy = board->combined[piece_type] & board->colour[WHITE];
    while(copy)
    {
      sq = __builtin_ctzll(copy);
      
      score += piece_values[piece_type];
      score += piece_location_bonus[piece_type][sq];
      
      copy &= copy-1;
    }
    
    // BLACK
    copy = board->combined[piece_type] & board->colour[BLACK];
    while(copy)
    {
      sq = __builtin_ctzll(copy);
      int sq_reverse = sq^56;
      
      score -= piece_values[piece_type];
      score -= piece_location_bonus[piece_type][sq_reverse];
      
      copy &= copy-1;
    }
  }
  
  int endgame = is_endgame(board);
  
  // Kings
  sq = __builtin_ctzll(board->combined[KINGS]&board->colour[WHITE]);
  score += king_location_bonus[endgame][sq];
  
  sq = __builtin_ctzll(board->combined[KINGS]&board->colour[BLACK]);
  score += king_location_bonus[endgame][sq^56];
  
  if(board->turn == WHITE)
  {
    return score;
  }
  else
  {
    return -score;
  }
}
