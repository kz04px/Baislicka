#include "defs.h"

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
  printf("sizeof(s_board): %"PRIdPTR"B\n", sizeof(s_board)); // 152
  printf("sizeof(s_move):  %"PRIdPTR"B\n", sizeof(s_move));  //  56
  printf("\n");
  
  // Test some stuff
  int i;
  for(i = 0; i < 64; ++i)
  {
    if(u64_to_sq((uint64_t)1<<i) != i)
    {
      printf("u64_to_sq() mismatch: %i %i %"PRIdPTR"\n", u64_to_sq(i), i, (uint64_t)1<<i);
    }
  }
  
  s_board* board = (s_board*) malloc(1*sizeof(s_board));
  if(board == NULL) {return -1;}
  
  // perft
  //perft_movegen(board, "perftsuite.epd");
  perft(board, 4, START_FEN);
  //perft_split(board, 1, "rnbqkbnr/ppppppp1/7p/8/8/6PP/PPPPPP2/RNBQKBNR b KQkq - 0 2");
  //perft_suite(board, 5, "perftsuite.epd");
  
  // search
  for(i = 1; i <= 7; ++i)
  {
    set_fen(board, START_FEN);
    search(board, i);
  }
  
  printf("Done\n");
  getchar();
  return 0;
}
