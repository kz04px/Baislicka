#include "defs.h"

int in_check(s_board *board, int colour)
{
	ASSERT(board != NULL);
	ASSERT(colour == WHITE || colour == BLACK);

	if(colour == WHITE)
	{
		return calculate_attacked(board, board->pieces[wK], BLACK);
	}
	else
	{
		return calculate_attacked(board, board->pieces[bK], WHITE);
	}
}

int calculate_attacked(s_board *board, U64 sq, int attacking_colour)
{	
	U64 pos;
	if(attacking_colour == WHITE)
	{
		// Pawns
		if((board->pieces[wP] >> 9) & (~U64_COL_H) & (sq)) {return TRUE;} // Up 1 left 1
		if((board->pieces[wP] >> 7) & (~U64_COL_A) & (sq)) {return TRUE;} // Up 1 right 1
		// Knights
		if((board->pieces[wN] >> 17) & (~U64_COL_H) & (sq))      {return TRUE;} // Up 2 left 1
		else if((board->pieces[wN] >> 15) & (~U64_COL_A) & (sq)) {return TRUE;} // Up 2 right 1
		else if((board->pieces[wN] << 17) & (~U64_COL_A) & (sq)) {return TRUE;} // Down 2 right 1
		else if((board->pieces[wN] << 15) & (~U64_COL_H) & (sq)) {return TRUE;} // Down 2 left 1
		else if((board->pieces[wN] >> 10) & ~(U64_COL_G|U64_COL_H) & (sq)) {return TRUE;} // Left 2 up 1
		else if((board->pieces[wN] << 6)  & ~(U64_COL_G|U64_COL_H) & (sq)) {return TRUE;} // Left 2 down 1
		else if((board->pieces[wN] >> 6)  & ~(U64_COL_A|U64_COL_B) & (sq)) {return TRUE;} // Right 2 up 1
		else if((board->pieces[wN] << 10) & ~(U64_COL_A|U64_COL_B) & (sq)) {return TRUE;} // Right 2 down 1
		// Diagonals (Bishops & Queens)
		pos = (sq >> 9) & (~U64_COL_H); // Up 1 left 1
		while(pos)
		{
			if(pos&board->pieces[wB]) {return TRUE;}
			if(pos&board->pieces[wQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos >> 9) & (~U64_COL_H);
		}
		pos = (sq >> 7) & (~U64_COL_A); // Up 1 right 1
		while(pos)
		{
			if(pos&board->pieces[wB]) {return TRUE;}
			if(pos&board->pieces[wQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos >> 7) & (~U64_COL_A);
		}
		pos = (sq << 7) & (~U64_COL_H); // Down 1 left 1
		while(pos)
		{
			if(pos&board->pieces[wB]) {return TRUE;}
			if(pos&board->pieces[wQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos << 7) & (~U64_COL_H);
		}
		pos = (sq << 9) & (~U64_COL_A); // Down 1 right 1
		while(pos)
		{
			if(pos&board->pieces[wB]) {return TRUE;}
			if(pos&board->pieces[wQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos << 9) & (~U64_COL_A);
		}
		// Horizontal & Vertical (Rooks & Queens)
		pos = (sq << 1) & (~U64_COL_A); // Right
		while(pos)
		{
			if(pos&board->pieces[wR]) {return TRUE;}
			if(pos&board->pieces[wQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos << 1) & (~U64_COL_A);
		}
		pos = (sq >> 1) & (~U64_COL_H); // Left
		while(pos)
		{
			if(pos&board->pieces[wR]) {return TRUE;}
			if(pos&board->pieces[wQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos >> 1) & (~U64_COL_H);
		}
		pos = (sq >> 8); // Up
		while(pos)
		{
			if(pos&board->pieces[wR]) {return TRUE;}
			if(pos&board->pieces[wQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos >> 8);
		}
		pos = (sq << 8); // Down
		while(pos)
		{
			if(pos&board->pieces[wR]) {return TRUE;}
			if(pos&board->pieces[wQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos << 8);
		}
		// King
		if((sq>>8) & (board->pieces[wK])) {return TRUE;} // Up
		if((sq<<8) & (board->pieces[wK])) {return TRUE;} // Down
		if((sq>>9) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} // Up 1   left 1
		if((sq>>1) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} //        left 1
		if((sq<<7) & (board->pieces[wK]) & (~U64_COL_H)) {return TRUE;} // Down 1 left 1
		if((sq>>7) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} // Up 1   right 1
		if((sq<<1) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} //        right 1
		if((sq<<9) & (board->pieces[wK]) & (~U64_COL_A)) {return TRUE;} // Down 1 right 1
	}
	else
	{
		// Pawns
		if((board->pieces[bP] << 7) & (~U64_COL_H) & (sq)) {return TRUE;} // Down 1 left 1
		if((board->pieces[bP] << 9) & (~U64_COL_A) & (sq)) {return TRUE;} // Down 1 right 1
		// Knights
		if((board->pieces[bN] >> 17) & (~U64_COL_H) & (sq))      {return TRUE;} // Up 2 left 1
		else if((board->pieces[bN] >> 15) & (~U64_COL_A) & (sq)) {return TRUE;} // Up 2 right 1
		else if((board->pieces[bN] << 17) & (~U64_COL_A) & (sq)) {return TRUE;} // Down 2 right 1
		else if((board->pieces[bN] << 15) & (~U64_COL_H) & (sq)) {return TRUE;} // Down 2 left 1
		else if((board->pieces[bN] >> 10) & ~(U64_COL_G|U64_COL_H) & (sq)) {return TRUE;} // Left 2 up 1
		else if((board->pieces[bN] << 6)  & ~(U64_COL_G|U64_COL_H) & (sq)) {return TRUE;} // Left 2 down 1
		else if((board->pieces[bN] >> 6)  & ~(U64_COL_A|U64_COL_B) & (sq)) {return TRUE;} // Right 2 up 1
		else if((board->pieces[bN] << 10) & ~(U64_COL_A|U64_COL_B) & (sq)) {return TRUE;} // Right 2 down 1
		// Diagonals (Bishops & Queens)
		pos = (sq >> 9) & (~U64_COL_H); // Up 1 left 1
		while(pos)
		{
			if(pos&board->pieces[bB]) {return TRUE;}
			if(pos&board->pieces[bQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos >> 9) & (~U64_COL_H);
		}
		pos = (sq >> 7) & (~U64_COL_A); // Up 1 right 1
		while(pos)
		{
			if(pos&board->pieces[bB]) {return TRUE;}
			if(pos&board->pieces[bQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos >> 7) & (~U64_COL_A);
		}
		pos = (sq << 7) & (~U64_COL_H); // Down 1 left 1
		while(pos)
		{
			if(pos&board->pieces[bB]) {return TRUE;}
			if(pos&board->pieces[bQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos << 7) & (~U64_COL_H);
		}
		pos = (sq << 9) & (~U64_COL_A); // Down 1 right 1
		while(pos)
		{
			if(pos&board->pieces[bB]) {return TRUE;}
			if(pos&board->pieces[bQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos << 9) & (~U64_COL_A);
		}
		// Horizontal & Vertical (Rooks & Queens)
		pos = (sq << 1) & (~U64_COL_A); // Right
		while(pos)
		{
			if(pos&board->pieces[bR]) {return TRUE;}
			if(pos&board->pieces[bQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos << 1) & (~U64_COL_A);
		}
		pos = (sq >> 1) & (~U64_COL_H); // Left
		while(pos)
		{
			if(pos&board->pieces[bR]) {return TRUE;}
			if(pos&board->pieces[bQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos >> 1) & (~U64_COL_H);
		}
		pos = (sq >> 8); // Up
		while(pos)
		{
			if(pos&board->pieces[bR]) {return TRUE;}
			if(pos&board->pieces[bQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos >> 8);
		}
		pos = (sq << 8); // Down
		while(pos)
		{
			if(pos&board->pieces[bR]) {return TRUE;}
			if(pos&board->pieces[bQ]) {return TRUE;}
			if(pos&board->all_pieces[BOTH]) {break;}
			pos = (pos << 8);
		}
		// King
		if((sq>>8) & (board->pieces[bK])) {return TRUE;} // Up
		if((sq<<8) & (board->pieces[bK])) {return TRUE;} // Down
		if((sq>>9) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} // Up 1   left 1
		if((sq>>1) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} //        left 1
		if((sq<<7) & (board->pieces[bK]) & (~U64_COL_H)) {return TRUE;} // Down 1 left 1
		if((sq>>7) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} // Up 1   right 1
		if((sq<<1) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} //        right 1
		if((sq<<9) & (board->pieces[bK]) & (~U64_COL_A)) {return TRUE;} // Down 1 right 1
	}
	
	return FALSE;
}

int white_attacking(s_board *board, U64 positions)
{
	if(board->all_attacking[WHITE]&positions)
		{return TRUE;}
	return FALSE;
}

int black_attacking(s_board *board, U64 positions)
{
	if(board->all_attacking[BLACK]&positions)
		{return TRUE;}
	return FALSE;
}

void update_wP(s_board *board)
{
	board->attacking[wP] = (board->pieces[wP]>>7)&(~U64_COL_A); // Up 1 right 1
	board->attacking[wP] |= (board->pieces[wP]>>9)&(~U64_COL_H); // UP 1 left 1
}

void update_bP(s_board *board)
{
	board->attacking[bP] = (board->pieces[bP]<<7)&(~U64_COL_H); // Down 1 right 1
	board->attacking[bP] |= (board->pieces[bP]<<9)&(~U64_COL_A); // Down 1 left 1
}

void update_wN(s_board *board)
{
	board->attacking[wN]  = (board->pieces[wN]>>17) & (~U64_COL_H); // Up 2 left 1
	board->attacking[wN] |= (board->pieces[wN]>>15) & (~U64_COL_A); // Up 2 right 1
	board->attacking[wN] |= (board->pieces[wN]<<17) & (~U64_COL_A); // Down 2 right 1
	board->attacking[wN] |= (board->pieces[wN]<<15) & (~U64_COL_H); // Down 2 left 1
	board->attacking[wN] |= (board->pieces[wN]>>10) & ~(U64_COL_G|U64_COL_H); // Left 2 up 1
	board->attacking[wN] |= (board->pieces[wN]<<6)  & ~(U64_COL_G|U64_COL_H); // Left 2 down 1
	board->attacking[wN] |= (board->pieces[wN]>>6)  & ~(U64_COL_A|U64_COL_B); // Right 2 up 1
	board->attacking[wN] |= (board->pieces[wN]<<10) & ~(U64_COL_A|U64_COL_B); // Right 2 down 1
}

void update_bN(s_board *board)
{
	board->attacking[bN]  = (board->pieces[bN]>>17) & (~U64_COL_H); // Up 2 left 1
	board->attacking[bN] |= (board->pieces[bN]>>15) & (~U64_COL_A); // Up 2 right 1
	board->attacking[bN] |= (board->pieces[bN]<<17) & (~U64_COL_A); // Down 2 right 1
	board->attacking[bN] |= (board->pieces[bN]<<15) & (~U64_COL_H); // Down 2 left 1
	board->attacking[bN] |= (board->pieces[bN]>>10) & ~(U64_COL_G|U64_COL_H); // Left 2 up 1
	board->attacking[bN] |= (board->pieces[bN]<<6)  & ~(U64_COL_G|U64_COL_H); // Left 2 down 1
	board->attacking[bN] |= (board->pieces[bN]>>6)  & ~(U64_COL_A|U64_COL_B); // Right 2 up 1
	board->attacking[bN] |= (board->pieces[bN]<<10) & ~(U64_COL_A|U64_COL_B); // Right 2 down 1
}

void update_diagonal(s_board *board, int piece, int colour)
{
	U64 pos;
	
	pos = board->pieces[piece]; // Up 1 right 1
	do
	{
		pos = (pos>>7)&(~U64_COL_A);
		board->attacking[piece] |= pos;
		pos = pos & (~board->all_pieces[BOTH]);
	}
	while(pos);
	
	pos = board->pieces[piece]; // Up 1 left 1
	do
	{
		pos = (pos>>9)&(~U64_COL_H);
		board->attacking[piece] |= pos;
		pos = pos & (~board->all_pieces[BOTH]);
	}
	while(pos);
	
	pos = board->pieces[piece]; // Down 1 right 1
	do
	{
		pos = (pos<<9)&(~U64_COL_A);
		board->attacking[piece] |= pos;
		pos = pos & (~board->all_pieces[BOTH]);
	}
	while(pos);
	
	pos = board->pieces[piece]; // Down 1 left 1
	do
	{
		pos = (pos<<9)&(~U64_COL_H);
		board->attacking[piece] |= pos;
		pos = pos & (~board->all_pieces[BOTH]);
	}
	while(pos);
}

void update_horizontal(s_board *board, int piece, int colour)
{
	U64 pos;
	
	pos = board->pieces[piece]; // Up 1
	do
	{
		pos = pos>>8;
		board->attacking[piece] |= pos;
		pos = pos & (~board->all_pieces[BOTH]);
	}
	while(pos);
	
	pos = board->pieces[piece]; // Down 1
	do
	{
		pos = pos<<8;
		board->attacking[piece] |= pos;
		pos = pos & (~board->all_pieces[BOTH]);
	}
	while(pos);
	
	pos = board->pieces[piece]; // Right 1
	do
	{
		pos = (pos<<1)&(~U64_COL_A);
		board->attacking[piece] |= pos;
		pos = pos & (~board->all_pieces[BOTH]);
	}
	while(pos);
	
	pos = board->pieces[piece]; // Left 1
	do
	{
		pos = (pos>>1)&(~U64_COL_H);
		board->attacking[piece] |= pos;
		pos = pos & (~board->all_pieces[BOTH]);
	}
	while(pos);
}

void update_wB(s_board *board)
{
	board->attacking[wB] = 0;
	update_diagonal(board, wB, WHITE);
}

void update_bB(s_board *board)
{
	board->attacking[bB] = 0;
	update_diagonal(board, bB, BLACK);
}

void update_wR(s_board *board)
{
	board->attacking[wR] = 0;
	update_horizontal(board, wR, WHITE);
}

void update_bR(s_board *board)
{
	board->attacking[bR] = 0;
	update_horizontal(board, bR, BLACK);
}

void update_wQ(s_board *board)
{
	board->attacking[wQ] = 0;
	update_diagonal(board, wQ, WHITE);
	update_horizontal(board, wQ, WHITE);
}

void update_bQ(s_board *board)
{
	board->attacking[bQ] = 0;
	update_diagonal(board, bQ, BLACK);
	update_horizontal(board, bQ, BLACK);
}

void update_wK(s_board *board)
{
	board->attacking[wK] = 0;
	board->attacking[wK] |= ((board->pieces[wK])>>8); // Up 1
	board->attacking[wK] |= ((board->pieces[wK])<<8); // Down 1
	board->attacking[wK] |= ((board->pieces[wK])>>9) & (~U64_COL_H); // Up 1   left  1
	board->attacking[wK] |= ((board->pieces[wK])>>1) & (~U64_COL_H); //        left  1
	board->attacking[wK] |= ((board->pieces[wK])<<7) & (~U64_COL_H); // Down 1 left  1
	board->attacking[wK] |= ((board->pieces[wK])>>7) & (~U64_COL_A); // Up 1   right 1
	board->attacking[wK] |= ((board->pieces[wK])<<1) & (~U64_COL_A); //        right 1
	board->attacking[wK] |= ((board->pieces[wK])<<9) & (~U64_COL_A); // Down 1 right 1
}

void update_bK(s_board *board)
{
	board->attacking[bK] = 0;
	board->attacking[bK] |= ((board->pieces[bK])>>8); // Up 1
	board->attacking[bK] |= ((board->pieces[bK])<<8); // Down 1
	board->attacking[bK] |= ((board->pieces[bK])>>9) & (~U64_COL_H); // Up 1   left  1
	board->attacking[bK] |= ((board->pieces[bK])>>1) & (~U64_COL_H); //        left  1
	board->attacking[bK] |= ((board->pieces[bK])<<7) & (~U64_COL_H); // Down 1 left  1
	board->attacking[bK] |= ((board->pieces[bK])>>7) & (~U64_COL_A); // Up 1   right 1
	board->attacking[bK] |= ((board->pieces[bK])<<1) & (~U64_COL_A); //        right 1
	board->attacking[bK] |= ((board->pieces[bK])<<9) & (~U64_COL_A); // Down 1 right 1
}

void update_white_attacking(s_board *board)
{
	update_wP(board);
	update_wN(board);
	update_wB(board);
	update_wR(board);
	update_wQ(board);
	update_wK(board);
	
	board->all_attacking[WHITE]	= board->attacking[wP] | board->attacking[wN] | board->attacking[wB] |
	                              board->attacking[wR] | board->attacking[wQ] | board->attacking[wK];
}

void update_black_attacking(s_board *board)
{
	update_bP(board);
	update_bN(board);
	update_bB(board);
	update_bR(board);
	update_bQ(board);
	update_bK(board);
	
	board->all_attacking[BLACK]	= board->attacking[bP] | board->attacking[bN] | board->attacking[bB] |
	                              board->attacking[bR] | board->attacking[bQ] | board->attacking[bK];
}

void update_attacking(s_board *board)
{
	update_white_attacking(board);
	update_black_attacking(board);
	//board->all_attacking[BOTH] = board->all_attacking[WHITE] | board->all_attacking[BLACK];
}