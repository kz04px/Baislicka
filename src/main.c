#include "defs.h"

#include <time.h>

int main()
{
  printf("%s %s\n\n", ENGINE_NAME, ENGINE_VERSION);
  
  // Initialise magic bitboards
  generateOccupancyVariations(0);
  generateMoveDatabase(0);
  generateOccupancyVariations(1);
  generateMoveDatabase(1);
  generateKnightMoves();
  
  #ifdef TEST_MOVEGEN
    printf("Test movegen\n\n");
  #else
    printf("Normal movegen\n\n");
  #endif
 
  s_board* board = (s_board*) malloc(1*sizeof(s_board));
  if(board == NULL) {return -1;}
  
  //int r = set_fen(board, "8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1");
  //display_board(board);
  //printf("Ret: %i\n", r);
  
  // perft
  //perft_movegen(board, "perftsuite.epd");
  perft(board, 7, START_FEN);
  //perft_split(board, 1, "rnbqkbnr/ppppppp1/7p/8/8/6PP/PPPPPP2/RNBQKBNR b KQkq - 0 2");
  //perft_suite(board, 5, "perftsuite.epd");
  
  printf("Done\n");
  getchar();
  return 0;
}
