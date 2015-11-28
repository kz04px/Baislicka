#include "defs.h"

// Return value is flipped for colour being evaluated
int evaluate(s_board *board)
{
	int score = 0;
	
	// Castling
	/*
	     if(U64_G1 & board->pieces[wK]) {score += 2;}
	else if(U64_C1 & board->pieces[wK]) {score += 1;}
	     if(U64_G8 & board->pieces[bK]) {score += -2;}
	else if(U64_C8 & board->pieces[bK]) {score += -1;}
	*/
	
	U64 pos = 1;
	while(pos)
	{
		if(pos&board->all_pieces[WHITE])
		{
			score += 1;
		}
		else if(pos&board->all_pieces[BLACK])
		{
			score += -1;
		}
		
		pos = pos<<1;
	}
	
	/*
	if(board->pieces[wN]&U64_H3)
	{
		score += 3;
	}
	*/
	
	if(board->turn == WHITE)
	{
		return score;
	}
	else
	{
		return -score;
	}
}