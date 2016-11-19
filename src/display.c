#include "defs.h"

void print_move(s_move move)
{
  printf("%c%c %c%c ", SQ_TO_FILE_CHAR(move.from), SQ_TO_RANK_CHAR(move.from), SQ_TO_FILE_CHAR(move.to), SQ_TO_RANK_CHAR(move.to));
  
  switch(move.piece_type)
  {
    case PAWNS:
      printf("(P) ");
      break;
    case KNIGHTS:
      printf("(N) ");
      break;
    case BISHOPS:
      printf("(B) ");
      break;
    case ROOKS:
      printf("(R) ");
      break;
    case QUEENS:
      printf("(Q) ");
      break;
    case KINGS:
      printf("(K) ");
      break;
    default:
      printf("[?] ");
      break;
  }
  
  switch(move.taken)
  {
    case PAWNS:
      printf("[p] ");
      break;
    case KNIGHTS:
      printf("[N] ");
      break;
    case BISHOPS:
      printf("[B] ");
      break;
    case ROOKS:
      printf("[R] ");
      break;
    case QUEENS:
      printf("[Q] ");
      break;
    case EMPTY:
      printf("[-] ");
      break;
    default:
      printf("[?] ");
      break;
  }
  
  switch(move.promotion)
  {
    case KNIGHTS:
      printf("[N] ");
      break;
    case BISHOPS:
      printf("[B] ");
      break;
    case ROOKS:
      printf("[R] ");
      break;
    case QUEENS:
      printf("[Q] ");
      break;
    case EMPTY:
      printf("[-] ");
      break;
    default:
      printf("[?] ");
      break;
  }
  
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
    case KSC:
      printf("KSC");
      break;
    case QSC:
      printf("QSC");
      break;
    default:
      printf("??? (%i)", move.type);
      break;
  }
  
  printf("\n");
}

void print_moves(s_move *moves, int num_moves)
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
  for(i = A8; i >= A1; ++i)
  {
    printf("%i", (int)(1&(board>>i)));
    if(i%8 == 7) {i -= 16; printf("\n");}
  }
  
  return;
}

void display_board(s_board *board)
{
  int i;
  for(i = A8; i >= A1; ++i)
  {
    if(GETBIT(board->pieces[PAWNS], i))
    {
      if(GETBIT(board->colour[WHITE], i))
      {
        printf("P");
      }
      else
      {
        printf("p");
      }
    }
    else if(GETBIT(board->pieces[KNIGHTS], i))
    {
      if(GETBIT(board->colour[WHITE], i))
      {
        printf("N");
      }
      else
      {
        printf("n");
      }
    }
    else if(GETBIT(board->pieces[BISHOPS], i))
    {
      if(GETBIT(board->colour[WHITE], i))
      {
        printf("B");
      }
      else
      {
        printf("b");
      }
    }
    else if(GETBIT(board->pieces[ROOKS], i))
    {
      if(GETBIT(board->colour[WHITE], i))
      {
        printf("R");
      }
      else
      {
        printf("r");
      }
    }
    else if(GETBIT(board->pieces[QUEENS], i))
    {
      if(GETBIT(board->colour[WHITE], i))
      {
        printf("Q");
      }
      else
      {
        printf("q");
      }
    }
    else if(GETBIT(board->pieces[KINGS], i))
    {
      if(GETBIT(board->colour[WHITE], i))
      {
        printf("K");
      }
      else
      {
        printf("k");
      }
    }
    else
    {
      printf("-");
    }
    
    if(i%8 == 7) {i -= 16; printf("\n");}
  }
  
  if(board->turn == WHITE) {printf("Turn: w\n");}
  else {printf("Turn: b\n");}
  
  printf("Key: %"PRIdPTR"\n", board->key);
  printf("50 move rule: %i\n", board->num_halfmoves);
  printf("history size: %i\n", board->history_size);
  printf("is 3fold:   %i\n", is_threefold(board));
  printf("is 50move:  %i\n", is_fifty_move_draw(board));
  printf("is endgame: %i\n", is_endgame(board));
  printf("Eval: %i\n", eval(board));
  
  printf("Castling: ");
  if(board->castling & wKSC) {printf("K");}
  if(board->castling & wQSC) {printf("Q");}
  if(board->castling & bKSC) {printf("k");}
  if(board->castling & bQSC) {printf("q");}
  printf("\n");
  
  printf("Ep: %c%c\n", SQ_TO_FILE_CHAR(board->ep), SQ_TO_RANK_CHAR(board->ep));
}

void display_history(s_board *board)
{
  assert(board != NULL);
  
  int i;
  for(i = 0; i < board->history_size; ++i)
  {
    printf("%i) %"PRIdPTR"\n", i, board->key_history[i]);
  }
}