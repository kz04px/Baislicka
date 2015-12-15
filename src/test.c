#include "defs.h"

int test_find_moves_wP(s_board *board, s_move *moves)
{
	int num_moves = 0;
	U64 pos = 0;
	U64 moves1 = (board->pieces[wP]<<8)&(~board->pieces_all);  // Up 1
	U64 moves2 = ((moves1&U64_ROW_3)<<8)&(~board->pieces_all); // Up 2
	U64 moves3 = (board->pieces[wP]<<7)&(board->pieces_colour[BLACK])&(~U64_COL_A); // Up 1 right 1
	U64 moves4 = (board->pieces[wP]<<9)&(board->pieces_colour[BLACK])&(~U64_COL_H); // Up 1 left 1
	
	// En passant
	if((board->pieces[wP]<<7) & (~U64_COL_A) & (board->ep))
	{
		moves[num_moves] = move_add(board, (board->ep)>>7, board->ep, EP, wP);
		num_moves++;
	}
	if((board->pieces[wP]<<9) & (~U64_COL_H) & (board->ep))
	{
		moves[num_moves] = move_add(board, (board->ep)>>9, board->ep, EP, wP);
		num_moves++;
	}
	
	while((pos = (moves1 & ~(moves1-1))))
	{
		if(pos&U64_ROW_8)
		{
			moves[num_moves  ] = add_promotion_move(board, pos>>8, pos, wP, wQ);
			moves[num_moves+1] = add_promotion_move(board, pos>>8, pos, wP, wR);
			moves[num_moves+2] = add_promotion_move(board, pos>>8, pos, wP, wB);
			moves[num_moves+3] = add_promotion_move(board, pos>>8, pos, wP, wN);
			num_moves += 4;
		}
		else
		{
			moves[num_moves] = move_add(board, pos>>8, pos, NORMAL, wP);
			num_moves++;
		}
		moves1 = moves1^pos;
	}
	while((pos = (moves2 & ~(moves2-1))))
	{
		moves[num_moves] = move_add(board, pos>>16, pos, DOUBLE_MOVE, wP);
		num_moves++;
		moves2 = moves2^pos;
	}
	while((pos = (moves3 & ~(moves3-1))))
	{
		if(pos&U64_ROW_8)
		{
			moves[num_moves  ] = add_promotion_move(board, pos>>7, pos, wP, wQ);
			moves[num_moves+1] = add_promotion_move(board, pos>>7, pos, wP, wR);
			moves[num_moves+2] = add_promotion_move(board, pos>>7, pos, wP, wB);
			moves[num_moves+3] = add_promotion_move(board, pos>>7, pos, wP, wN);
			num_moves += 4;
		}
		else
		{
			moves[num_moves] = move_add(board, pos>>7, pos, CAPTURE, wP);
			num_moves++;
		}
		moves3 = moves3^pos;
	}
	while((pos = (moves4 & ~(moves4-1))))
	{
		if(pos&U64_ROW_8)
		{
			moves[num_moves  ] = add_promotion_move(board, pos>>9, pos, wP, wQ);
			moves[num_moves+1] = add_promotion_move(board, pos>>9, pos, wP, wR);
			moves[num_moves+2] = add_promotion_move(board, pos>>9, pos, wP, wB);
			moves[num_moves+3] = add_promotion_move(board, pos>>9, pos, wP, wN);
			num_moves += 4;
		}
		else
		{
			moves[num_moves] = move_add(board, pos>>9, pos, CAPTURE, wP);
			num_moves++;
		}
		moves4 = moves4^pos;
	}
	return num_moves;
}

int test_find_moves_bP(s_board *board, s_move *moves)
{
	int num_moves = 0;
	U64 pos = 0;
	U64 moves1 = (board->pieces[bP]>>8)&(~board->pieces_all);  // Down 1
	U64 moves2 = ((moves1&U64_ROW_6)>>8)&(~board->pieces_all); // Down 2
	U64 moves3 = (board->pieces[bP]>>7)&(board->pieces_colour[WHITE])&(~U64_COL_H); // Down 1 left 1
	U64 moves4 = (board->pieces[bP]>>9)&(board->pieces_colour[WHITE])&(~U64_COL_A); // Down 1 right 1
	
	// En passant
	if((board->pieces[bP]>>7) & (~U64_COL_H) & (board->ep))
	{
		moves[num_moves] = move_add(board, (board->ep)<<7, board->ep, EP, bP);
		num_moves++;
	}
	if((board->pieces[bP]>>9) & (~U64_COL_A) & (board->ep))
	{
		moves[num_moves] = move_add(board, (board->ep)<<9, board->ep, EP, bP);
		num_moves++;
	}
	
	while((pos = (moves1 & ~(moves1-1))))
	{
		if(pos&U64_ROW_1)
		{
			moves[num_moves  ] = add_promotion_move(board, pos<<8, pos, bP, bQ);
			moves[num_moves+1] = add_promotion_move(board, pos<<8, pos, bP, bR);
			moves[num_moves+2] = add_promotion_move(board, pos<<8, pos, bP, bB);
			moves[num_moves+3] = add_promotion_move(board, pos<<8, pos, bP, bN);
			num_moves += 4;
		}
		else
		{
			moves[num_moves] = move_add(board, pos<<8, pos, NORMAL, bP);
			num_moves++;
		}
		moves1 = moves1^pos;
	}
	while((pos = (moves2 & ~(moves2-1))))
	{
		moves[num_moves] = move_add(board, pos<<16, pos, DOUBLE_MOVE, bP);
		num_moves++;
		moves2 = moves2^pos;
	}
	while((pos = (moves3 & ~(moves3-1))))
	{
		if(pos&U64_ROW_1)
		{
			moves[num_moves  ] = add_promotion_move(board, pos<<7, pos, bP, bQ);
			moves[num_moves+1] = add_promotion_move(board, pos<<7, pos, bP, bR);
			moves[num_moves+2] = add_promotion_move(board, pos<<7, pos, bP, bB);
			moves[num_moves+3] = add_promotion_move(board, pos<<7, pos, bP, bN);
			num_moves += 4;
		}
		else
		{
			moves[num_moves] = move_add(board, pos<<7, pos, CAPTURE, bP);
			num_moves++;
		}
		moves3 = moves3^pos;
	}
	while((pos = (moves4 & ~(moves4-1))))
	{
		if(pos&U64_ROW_1)
		{
			moves[num_moves  ] = add_promotion_move(board, pos<<9, pos, bP, bQ);
			moves[num_moves+1] = add_promotion_move(board, pos<<9, pos, bP, bR);
			moves[num_moves+2] = add_promotion_move(board, pos<<9, pos, bP, bB);
			moves[num_moves+3] = add_promotion_move(board, pos<<9, pos, bP, bN);
			num_moves += 4;
		}
		else
		{
			moves[num_moves] = move_add(board, pos<<9, pos, CAPTURE, bP);
			num_moves++;
		}
		moves4 = moves4^pos;
	}
	return num_moves;
}

int test_find_moves_knight(s_board *board, s_move *moves, int colour)
{
	int num_moves = 0;
	U64 pos = 0;
	int type;
	int other_colour;
	if(colour == WHITE)
	{
		type = wN;
		other_colour = BLACK;
	}
	else
	{
		type = bN;
		other_colour = WHITE;
	}
	
	U64 moves1 = (board->pieces[type]<<17) & (~board->pieces_colour[colour]) & (~U64_COL_H); // Up 2 left 1
	U64 moves2 = (board->pieces[type]<<15) & (~board->pieces_colour[colour]) & (~U64_COL_A); // Up 2 right 1
	U64 moves3 = (board->pieces[type]>>17) & (~board->pieces_colour[colour]) & (~U64_COL_A); // Down 2 right 1
	U64 moves4 = (board->pieces[type]>>15) & (~board->pieces_colour[colour]) & (~U64_COL_H); // Down 2 left 1
	U64 moves5 = (board->pieces[type]<<10) & (~board->pieces_colour[colour]) & ~(U64_COL_G|U64_COL_H); // Left 2 up 1
	U64 moves6 = (board->pieces[type]>>6)  & (~board->pieces_colour[colour]) & ~(U64_COL_G|U64_COL_H); // Left 2 down 1
	U64 moves7 = (board->pieces[type]<<6)  & (~board->pieces_colour[colour]) & ~(U64_COL_A|U64_COL_B); // Right 2 up 1
	U64 moves8 = (board->pieces[type]>>10) & (~board->pieces_colour[colour]) & ~(U64_COL_A|U64_COL_B); // Right 2 down 1
	
	while((pos = (moves1 & ~(moves1-1))))
	{
		if(board->pieces_colour[other_colour]&pos)
		{
			moves[num_moves] = move_add(board, pos>>17, pos, CAPTURE, type);
		}
		else
		{
			moves[num_moves] = move_add(board, pos>>17, pos, NORMAL, type);
		}
		num_moves++;
		moves1 = moves1^pos;
	}
	
	while((pos = (moves2 & ~(moves2-1))))
	{
		if(board->pieces_colour[other_colour]&pos)
		{
			moves[num_moves] = move_add(board, pos>>15, pos, CAPTURE, type);
		}
		else
		{
			moves[num_moves] = move_add(board, pos>>15, pos, NORMAL, type);
		}
		num_moves++;
		moves2 = moves2^pos;
	}
	while((pos = (moves3 & ~(moves3-1))))
	{
		if(board->pieces_colour[other_colour]&pos)
		{
			moves[num_moves] = move_add(board, pos<<17, pos, CAPTURE, type);
		}
		else
		{
			moves[num_moves] = move_add(board, pos<<17, pos, NORMAL, type);
		}
		num_moves++;
		moves3 = moves3^pos;
	}
	while((pos = (moves4 & ~(moves4-1))))
	{
		if(board->pieces_colour[other_colour]&pos)
		{
			moves[num_moves] = move_add(board, pos<<15, pos, CAPTURE, type);
		}
		else
		{
			moves[num_moves] = move_add(board, pos<<15, pos, NORMAL, type);
		}
		num_moves++;
		moves4 = moves4^pos;
	}
	while((pos = (moves5 & ~(moves5-1))))
	{
		if(board->pieces_colour[other_colour]&pos)
		{
			moves[num_moves] = move_add(board, pos>>10, pos, CAPTURE, type);
		}
		else
		{
			moves[num_moves] = move_add(board, pos>>10, pos, NORMAL, type);
		}
		num_moves++;
		moves5 = moves5^pos;
	}
	while((pos = (moves6 & ~(moves6-1))))
	{
		if(board->pieces_colour[other_colour]&pos)
		{
			moves[num_moves] = move_add(board, pos<<6, pos, CAPTURE, type);
		}
		else
		{
			moves[num_moves] = move_add(board, pos<<6, pos, NORMAL, type);
		}
		num_moves++;
		moves6 = moves6^pos;
	}
	while((pos = (moves7 & ~(moves7-1))))
	{
		if(board->pieces_colour[other_colour]&pos)
		{
			moves[num_moves] = move_add(board, pos>>6, pos, CAPTURE, type);
		}
		else
		{
			moves[num_moves] = move_add(board, pos>>6, pos, NORMAL, type);
		}
		num_moves++;
		moves7 = moves7^pos;
	}
	while((pos = (moves8 & ~(moves8-1))))
	{
		if(board->pieces_colour[other_colour]&pos)
		{
			moves[num_moves] = move_add(board, pos<<10, pos, CAPTURE, type);
		}
		else
		{
			moves[num_moves] = move_add(board, pos<<10, pos, NORMAL, type);
		}
		num_moves++;
		moves8 = moves8^pos;
	}
	
	return num_moves;
}

int find_diagonal_moves(s_board *board, s_move *moves, int colour, int type)
{
	int num_moves = 0;
	U64 to = 0;
	U64 from = 0;
	U64 pos = board->pieces[type];
	int opposite_colour = WHITE;
	if(colour == WHITE)
	{
		opposite_colour = BLACK;
	}
	
	while((from = (pos & ~(pos-1))))
	{
		// Up 1 left 1
		to = (from<<9)&(~U64_COL_H);
		while(to)
		{
			if(to&board->pieces_all)
			{
				if(to&board->pieces_colour[opposite_colour])
				{
					moves[num_moves] = move_add(board, from, to, CAPTURE, type);
					num_moves++;
				}
				break;
			}
			else
			{
				moves[num_moves] = move_add(board, from, to, NORMAL, type);
				num_moves++;
			}
			
			to = (to<<9)&(~U64_COL_H);
		}
		
		// Up 1 right 1
		to = (from<<7)&(~U64_COL_A);
		while(to)
		{
			if(to&board->pieces_all)
			{
				if(to&board->pieces_colour[opposite_colour])
				{
					moves[num_moves] = move_add(board, from, to, CAPTURE, type);
					num_moves++;
				}
				break;
			}
			else
			{
				moves[num_moves] = move_add(board, from, to, NORMAL, type);
				num_moves++;
			}
			
			to = (to<<7)&(~U64_COL_A);
		}
		
		// Down 1 left 1
		to = (from>>7)&(~U64_COL_H);
		while(to)
		{
			if(to&board->pieces_all)
			{
				if(to&board->pieces_colour[opposite_colour])
				{
					moves[num_moves] = move_add(board, from, to, CAPTURE, type);
					num_moves++;
				}
				break;
			}
			else
			{
				moves[num_moves] = move_add(board, from, to, NORMAL, type);
				num_moves++;
			}
			
			to = (to>>7)&(~U64_COL_H);
		}
		
		// Down 1 right 1
		to = (from>>9)&(~U64_COL_A);
		while(to)
		{
			if(to&board->pieces_all)
			{
				if(to&board->pieces_colour[opposite_colour])
				{
					moves[num_moves] = move_add(board, from, to, CAPTURE, type);
					num_moves++;
				}
				break;
			}
			else
			{
				moves[num_moves] = move_add(board, from, to, NORMAL, type);
				num_moves++;
			}
			
			to = (to>>9)&(~U64_COL_A);
		}
		
		pos = pos^from;
	}
	
	return num_moves;
}

int find_horizontal_moves(s_board *board, s_move *moves, int colour, int type)
{
	int num_moves = 0;
	U64 to = 0;
	U64 from = 0;
	U64 pos = board->pieces[type];
	int opposite_colour = WHITE;
	if(colour == WHITE)
	{
		opposite_colour = BLACK;
	}
	
	while((from = (pos & ~(pos-1))))
	{
		// Up
		to = from<<8;
		while(to)
		{
			if(to&board->pieces_all)
			{
				if(to&board->pieces_colour[opposite_colour])
				{
					moves[num_moves] = move_add(board, from, to, CAPTURE, type);
					num_moves++;
				}
				break;
			}
			else
			{
				moves[num_moves] = move_add(board, from, to, NORMAL, type);
				num_moves++;
			}
			
			to = to<<8;
		}
		
		// Down
		to = from>>8;
		while(to)
		{
			if(to&board->pieces_all)
			{
				if(to&board->pieces_colour[opposite_colour])
				{
					moves[num_moves] = move_add(board, from, to, CAPTURE, type);
					num_moves++;
				}
				break;
			}
			else
			{
				moves[num_moves] = move_add(board, from, to, NORMAL, type);
				num_moves++;
			}
			
			to = to>>8;
		}
		
		// Left
		to = (from>>1)&(~U64_COL_A);
		while(to)
		{
			if(to&board->pieces_all)
			{
				if(to&board->pieces_colour[opposite_colour])
				{
					moves[num_moves] = move_add(board, from, to, CAPTURE, type);
					num_moves++;
				}
				break;
			}
			else
			{
				moves[num_moves] = move_add(board, from, to, NORMAL, type);
				num_moves++;
			}
			
			to = (to>>1)&(~U64_COL_A);
		}
		
		// Right
		to = (from<<1)&(~U64_COL_H);
		while(to)
		{
			if(to&board->pieces_all)
			{
				if(to&board->pieces_colour[opposite_colour])
				{
					moves[num_moves] = move_add(board, from, to, CAPTURE, type);
					num_moves++;
				}
				break;
			}
			else
			{
				moves[num_moves] = move_add(board, from, to, NORMAL, type);
				num_moves++;
			}
			
			to = (to<<1)&(~U64_COL_H);
		}
		
		pos = pos^from;
	}
	
	return num_moves;
}

int test_find_moves_wN(s_board *board, s_move *moves)
{
	return test_find_moves_knight(board, moves, WHITE);
}

int test_find_moves_bN(s_board *board, s_move *moves)
{
	return test_find_moves_knight(board, moves, BLACK);
}

int test_find_moves_wB(s_board *board, s_move *moves)
{
	return find_diagonal_moves(board, moves, WHITE, wB);
}

int test_find_moves_bB(s_board *board, s_move *moves)
{
	return find_diagonal_moves(board, moves, BLACK, bB);
}

int test_find_moves_wR(s_board *board, s_move *moves)
{
	return find_horizontal_moves(board, moves, WHITE, wR);
}

int test_find_moves_bR(s_board *board, s_move *moves)
{
	return find_horizontal_moves(board, moves, BLACK, bR);
}

int test_find_moves_wQ(s_board *board, s_move *moves)
{
	int num_moves = 0;
	num_moves += find_diagonal_moves(board, moves, WHITE, wQ);
	num_moves += find_horizontal_moves(board, &moves[num_moves], WHITE, wQ);
	return num_moves;
}

int test_find_moves_bQ(s_board *board, s_move *moves)
{
	int num_moves = 0;
	num_moves += find_diagonal_moves(board, moves, BLACK, bQ);
	num_moves += find_horizontal_moves(board, &moves[num_moves], BLACK, bQ);
	return num_moves;
}

int test_find_moves_wK(s_board *board, s_move *moves)
{
	int num_moves = 0;
	U64 from = board->pieces[wK];
	
	// wKSC
	if(board->castling[wKSC] == TRUE &&
	   GETBIT(board->pieces_all, F1) == FALSE &&
	   GETBIT(board->pieces_all, G1) == FALSE &&
	   calculate_attacked(board, U64_E1, BLACK) == FALSE &&
	   calculate_attacked(board, U64_F1, BLACK) == FALSE &&
	   calculate_attacked(board, U64_G1, BLACK) == FALSE)
	   //black_attacking(board, U64_E1) == FALSE &&
	   //black_attacking(board, U64_F1) == FALSE &&
	   //black_attacking(board, U64_G1) == FALSE)
	{
		moves[num_moves] = move_add(board, from, from>>2, wKSC, wK);
		num_moves++;
	}
	
	// wQSC
	if(board->castling[wQSC] == TRUE &&
	   GETBIT(board->pieces_all, D1) == FALSE &&
	   GETBIT(board->pieces_all, C1) == FALSE &&
	   GETBIT(board->pieces_all, B1) == FALSE &&
	   calculate_attacked(board, U64_E1, BLACK) == FALSE &&
	   calculate_attacked(board, U64_D1, BLACK) == FALSE &&
	   calculate_attacked(board, U64_C1, BLACK) == FALSE)
	   //black_attacking(board, U64_E1) == FALSE &&
	   //black_attacking(board, U64_D1) == FALSE &&
	   //black_attacking(board, U64_C1) == FALSE)
	{
		moves[num_moves] = move_add(board, from, from<<2, wQSC, wK);
		num_moves++;
	}
	
	// Up 1
	if(board->pieces[wK]<<8)
	{
		if((board->pieces[wK]<<8) & (board->pieces_colour[BLACK]))
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]<<8, CAPTURE, wK);
			num_moves++;
		}
		else if((board->pieces[wK]<<8) & (board->pieces_colour[WHITE]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]<<8, NORMAL, wK);
			num_moves++;
		}
	}
	
	// Up 1 right 1
	if((board->pieces[wK]<<7)&(~U64_COL_A))
	{
		if((board->pieces[wK]<<7) & (board->pieces_colour[BLACK]))
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]<<7, CAPTURE, wK);
			num_moves++;
		}
		else if((board->pieces[wK]<<7) & (board->pieces_colour[WHITE]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]<<7, NORMAL, wK);
			num_moves++;
		}
	}
	
	// Up 1 left 1
	if((board->pieces[wK]<<9)&(~U64_COL_H))
	{
		if((board->pieces[wK]<<9) & (board->pieces_colour[BLACK]))
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]<<9, CAPTURE, wK);
			num_moves++;
		}
		else if((board->pieces[wK]<<9) & (board->pieces_colour[WHITE]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]<<9, NORMAL, wK);
			num_moves++;
		}
	}
	
	// Down 1
	if(board->pieces[wK]>>8)
	{
		if((board->pieces[wK]>>8) & (board->pieces_colour[BLACK]))
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]>>8, CAPTURE, wK);
			num_moves++;
		}
		else if((board->pieces[wK]>>8) & (board->pieces_colour[WHITE]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]>>8, NORMAL, wK);
			num_moves++;
		}
	}
	
	// Down 1 left 1
	if((board->pieces[wK]>>7)&(~U64_COL_H))
	{
		if((board->pieces[wK]>>7) & (board->pieces_colour[BLACK]))
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]>>7, CAPTURE, wK);
			num_moves++;
		}
		else if((board->pieces[wK]>>7) & (board->pieces_colour[WHITE]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]>>7, NORMAL, wK);
			num_moves++;
		}
	}
	
	// Down 1 right 1
	if((board->pieces[wK]>>9)&(~U64_COL_A))
	{
		if((board->pieces[wK]>>9) & (board->pieces_colour[BLACK]))
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]>>9, CAPTURE, wK);
			num_moves++;
		}
		else if((board->pieces[wK]>>9) & (board->pieces_colour[WHITE]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]>>9, NORMAL, wK);
			num_moves++;
		}
	}
	
	// Left 1
	if((board->pieces[wK]<<1)&(~U64_COL_H))
	{
		if((board->pieces[wK]<<1) & (board->pieces_colour[BLACK]))
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]<<1, CAPTURE, wK);
			num_moves++;
		}
		else if((board->pieces[wK]<<1) & (board->pieces_colour[WHITE]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]<<1, NORMAL, wK);
			num_moves++;
		}
	}
	
	// Right 1
	if((board->pieces[wK]>>1)&(~U64_COL_A))
	{
		if((board->pieces[wK]>>1) & (board->pieces_colour[BLACK]))
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]>>1, CAPTURE, wK);
			num_moves++;
		}
		else if((board->pieces[wK]>>1) & (board->pieces_colour[WHITE]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[wK], board->pieces[wK]>>1, NORMAL, wK);
			num_moves++;
		}
	}
	return num_moves;
}

int test_find_moves_bK(s_board *board, s_move *moves)
{
	int num_moves = 0;
	U64 from = board->pieces[bK];
	
	// bKSC
	if(board->castling[bKSC] == TRUE &&
	   GETBIT(board->pieces_all, F8) == FALSE &&
	   GETBIT(board->pieces_all, G8) == FALSE &&
	   calculate_attacked(board, U64_E8, WHITE) == FALSE &&
	   calculate_attacked(board, U64_F8, WHITE) == FALSE &&
	   calculate_attacked(board, U64_G8, WHITE) == FALSE)
	   //white_attacking(board, U64_E8) == FALSE &&
	   //white_attacking(board, U64_F8) == FALSE &&
	   //white_attacking(board, U64_G8) == FALSE)
	{
		moves[num_moves] = move_add(board, from, from>>2, bKSC, bK);
		num_moves++;
	}
	
	// bQSC
	if(board->castling[bQSC] == TRUE &&
	   GETBIT(board->pieces_all, D8) == FALSE &&
	   GETBIT(board->pieces_all, C8) == FALSE &&
	   GETBIT(board->pieces_all, B8) == FALSE &&
	   calculate_attacked(board, U64_E8, WHITE) == FALSE &&
	   calculate_attacked(board, U64_D8, WHITE) == FALSE &&
	   calculate_attacked(board, U64_C8, WHITE) == FALSE)
	   //white_attacking(board, U64_E8) == FALSE &&
	   //white_attacking(board, U64_D8) == FALSE &&
	   //white_attacking(board, U64_C8) == FALSE)
	{
		moves[num_moves] = move_add(board, from, from<<2, bQSC, bK);
		num_moves++;
	}
	
	// Up 1
	if(board->pieces[bK]<<8)
	{
		if((board->pieces[bK]<<8) & (board->pieces_colour[WHITE]))
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]<<8, CAPTURE, bK);
			num_moves++;
		}
		else if((board->pieces[bK]<<8) & (board->pieces_colour[BLACK]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]<<8, NORMAL, bK);
			num_moves++;
		}
	}
	
	// Up 1 right 1
	if((board->pieces[bK]<<7)&(~U64_COL_A))
	{
		if((board->pieces[bK]<<7) & (board->pieces_colour[WHITE]))
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]<<7, CAPTURE, bK);
			num_moves++;
		}
		else if((board->pieces[bK]<<7) & (board->pieces_colour[BLACK]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]<<7, NORMAL, bK);
			num_moves++;
		}
	}
	
	// Up 1 left 1
	if((board->pieces[bK]<<9)&(~U64_COL_H))
	{
		if((board->pieces[bK]<<9) & (board->pieces_colour[WHITE]))
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]<<9, CAPTURE, bK);
			num_moves++;
		}
		else if((board->pieces[bK]<<9) & (board->pieces_colour[BLACK]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]<<9, NORMAL, bK);
			num_moves++;
		}
	}
	
	// Down 1
	if(board->pieces[bK]>>8)
	{
		if((board->pieces[bK]>>8) & (board->pieces_colour[WHITE]))
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]>>8, CAPTURE, bK);
			num_moves++;
		}
		else if((board->pieces[bK]>>8) & (board->pieces_colour[BLACK]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]>>8, NORMAL, bK);
			num_moves++;
		}
	}
	
	// Down 1 left 1
	if((board->pieces[bK]>>7)&(~U64_COL_H))
	{
		if((board->pieces[bK]>>7) & (board->pieces_colour[WHITE]))
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]>>7, CAPTURE, bK);
			num_moves++;
		}
		else if((board->pieces[bK]>>7) & (board->pieces_colour[BLACK]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]>>7, NORMAL, bK);
			num_moves++;
		}
	}
	
	// Down 1 right 1
	if((board->pieces[bK]>>9)&(~U64_COL_A))
	{
		if((board->pieces[bK]>>9) & (board->pieces_colour[WHITE]))
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]>>9, CAPTURE, bK);
			num_moves++;
		}
		else if((board->pieces[bK]>>9) & (board->pieces_colour[BLACK]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]>>9, NORMAL, bK);
			num_moves++;
		}
	}
	
	// Left 1
	if((board->pieces[bK]<<1)&(~U64_COL_H))
	{
		if((board->pieces[bK]<<1) & (board->pieces_colour[WHITE]))
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]<<1, CAPTURE, bK);
			num_moves++;
		}
		else if((board->pieces[bK]<<1) & (board->pieces_colour[BLACK]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]<<1, NORMAL, bK);
			num_moves++;
		}
	}
	
	// Right 1
	if((board->pieces[bK]>>1)&(~U64_COL_A))
	{
		if((board->pieces[bK]>>1) & (board->pieces_colour[WHITE]))
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]>>1, CAPTURE, bK);
			num_moves++;
		}
		else if((board->pieces[bK]>>1) & (board->pieces_colour[BLACK]))
		{
		}
		else
		{
			moves[num_moves] = move_add(board, board->pieces[bK], board->pieces[bK]>>1, NORMAL, bK);
			num_moves++;
		}
	}
	return num_moves;
}

int test_find_white_moves(s_board *board, s_move *moves)
{
  int num_moves = 0;
  num_moves += test_find_moves_wP(board, moves);
  num_moves += test_find_moves_wN(board, &moves[num_moves]);
  num_moves += test_find_moves_wB(board, &moves[num_moves]);
  num_moves += test_find_moves_wR(board, &moves[num_moves]);
  num_moves += test_find_moves_wQ(board, &moves[num_moves]);
  num_moves += test_find_moves_wK(board, &moves[num_moves]);
  return num_moves;
}

int test_find_black_moves(s_board *board, s_move *moves)
{
  int num_moves = 0;
  num_moves += test_find_moves_bP(board, moves);
  num_moves += test_find_moves_bN(board, &moves[num_moves]);
  num_moves += test_find_moves_bB(board, &moves[num_moves]);
  num_moves += test_find_moves_bR(board, &moves[num_moves]);
  num_moves += test_find_moves_bQ(board, &moves[num_moves]);
  num_moves += test_find_moves_bK(board, &moves[num_moves]);
  return num_moves;
}

int test_find_moves(s_board *board, s_move *moves)
{
	int num_moves = 0;
	if(board->turn == WHITE)
	{
    num_moves += test_find_white_moves(board, moves);
	}
	else
	{
    num_moves += test_find_black_moves(board, moves);
	}
	return num_moves;
}