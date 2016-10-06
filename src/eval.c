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

int is_endgame(s_board* board)
{
  assert(board != NULL);
  
  if(__builtin_popcount(board->colour[WHITE]) < 5 && __builtin_popcount(board->colour[BLACK]) < 5)
  {
    return 1;
  }
  
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
  //assert(board->history_size >= board->fifty_moves);
  
  if(board->fifty_moves < 8)
  {
    return 0;
  }
  
  int repeats = 1;
  
  int i;
  //for(i = board->history_size-3; i >= 0; i -= 2)
  for(i = 3; i <= board->fifty_moves; i += 2)
  {
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
  for(piece_type = 0; piece_type < 7; ++piece_type)
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
  
  if(board->turn == WHITE)
  {
    return score;
  }
  else
  {
    return -score;
  }
}
