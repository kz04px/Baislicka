#include "defs.h"

void display_board(s_board *board)
{
	int n, i;
	for(n = 0; n < 64; ++n)
	{
		if(GETBIT(board->pieces[wP], n))
		{
			printf("P");
		}
		else if(GETBIT(board->pieces[wN], n))
		{
			printf("N");
		}
		else if(GETBIT(board->pieces[wB], n))
		{
			printf("B");
		}
		else if(GETBIT(board->pieces[wR], n))
		{
			printf("R");
		}
		else if(GETBIT(board->pieces[wQ], n))
		{
			printf("Q");
		}
		else if(GETBIT(board->pieces[wK], n))
		{
			printf("K");
		}
		else if(GETBIT(board->pieces[bP], n))
		{
			printf("p");
		}
		else if(GETBIT(board->pieces[bN], n))
		{
			printf("n");
		}
		else if(GETBIT(board->pieces[bB], n))
		{
			printf("b");
		}
		else if(GETBIT(board->pieces[bR], n))
		{
			printf("r");
		}
		else if(GETBIT(board->pieces[bQ], n))
		{
			printf("q");
		}
		else if(GETBIT(board->pieces[bK], n))
		{
			printf("k");
		}
		else
		{
			printf("-");
		}
		
		if(n%8 == 7)
		{
			printf("\n");
		}
	}
	
	/*
	if(board->turn == WHITE)
	{
		printf("Turn: w\n");
	}
	else
	{
		printf("Turn: b\n");
	}
	*/
}

void display_bitboard(U64 Our_Bitboard)
{
	int n;
	for(n = 0; n < 64; ++n)
	{
		printf("%i", GETBIT(Our_Bitboard, n));
		if(n%8 == 7) printf("\n");
	}
}