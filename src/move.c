#include "defs.h"
	
int move_print(s_move *our_move)
{
	printf("%c%c%c%c ", U64_to_Col(our_move->from)+'a', U64_to_Row(our_move->from)+'1',
	                    U64_to_Col(our_move->to)+'a', U64_to_Row(our_move->to)+'1');

	switch(our_move->type)
	{
		case NORMAL:
			printf("Move\n");
			break;
		case CAPTURE:
			printf("Capture\n");
			break;
		case DOUBLE_MOVE:
			printf("Double move\n");
			break;
		case wKSC:
			printf("wKSC\n");
			break;
		case wQSC:
			printf("wQSC\n");
			break;
		case bKSC:
			printf("bKSC\n");
			break;
		case bQSC:
			printf("bQSC\n");
			break;
		case PROMOTE:
			printf("Promotion\n");
			break;
		case EP:
			printf("En Passant\n");
			break;
	}
	return 0;
}

s_move add_promotion_move(s_board *board, U64 from, U64 to, int piece_type, int promo_piece)
{
	int taken = EMPTY;
	int n;
	for(n = 0; n < 12; ++n)
	{
		if(board->pieces[n]&to) {taken = n; break;}
	}
	
	s_move move;
	move.from = from;
	move.to = to;
	move.type = PROMOTE;
	move.taken = taken;
	move.piece_type = piece_type;
	move.promotion = promo_piece;
	return move;
}

s_move add_move(s_board *board, U64 from, U64 to, int type, int piece_type)
{
	int taken = EMPTY;
	if(type == CAPTURE || type == PROMOTE)
	{
		int n;
		for(n = 0; n < 12; ++n)
		{
			if(board->pieces[n]&to) {taken = n; break;}
		}
	}
	else if(type == EP)
	{
		if(piece_type == wP)
		{
			taken = bP;
		}
		else
		{
			taken = wP;
		}
	}
	
	s_move move;
	move.from = from;
	move.to = to;
	move.type = type;
	move.taken = taken;
	move.piece_type = piece_type;
	return move;
}

void make_move(s_board *board, s_move *move)
{
	// set old permissions
	move->ep_old = board->ep;
	move->wKSC_old = board->castling[wKSC];
	move->wQSC_old = board->castling[wQSC];
	move->bKSC_old = board->castling[bKSC];
	move->bQSC_old = board->castling[bQSC];
	board->ep = 0;
	
	if((move->from) & U64_E1) // White king move
	{
		board->castling[wQSC] = FALSE;
		board->castling[wKSC] = FALSE;
	}
	else if((move->to&U64_A1) || (move->from&U64_A1)) // A1 move
	{
		board->castling[wQSC] = FALSE;
	}
	else if((move->to&U64_H1) || (move->from&U64_H1)) // H1 move
	{
		board->castling[wKSC] = FALSE;
	}
	
	if(move->from & U64_E8) // Black king move
	{
		board->castling[bQSC] = FALSE;
		board->castling[bKSC] = FALSE;
	}
	else if((move->to&U64_A8) || (move->from&U64_A8)) // A8 move
	{
		board->castling[bQSC] = FALSE;
	}
	else if((move->to&U64_H8) || (move->from&U64_H8)) // H8 move
	{
		board->castling[bKSC] = FALSE;
	}
	
	switch(move->type)
	{
		case NORMAL:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->piece_type] ^= move->to;
			break;
		case CAPTURE:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->piece_type] ^= move->to;
			board->pieces[move->taken] ^= move->to;
			break;
		case DOUBLE_MOVE:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->piece_type] ^= move->to;
			// Add ep square
			if(move->piece_type == wP)
			{
				board->ep = (move->to)<<8;
			}
			else
			{
				board->ep = (move->to)>>8;
			}			
			break;
		case PROMOTE:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->promotion] ^= move->to;
			if(move->taken != EMPTY)
			{
				board->pieces[move->taken] ^= move->to;
			}
			break;
		case EP:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->piece_type] ^= move->to;
			if(move->piece_type == wP)
			{
				board->pieces[bP] ^= (move->to)<<8;
			}
			else
			{
				board->pieces[wP] ^= (move->to)>>8;
			}
			break;
		case wKSC:
			board->pieces[wK] ^= U64_E1;
			board->pieces[wK] ^= U64_G1;
			board->pieces[wR] ^= U64_H1;
			board->pieces[wR] ^= U64_F1;
			break;
		case wQSC:
			board->pieces[wK] ^= U64_E1;
			board->pieces[wK] ^= U64_C1;
			board->pieces[wR] ^= U64_A1;
			board->pieces[wR] ^= U64_D1;
			break;
		case bKSC:
			board->pieces[bK] ^= U64_E8;
			board->pieces[bK] ^= U64_G8;
			board->pieces[bR] ^= U64_H8;
			board->pieces[bR] ^= U64_F8;
			break;
		case bQSC:
			board->pieces[bK] ^= U64_E8;
			board->pieces[bK] ^= U64_C8;
			board->pieces[bR] ^= U64_A8;
			board->pieces[bR] ^= U64_D8;
			break;
	}
	
	board->all_pieces[WHITE] = board->pieces[wP] |
	                           board->pieces[wN] |
	                           board->pieces[wB] |
	                           board->pieces[wR] |
	                           board->pieces[wQ] |
	                           board->pieces[wK];
	board->all_pieces[BLACK] = board->pieces[bP] |
	                           board->pieces[bN] |
	                           board->pieces[bB] |
	                           board->pieces[bR] |
	                           board->pieces[bQ] |
	                           board->pieces[bK];
	board->all_pieces[BOTH] = board->all_pieces[WHITE]|board->all_pieces[BLACK];

	board->turn = 1-(board->turn);
}

void undo_move(s_board *board, s_move *move)
{  
	switch(move->type)
	{
		case NORMAL:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->piece_type] ^= move->to;
			break;
		case CAPTURE:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->piece_type] ^= move->to;
			board->pieces[move->taken] ^= move->to;
			break;
		case DOUBLE_MOVE:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->piece_type] ^= move->to;
			break;
		case PROMOTE:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->promotion] ^= move->to;
			if(move->taken != EMPTY)
			{
				board->pieces[move->taken] ^= move->to;
			}
			break;
		case EP:
			board->pieces[move->piece_type] ^= move->from;
			board->pieces[move->piece_type] ^= move->to;
			if(move->piece_type == wP)
			{
				board->pieces[bP] ^= (move->to)<<8;
			}
			else
			{
				board->pieces[wP] ^= (move->to)>>8;
			}
			break;
		case wKSC:
			board->pieces[wK] ^= U64_E1;
			board->pieces[wK] ^= U64_G1;
			board->pieces[wR] ^= U64_H1;
			board->pieces[wR] ^= U64_F1;
			break;
		case wQSC:
			board->pieces[wK] ^= U64_E1;
			board->pieces[wK] ^= U64_C1;
			board->pieces[wR] ^= U64_A1;
			board->pieces[wR] ^= U64_D1;
			break;
		case bKSC:
			board->pieces[bK] ^= U64_E8;
			board->pieces[bK] ^= U64_G8;
			board->pieces[bR] ^= U64_H8;
			board->pieces[bR] ^= U64_F8;
			break;
		case bQSC:
			board->pieces[bK] ^= U64_E8;
			board->pieces[bK] ^= U64_C8;
			board->pieces[bR] ^= U64_A8;
			board->pieces[bR] ^= U64_D8;
			break;
	}
	
	board->ep = move->ep_old;
	board->castling[wKSC] = move->wKSC_old;
	board->castling[wQSC] = move->wQSC_old;
	board->castling[bKSC] = move->bKSC_old;
	board->castling[bQSC] = move->bQSC_old;
	
	int x;
	board->all_pieces[WHITE] = 0;
	board->all_pieces[BLACK] = 0;
	for(x = wP; x <= wK; ++x)
	{
		board->all_pieces[WHITE] |= board->pieces[x];
	}
	for(x = bP; x <= bK; ++x)
	{
		board->all_pieces[BLACK] |= board->pieces[x];
	}
	board->all_pieces[BOTH] = board->all_pieces[WHITE] | board->all_pieces[BLACK];
	
	board->turn = 1-(board->turn);
}