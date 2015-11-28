#include "defs.h"

char Int_to_Char(int n)
{
	switch(n)
	{
		case bP:
		{
			return 'p';
			break;
		}
		case bN:
		{
			return 'n';
			break;
		}
		case bB:
		{
			return 'b';
			break;
		}
		case bR:
		{
			return 'r';
			break;
		}
		case bQ:
		{
			return 'q';
			break;
		}
		case bK:
		{
			return 'k';
			break;
		}
		case wP:
		{
			return 'P';
			break;
		}
		case wN:
		{
			return 'N';
			break;
		}
		case wB:
		{
			return 'B';
			break;
		}
		case wR:
		{
			return 'R';
			break;
		}
		case wQ:
		{
			return 'Q';
			break;
		}
		case wK:
		{
			return 'K';
			break;
		}
	}
	return '?';
}

void Print_Piecelist(s_board *board)
{
	int n;
	for(n = 0; n < 12; ++n)
	{
		int i;
		printf("%c: ", Int_to_Char(n));
		for(i = 0; i < board->num_pieces[n]; ++i)
		{
			printf("%i ", board->locations[n][i]);
		}
		printf("\n");
	}
}

int set_fen(s_board *board, const char *string)
{
	board->all_pieces[BOTH] = 0;
	board->all_pieces[WHITE] = 0;
	board->all_pieces[BLACK] = 0;
	
	board->pieces[wP] = 0;
	board->pieces[wN] = 0;
	board->pieces[wB] = 0;
	board->pieces[wR] = 0;
	board->pieces[wQ] = 0;
	board->pieces[wK] = 0;
	
	board->pieces[bP] = 0;
	board->pieces[bN] = 0;
	board->pieces[bB] = 0;
	board->pieces[bR] = 0;
	board->pieces[bQ] = 0;
	board->pieces[bK] = 0;
	
	board->ep = 0;
	
	board->castling[wKSC] = FALSE;
	board->castling[wQSC] = FALSE;
	board->castling[bKSC] = FALSE;
	board->castling[bQSC] = FALSE;
	
	int i, j;
	for(i = 0; i < 12; ++i)
	{
		board->num_pieces[i] = 0;
		for(j = 0; j < 10; ++j)
		{
			board->locations[i][j] = 0;
		}
	}
	
	int n = 0, x = 0;
	while(string[n] != ' ')
	{
		switch(string[n])
		{
			case '/':
			{
				x--;
				break;
			}
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			{
				x += string[n]-48 - 1; // FIXME: string[n] = '0';
				break;
			}
			case 'p':
			{
				SETBIT(board->pieces[bP], x);
				board->locations[bP][board->num_pieces[bP]] = x;
				board->num_pieces[bP]++;
				break;
			}
			case 'n':
			{
				SETBIT(board->pieces[bN], x);
				board->locations[bN][board->num_pieces[bN]] = x;
				board->num_pieces[bN]++;
				break;
			}
			case 'b':
			{
				SETBIT(board->pieces[bB], x);
				board->locations[bB][board->num_pieces[bB]] = x;
				board->num_pieces[bB]++;
				break;
			}
			case 'r':
			{
				SETBIT(board->pieces[bR], x);
				board->locations[bR][board->num_pieces[bR]] = x;
				board->num_pieces[bR]++;
				break;
			}
			case 'q':
			{
				SETBIT(board->pieces[bQ], x);
				board->locations[bQ][board->num_pieces[bQ]] = x;
				board->num_pieces[bQ]++;
				break;
			}
			case 'k':
			{
				SETBIT(board->pieces[bK], x);
				board->locations[bK][board->num_pieces[bK]] = x;
				board->num_pieces[bK]++;
				
				//printf("x: %i\n", x);
				//printf("num_pieces[bK]: %i\n", board->num_pieces[bK]);
				//printf("eh: %i\n", board->locations[bK][board->num_pieces[bK]]);
				break;
			}
			case 'P':
			{
				SETBIT(board->pieces[wP], x);
				board->locations[wP][board->num_pieces[wP]] = x;
				board->num_pieces[wP]++;
				break;
			}
			case 'N':
			{
				SETBIT(board->pieces[wN], x);
				board->locations[wN][board->num_pieces[wN]] = x;
				board->num_pieces[wN]++;
				break;
			}
			case 'B':
			{
				SETBIT(board->pieces[wB], x);
				board->locations[wB][board->num_pieces[wB]] = x;
				board->num_pieces[wB]++;
				break;
			}
			case 'R':
			{
				SETBIT(board->pieces[wR], x);
				board->locations[wR][board->num_pieces[wR]] = x;
				board->num_pieces[wR]++;
				break;
			}
			case 'Q':
			{
				SETBIT(board->pieces[wQ], x);
				board->locations[wQ][board->num_pieces[wQ]] = x;
				board->num_pieces[wQ]++;
				break;
			}
			case 'K':
			{
				SETBIT(board->pieces[wK], x);
				board->locations[wK][board->num_pieces[wK]] = x;
				board->num_pieces[wK]++;
				break;
			}
			default:
			{
				printf("'%c' ???\n", string[n]);
				break;
				// return -3; // FIXME
			}
		}
		n++;
		x++;
	}
	n += 1;
	
	if(string[n] == 'w')
	{
		board->turn = WHITE;
	}
	else if(string[n] == 'b')
	{
		board->turn = BLACK;
	}
	else
	{
		printf("string: %s\n", string);
		printf("n: %i\n", n);
		printf("string[n-1]: %c\n", string[n-1]);
		printf("string[n]:   %c\n", string[n]);
		printf("string[n+1]: %c\n", string[n+1]);
		getchar();
		return -1;
	}
	n += 2;
	
	while(string[n] != ' ')
	{
		switch(string[n])
		{
			case 'K':
			{
				board->castling[wKSC] = TRUE;
				break;
			}
			case 'Q':
			{
				board->castling[wQSC] = TRUE;
				break;
			}
			case 'k':
			{
				board->castling[bKSC] = TRUE;
				break;
			}
			case 'q':
			{
				board->castling[bQSC] = TRUE;
				break;
			}
			case '-':
			{
				break;
			}
			default:
			{
				printf("string: %s\n", string);
				printf("n: %i\n", n);
				printf("string[n-1]: %c\n", string[n-1]);
				printf("string[n]:   %c\n", string[n]);
				printf("string[n+1]: %c\n", string[n+1]);
				getchar();
				return -2;
			}
		}
		n++;
	}
	n++;
	
	if(string[n] != '-')
	{
		int col = string[n]-'a';
		int row = 7-(string[n+1]-'1');
		printf("Row: %i\n", row);
		board->ep = (U64)1<<(row*8 + col);
		printf("EP: %c%c\n", string[n], string[n+1]);
		display_bitboard(board->ep);
		n++;
	}
	n += 2;
	
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
	board->all_pieces[BOTH] = board->all_pieces[WHITE]|board->all_pieces[BLACK];
	
	update_attacking(board);
	
	/*
	if(board->turn == WHITE)
	{
		printf("Turn: w\n");
	}
	else
	{
		printf("Turn: b\n");
	}
	printf("wKSC: %i\n", board->castling[wKSC]);
	printf("wQSC: %i\n", board->castling[wQSC]);
	printf("bKSC: %i\n", board->castling[bKSC]);
	printf("bQSC: %i\n", board->castling[bQSC]);
	*/
	
	return 0;
}