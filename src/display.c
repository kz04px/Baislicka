#include "defs.h"

void print_move(s_move move)
{
  int p;
  for(p = 0; p <= 63; ++p)
  {
    if(move.from>>p & 1) {break;}
  }
  printf("%c%c ", POS_TO_COL_CHAR(p), POS_TO_ROW_CHAR(p));
  
  for(p = 0; p <= 63; ++p)
  {
    if(move.to>>p & 1) {break;}
  }
  printf("%c%c ", POS_TO_COL_CHAR(p), POS_TO_ROW_CHAR(p));
  
  // Type
  switch(move.type)
  {
    case NORMAL:
      printf("normal");
      break;
    case DOUBLE_MOVE:
      printf("double_move");
      break;
    case CAPTURE:
      printf("capture");
      break;
    case EP:
      printf("ep");
      break;
    case wKSC:
      printf("wKSC");
      break;
    case wQSC:
      printf("wQSC");
      break;
    case bKSC:
      printf("bKSC");
      break;
    case bQSC:
      printf("bQSC");
      break;
    default:
      printf("??? (%i)", move.type);
      printf("\n");
      
      printf("from:\n");
      print_u64(move.from);
      
      printf("to:\n");
      print_u64(move.to);
      
      break;
  }
    
  printf("\n");
}

void print_move_list(s_move* move_list, int num_moves)
{
  ASSERT(move_list != NULL);
  
  int i;
  for(i = 0; i < num_moves; ++i)
  {
    if(i+1 < 10) {printf(" ");}
    printf("%i: ", i+1);
    
    print_move(move_list[i]);
  }
}

void print_u64(U64 board)
{
  int i;
  for(i = 63; i >= 0; --i)
  {
    printf("%i", (int)(1&(board>>i)));
    if(i%8 == 0) {printf("\n");}
  }
  printf("\n");
  return;
}

void display_board(s_board *board)
{
	int p;
	for(p = 63; p >= 0; --p)
	{
		if(GETBIT(board->pieces[wP], p))
		{
			printf("P");
		}
		else if(GETBIT(board->pieces[wN], p))
		{
			printf("N");
		}
		else if(GETBIT(board->pieces[wB], p))
		{
			printf("B");
		}
		else if(GETBIT(board->pieces[wR], p))
		{
			printf("R");
		}
		else if(GETBIT(board->pieces[wQ], p))
		{
			printf("Q");
		}
		else if(GETBIT(board->pieces[wK], p))
		{
			printf("K");
		}
		else if(GETBIT(board->pieces[bP], p))
		{
			printf("p");
		}
		else if(GETBIT(board->pieces[bN], p))
		{
			printf("n");
		}
		else if(GETBIT(board->pieces[bB], p))
		{
			printf("b");
		}
		else if(GETBIT(board->pieces[bR], p))
		{
			printf("r");
		}
		else if(GETBIT(board->pieces[bQ], p))
		{
			printf("q");
		}
		else if(GETBIT(board->pieces[bK], p))
		{
			printf("k");
		}
		else
		{
			printf("-");
		}
		
		if(p%8 == 0)
		{
			printf("\n");
		}
	}
  
  printf("Ep: %I64u\n", board->ep);
}