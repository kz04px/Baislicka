#include "defs.h"

const int piece_values[5] = {100, 320, 330, 500, 900};

const int king_location_bonus[2][64] = {
{
// Kings - Mid game
//  A   B   C   D   E   F   G   H
   20, 30, 10,  0,  0, 10, 30, 20, // 1
   20, 20,  0,  0,  0,  0, 20, 20, // 2
  -10,-20,-20,-20,-20,-20,-20,-10, // 3
  -20,-30,-30,-40,-40,-30,-30,-20, // 4
  -30,-40,-40,-50,-50,-40,-40,-30, // 5
  -30,-40,-40,-50,-50,-40,-40,-30, // 6
  -30,-40,-40,-50,-50,-40,-40,-30, // 7
  -30,-40,-40,-50,-50,-40,-40,-30  // 8
},{
// Kings - End game
//  A   B   C   D   E   F   G   H
  -50,-30,-30,-30,-30,-30,-30,-50, // 1
  -30,-30,  0,  0,  0,  0,-30,-30, // 2
  -30,-10, 20, 30, 30, 20,-10,-30, // 3
  -30,-10, 30, 40, 40, 30,-10,-30, // 4
  -30,-10, 30, 40, 40, 30,-10,-30, // 5
  -30,-10, 20, 30, 30, 20,-10,-30, // 6
  -30,-20,-10,  0,  0,-10,-20,-30, // 7
  -50,-40,-30,-20,-20,-30,-40,-50  // 8
}
};

const int piece_location_bonus[5][64] = {
{
// Pawns
//  A   B   C   D   E   F   G   H
    0,  0,  0,  0,  0,  0,  0,  0, // 1
    5, 10, 10,-20,-20, 10, 10,  5, // 2
    5, -5,-10,  0,  0,-10, -5,  5, // 3
    0,  0,  0, 20, 20,  0,  0,  0, // 4
    5,  5, 10, 25, 25, 10,  5,  5, // 5
   10, 10, 20, 30, 30, 20, 10, 10, // 6
   50, 50, 50, 50, 50, 50, 50, 50, // 7
    0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// Knights
//  A   B   C   D   E   F   G   H
  -50,-40,-30,-30,-30,-30,-40,-50, // 1
  -40,-20,  0,  5,  5,  0,-20,-40, // 2
  -30,  5, 10, 15, 15, 10,  5,-30, // 3
  -30,  0, 15, 20, 20, 15,  0,-30, // 4
  -30,  5, 15, 20, 20, 15,  5,-30, // 5
  -30,  0, 10, 15, 15, 10,  0,-30, // 6
  -40,-20,  0,  0,  0,  0,-20,-40, // 7
  -50,-40,-30,-30,-30,-30,-40,-50  // 8
},{
// Bishops
//  A   B   C   D   E   F   G   H
  -20,-10,-10,-10,-10,-10,-10,-20, // 1
  -10,  5,  0,  0,  0,  0,  5,-10, // 2
  -10, 10, 10, 10, 10, 10, 10,-10, // 3
  -10,  0, 10, 10, 10, 10,  0,-10, // 4
  -10,  5,  5, 10, 10,  5,  5,-10, // 5
  -10,  0,  5, 10, 10,  5,  0,-10, // 6
  -10,  0,  0,  0,  0,  0,  0,-10, // 7
  -20,-10,-10,-10,-10,-10,-10,-20  // 8
},{
// Rooks
//  A   B   C   D   E   F   G   H
    0,  0,  0,  5,  5,  0,  0,  0, // 1
   -5,  0,  0,  0,  0,  0,  0, -5, // 2
   -5,  0,  0,  0,  0,  0,  0, -5, // 3
   -5,  0,  0,  0,  0,  0,  0, -5, // 4
   -5,  0,  0,  0,  0,  0,  0, -5, // 5
   -5,  0,  0,  0,  0,  0,  0, -5, // 6
    5, 10, 10, 10, 10, 10, 10,  5, // 7
    0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// Queens
//  A   B   C   D   E   F   G   H
  -20,-10,-10, -5, -5,-10,-10,-20, // 1
  -10,  0,  5,  0,  0,  0,  0,-10, // 2
  -10,  5,  5,  5,  5,  5,  0,-10, // 3
    0,  0,  5,  5,  5,  5,  0, -5, // 4
   -5,  0,  5,  5,  5,  5,  0, -5, // 5
  -10,  0,  5,  5,  5,  5,  0,-10, // 6
  -10,  0,  0,  0,  0,  0,  0,-10, // 7
  -20,-10,-10, -5, -5,-10,-10,-20  // 8
}
};

const int bishop_pair_value = 25;
const int knight_pair_value = 15;

const int doubled_pawn_value  = -10;
const int isolated_pawn_value = -20;

const int open_file_value = 25;

int is_endgame(s_board* board)
{
  assert(board != NULL);
  
  // Both sides have no queens
  if(!board->pieces[QUEENS])
  {
    return 1;
  }
  
  // Every side which has a queen has additionally no other pieces or one minorpiece maximum
  if(board->colour[WHITE]&board->pieces[QUEENS] && !(board->colour[WHITE] & (board->pieces[KNIGHTS] | board->pieces[BISHOPS] | board->pieces[ROOKS])))
  {
    return 1;
  }
  if(board->colour[BLACK]&board->pieces[QUEENS] && !(board->colour[BLACK] & (board->pieces[KNIGHTS] | board->pieces[BISHOPS] | board->pieces[ROOKS])))
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

int is_fifty_move_draw(s_board* board)
{
  assert(board != NULL);
  
  if(board->num_halfmoves >= 100)
  {
    return 1;
  }
  
  return 0;
}

int is_threefold(s_board* board)
{
  assert(board != NULL);
  
  if(board->num_halfmoves < 8)
  {
    return 0;
  }
  
  int repeats = 0;
  
  int lim = (board->num_halfmoves+1 < board->history_size) ? board->num_halfmoves+1 : board->history_size;
  
  int i;
  for(i = 1; i <= lim; ++i)
  {
    assert(board->history_size-i >= 0);
    
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

int insufficient_material(s_board* board)
{
  assert(board != NULL);
  
  // KP
  if(board->pieces[PAWNS])  {return 0;}
  // KQ
  if(board->pieces[QUEENS]) {return 0;}
  // KR
  if(board->pieces[ROOKS])  {return 0;}
  // KNB
  if(board->colour[WHITE]&board->pieces[KNIGHTS] && board->colour[WHITE]&board->pieces[BISHOPS]) {return 0;}
  if(board->colour[BLACK]&board->pieces[KNIGHTS] && board->colour[BLACK]&board->pieces[BISHOPS]) {return 0;}
  
  return 1;
}

const uint64_t files[8] = {U64_FILE_A, U64_FILE_B, U64_FILE_C, U64_FILE_D, U64_FILE_E, U64_FILE_F, U64_FILE_G, U64_FILE_H};
const uint64_t adj_files[8] = {U64_FILE_B, U64_FILE_A|U64_FILE_C, U64_FILE_B|U64_FILE_D, U64_FILE_C|U64_FILE_E,
                               U64_FILE_D|U64_FILE_F, U64_FILE_E|U64_FILE_G, U64_FILE_F|U64_FILE_H, U64_FILE_G};

int eval(s_board* board)
{
  int endgame = is_endgame(board);
  
  /*
  if(endgame && insufficient_material(board))
  {
    return 0;
  }
  */
  
  int score = 0;
  
  // Piece pairs
  if((board->pieces[BISHOPS] & board->colour[WHITE]) & ((board->pieces[BISHOPS] & board->colour[WHITE])-1)) {score += bishop_pair_value;}
  if((board->pieces[BISHOPS] & board->colour[BLACK]) & ((board->pieces[BISHOPS] & board->colour[BLACK])-1)) {score -= bishop_pair_value;}
  if((board->pieces[KNIGHTS] & board->colour[WHITE]) & ((board->pieces[KNIGHTS] & board->colour[WHITE])-1)) {score += knight_pair_value;}
  if((board->pieces[KNIGHTS] & board->colour[BLACK]) & ((board->pieces[KNIGHTS] & board->colour[BLACK])-1)) {score -= knight_pair_value;}
  
  int colour;
  for(colour = WHITE; colour <= BLACK; ++colour)
  {
    int i;
    for(i = 0; i <= 7; ++i)
    {
      uint64_t file;
      
      // Check if doubled
      file = files[i] & board->colour[colour] & board->pieces[PAWNS];
      if(file ^ (file & ~(file-1)))
      {
        score += doubled_pawn_value * ((colour == WHITE) ? 1 : -1);
      }
      
      // Check if isolated
      if(!(adj_files[i] & board->colour[colour] & board->pieces[PAWNS]))
      {
        score += isolated_pawn_value * ((colour == WHITE) ? 1 : -1);
      }
      
      // Rooks & Queens on open files
      file = files[i] & board->pieces[PAWNS];
      
      if(!file && (files[i] & ((board->pieces[ROOKS] | board->pieces[QUEENS]) & board->colour[colour])))
      {
        score += open_file_value * ((colour == WHITE) ? 1 : -1);
      }
    }
  }
  
  int sq;
  uint64_t copy;
  
  int piece_type;
  for(piece_type = 0; piece_type < 5; ++piece_type)
  {
    // WHITE
    copy = board->pieces[piece_type] & board->colour[WHITE];
    while(copy)
    {
      sq = __builtin_ctzll(copy);
      
      score += piece_values[piece_type];
      score += piece_location_bonus[piece_type][sq];
      
      copy &= copy-1;
    }
    
    // BLACK
    copy = board->pieces[piece_type] & board->colour[BLACK];
    while(copy)
    {
      sq = __builtin_ctzll(copy);
      
      score -= piece_values[piece_type];
      score -= piece_location_bonus[piece_type][sq^56];
      
      copy &= copy-1;
    }
  }
  
  // Kings
  sq = __builtin_ctzll(board->pieces[KINGS]&board->colour[WHITE]);
  score += king_location_bonus[endgame][sq];
  
  sq = __builtin_ctzll(board->pieces[KINGS]&board->colour[BLACK]);
  score -= king_location_bonus[endgame][sq^56];
  
  if(board->turn == WHITE)
  {
    return score;
  }
  else
  {
    return -score;
  }
}
