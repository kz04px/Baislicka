#include "defs.h"

int main()
{
  printf("Baislicka\n\n");
  
  // Initialise magic bitboards
  generateOccupancyVariations(0);
  generateMoveDatabase(0);
  generateOccupancyVariations(1);
  generateMoveDatabase(1);
  
  s_board* board = (s_board*) malloc(1*sizeof(s_board));
  if(board == NULL) {return -1;}
  
  //int r = set_fen(board, "8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1");
  //display_board(board);
  //printf("Ret: %i\n", r);
  
  // perft
  //perft(board, 7, "8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1 ");
  //perft_split(board, 3, "rnbqkbnr/1ppppppp/p7/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");
  
  //perft_split(board, 1, "8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1 ");
  perft_suite(board, 4, "perftsuite.epd");
  
  printf("Done\n");
  getchar();
  return 0;
}

