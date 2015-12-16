#include "defs.h"

#include <time.h>

int main()
{
  printf("%s %s\n\n", ENGINE_NAME, ENGINE_VERSION);
  
  // Initialise magic bitboards
  bitboards_init();
  
  // Print some details
  #ifdef TEST_MOVEGEN
    printf("Test movegen\n");
  #else
    printf("Normal movegen\n");
  #endif
  printf("sizeof(s_board): %I64uB\n", sizeof(s_board)); // 152
  printf("sizeof(s_move):  %I64uB\n", sizeof(s_move));  //  56
  printf("\n");
  
  // Test some stuff
  if(u64_to_sq(U64_C4) != C4)
  {
    printf("Error:\n");
    printf("%i %i\n", u64_to_sq(U64_C4), C4);
  }
  
  s_board* board = (s_board*) malloc(1*sizeof(s_board));
  if(board == NULL) {return -1;}
  
  // perft
  //perft_movegen(board, "perftsuite.epd");
  //perft(board, 7, START_FEN);
  //perft_split(board, 1, "rnbqkbnr/ppppppp1/7p/8/8/6PP/PPPPPP2/RNBQKBNR b KQkq - 0 2");
  perft_suite(board, 5, "perftsuite.epd");
  
  printf("Done\n");
  getchar();
  return 0;
}
