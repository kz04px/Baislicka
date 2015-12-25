#include "defs.h"

#define INF 999999

const int piece_values[12] = { 100,  300,  300,  500,  900,  INF,
                              -100, -300, -300, -500, -900, -INF};

const int wKSC_value =  75;
const int wQSC_value =  50;
const int bKSC_value = -75;
const int bQSC_value = -50;

const int bishop_pair_value = 75;
const int knight_pair_value = 50;

int eval(s_board* board)
{
  int score = 0;
  
  // King safety
       if(U64_G1 & board->pieces[wK]) {score += wKSC_value;}
  else if(U64_C1 & board->pieces[wK]) {score += wQSC_value;}
       if(U64_G8 & board->pieces[bK]) {score += bKSC_value;}
  else if(U64_C8 & board->pieces[bK]) {score += bQSC_value;}
  
  // Piece pairs
  if(board->pieces[wB] & (board->pieces[wB]-1)) {score += bishop_pair_value;}
  if(board->pieces[bB] & (board->pieces[bB]-1)) {score -= bishop_pair_value;}
  if(board->pieces[wN] & (board->pieces[wN]-1)) {score += knight_pair_value;}
  if(board->pieces[bN] & (board->pieces[bN]-1)) {score -= knight_pair_value;}
  
  int sq;
  for(sq = 0; sq < 64; ++sq)
  {
    int p;
    
    // Sum piece values
    for(p = 0; p < 12; ++p)
    {
      if((board->pieces[p]>>sq)&1)
      {
        score += piece_values[p];
        break;
      }
    }
  }
  
  return score;
}