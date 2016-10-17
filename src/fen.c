#include "defs.h"
#include <string.h>

const char temp[256] = START_FEN;

int set_fen(s_board *board, const char *fen)
{
  assert(board != NULL);
  assert(fen != NULL);
  
  if(strncmp(fen, "startpos", 8) == 0)
  {
    fen = temp;
  }
  
  int i;
  for(i = 0; i < 6; ++i)
  {
    board->pieces[i] = 0;
  }
  board->colour[WHITE] = 0;
  board->colour[BLACK] = 0;
  
  board->ep = 0;
  
  board->castling = 0;
  
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
  
  int n = 0, x = A8;
  while(fen[n] != ' ')
  {
    switch(fen[n])
    {
      case '/':
      {
        x -= 17;
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
        x += fen[n] - '1'; // FIXME: fen[n] = '0';
        break;
      }
      case 'p':
      {
        board->pieces[PAWNS] ^= (uint64_t)1<<x;
        board->colour[BLACK] ^= (uint64_t)1<<x;
        break;
      }
      case 'n':
      {
        board->pieces[KNIGHTS] ^= (uint64_t)1<<x;
        board->colour[BLACK] ^= (uint64_t)1<<x;
        break;
      }
      case 'b':
      {
        board->pieces[BISHOPS] ^= (uint64_t)1<<x;
        board->colour[BLACK] ^= (uint64_t)1<<x;
        break;
      }
      case 'r':
      {
        board->pieces[ROOKS] ^= (uint64_t)1<<x;
        board->colour[BLACK] ^= (uint64_t)1<<x;
        break;
      }
      case 'q':
      {
        board->pieces[QUEENS] ^= (uint64_t)1<<x;
        board->colour[BLACK] ^= (uint64_t)1<<x;
        break;
      }
      case 'k':
      {
        board->pieces[KINGS] ^= (uint64_t)1<<x;
        board->colour[BLACK] ^= (uint64_t)1<<x;
        break;
      }
      case 'P':
      {
        board->pieces[PAWNS] ^= (uint64_t)1<<x;
        board->colour[WHITE] ^= (uint64_t)1<<x;
        break;
      }
      case 'N':
      {
        board->pieces[KNIGHTS] ^= (uint64_t)1<<x;
        board->colour[WHITE] ^= (uint64_t)1<<x;
        break;
      }
      case 'B':
      {
        board->pieces[BISHOPS] ^= (uint64_t)1<<x;
        board->colour[WHITE] ^= (uint64_t)1<<x;
        break;
      }
      case 'R':
      {
        board->pieces[ROOKS] ^= (uint64_t)1<<x;
        board->colour[WHITE] ^= (uint64_t)1<<x;
        break;
      }
      case 'Q':
      {
        board->pieces[QUEENS] ^= (uint64_t)1<<x;
        board->colour[WHITE] ^= (uint64_t)1<<x;
        break;
      }
      case 'K':
      {
        board->pieces[KINGS] ^= (uint64_t)1<<x;
        board->colour[WHITE] ^= (uint64_t)1<<x;
        break;
      }
      default:
      {
        break;
      }
    }
    n++;
    x++;
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
        board->castling |= wKSC;
        break;
      }
      case 'Q':
      {
        board->castling |= wQSC;
        break;
      }
      case 'k':
      {
        board->castling |= bKSC;
        break;
      }
      case 'q':
      {
        board->castling |= bQSC;
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
    int file = fen[n] - 'a';
    int rank = fen[n+1] - '1';
    assert(0 <= file && file <= 7);
    assert(0 <= rank && rank <= 7);
    board->ep = rank*8 + file;
    n++;
  }
  n += 2;
  
  board->num_halfmoves = atoi(&fen[n]);
  
  #ifdef HASHTABLE
    board->key = create_key_board(board);
  #endif
  
  // History
  board->key_history[0] = board->key;
  board->history_size = 1;
  
  return 0;
}