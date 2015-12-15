#include "defs.h"

int set_fen(s_board *board, const char *fen)
{
  ASSERT(board != NULL);
  ASSERT(fen != NULL);
  
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
  
  /*
  int i, j;
  for(i = 0; i < 12; ++i)
  {
    board->num_pieces[i] = 0;
    for(j = 0; j < 10; ++j)
    {
      board->locations[i][j] = 0;
    }
  }
  */
  
  int n = 0, x = 63;
  while(fen[n] != ' ')
  {
    switch(fen[n])
    {
      case '/':
      {
        x++;
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
        x -= fen[n] - '1'; // FIXME: fen[n] = '0';
        break;
      }
      case 'p':
      {
        SETBIT(board->pieces[bP], x);
        //board->locations[bP][board->num_pieces[bP]] = x;
        //board->num_pieces[bP]++;
        break;
      }
      case 'n':
      {
        SETBIT(board->pieces[bN], x);
        //board->locations[bN][board->num_pieces[bN]] = x;
        //board->num_pieces[bN]++;
        break;
      }
      case 'b':
      {
        SETBIT(board->pieces[bB], x);
        //board->locations[bB][board->num_pieces[bB]] = x;
        //board->num_pieces[bB]++;
        break;
      }
      case 'r':
      {
        SETBIT(board->pieces[bR], x);
        //board->locations[bR][board->num_pieces[bR]] = x;
        //board->num_pieces[bR]++;
        break;
      }
      case 'q':
      {
        SETBIT(board->pieces[bQ], x);
        //board->locations[bQ][board->num_pieces[bQ]] = x;
        //board->num_pieces[bQ]++;
        break;
      }
      case 'k':
      {
        SETBIT(board->pieces[bK], x);
        //board->locations[bK][board->num_pieces[bK]] = x;
        //board->num_pieces[bK]++;
        break;
      }
      case 'P':
      {
        SETBIT(board->pieces[wP], x);
        //board->locations[wP][board->num_pieces[wP]] = x;
        //board->num_pieces[wP]++;
        break;
      }
      case 'N':
      {
        SETBIT(board->pieces[wN], x);
        //board->locations[wN][board->num_pieces[wN]] = x;
        //board->num_pieces[wN]++;
        break;
      }
      case 'B':
      {
        SETBIT(board->pieces[wB], x);
        //board->locations[wB][board->num_pieces[wB]] = x;
        //board->num_pieces[wB]++;
        break;
      }
      case 'R':
      {
        SETBIT(board->pieces[wR], x);
        //board->locations[wR][board->num_pieces[wR]] = x;
        //board->num_pieces[wR]++;
        break;
      }
      case 'Q':
      {
        SETBIT(board->pieces[wQ], x);
        //board->locations[wQ][board->num_pieces[wQ]] = x;
        //board->num_pieces[wQ]++;
        break;
      }
      case 'K':
      {
        SETBIT(board->pieces[wK], x);
        //board->locations[wK][board->num_pieces[wK]] = x;
        //board->num_pieces[wK]++;
        break;
      }
      default:
      {
        break;
      }
    }
    n++;
    x--;
  }
  n += 1;
  
  if(fen[n] == 'w')
  {
    board->turn = WHITE;
  }
  else if(fen[n] == 'b')
  {
    board->turn = BLACK;
  }
  else
  {
    return -1;
  }
  n += 2;
  
  while(fen[n] != ' ' && fen[n] != '\0' && fen[n] != '\n')
  {
    switch(fen[n])
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
        return -2;
      }
    }
    n++;
  }
  n++;
  
  if(fen[n] != '-')
  {
    int col = fen[n] - 'a';
    int row = fen[n+1] - '1';
    board->ep = (U64)1<<(row*8 + 7 - col);
    n++;
  }
  n += 2;
  
  board->pieces_colour[WHITE] = board->pieces[wP] | board->pieces[wN] | board->pieces[wB] |
                                board->pieces[wR] | board->pieces[wQ] | board->pieces[wK];
  
  board->pieces_colour[BLACK] = board->pieces[bP] | board->pieces[bN] | board->pieces[bB] |
                                board->pieces[bR] | board->pieces[bQ] | board->pieces[bK];
  
  board->pieces_all = board->pieces_colour[WHITE] | board->pieces_colour[BLACK];
  
  //update_attacking(board);
  
  return 0;
}