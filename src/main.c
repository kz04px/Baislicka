#include "defs.h"

int main()
{
  printf("%s %s\n\n", ENGINE_NAME, ENGINE_VERSION);
  
  // Initialise magic bitboards
  bitboards_init();
  
  #ifndef NDEBUG
  // Print some details
  printf("sizeof(s_board): %"PRIdPTR"B\n", sizeof(s_board)); // 152
  printf("sizeof(s_move):  %"PRIdPTR"B\n", sizeof(s_move));  //  56
  printf("\n");
  #endif
  
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
  //perft_movegen_sides(board, "perftsuite.epd");
  //perft(board, 7, START_FEN);
  //perft_split(board, 1, START_FEN);
  perft_suite(board, 4, "perftsuite.epd");
  
  /*
  // search
  for(i = 0; i <= 9; ++i)
  {
    
    set_fen(board, START_FEN);
    search(board, i);
  }
  */
  
  printf("Done\n");
  getchar();
  return 0;
}
