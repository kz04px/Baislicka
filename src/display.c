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
  
  switch(move.piece_type)
  {
    case wP:
      printf("(P) ");
      break;
    case wN:
      printf("(N) ");
      break;
    case wB:
      printf("(B) ");
      break;
    case wR:
      printf("(R) ");
      break;
    case wQ:
      printf("(Q) ");
      break;
    case wK:
      printf("(K) ");
      break;
    case bP:
      printf("(p) ");
      break;
    case bN:
      printf("(n) ");
      break;
    case bB:
      printf("(b) ");
      break;
    case bR:
      printf("(r) ");
      break;
    case bQ:
      printf("(q) ");
      break;
    case bK:
      printf("(k) ");
      break;
    default:
      break;
  }
  
  // Type
  switch(move.type)
  {
    case QUIET:
      printf("normal");
      break;
    case DOUBLE_PAWN:
      printf("double_move");
      break;
    case CAPTURE:
      printf("capture");
      break;
    case EP:
      printf("ep");
      break;
    case PROMOTE:
      printf("promote");
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

void print_moves(s_move* moves, int num_moves)
{
  assert(moves != NULL);
  
  int i;
  for(i = 0; i < num_moves; ++i)
  {
    if(i+1 < 10) {printf(" ");}
    printf("%i: ", i+1);
    
    print_move(moves[i]);
  }
}

void print_u64(uint64_t board)
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
  
  if(board->turn == WHITE) {printf("Turn: w\n");}
  else {printf("Turn: b\n");}
  
  printf("Castling: ");
  if(board->castling[wKSC]) {printf("K");}
  if(board->castling[wQSC]) {printf("Q");}
  if(board->castling[bKSC]) {printf("k");}
  if(board->castling[bQSC]) {printf("q");}
  printf("\n");
  
  printf("Ep: %"PRIdPTR"\n", board->ep);
}