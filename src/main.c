#include "defs.h"

int main()
{
  printf("%s %s\n\n", ENGINE_NAME, ENGINE_VERSION);
  
  // Initialise magic bitboards
  bitboards_init();
  
  #ifndef NDEBUG
    printf("Search debug info:\n");
    #ifdef ALPHA_BETA
      printf("Alphabeta search\n");
    #elif MINMAX
      printf("Minmax search\n");
    #endif
    
    #ifdef QUIESCENCE_SEARCH
      printf("Quiescence search enabled\n");
    #elif
      printf("Quiescence search disabled\n");
    #endif
    
    printf("Max search depth: %i\n", MAX_DEPTH);
    
    printf("\n");
    
    printf("Other info:\n");
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
  //perft_split(board, 2, START_FEN);
  //perft_suite(board, 4, "perftsuite.epd");
  
  /*
  set_fen(board, "rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
  display_board(board);
  printf("\n");
  printf("Eval: %i\n", eval(board));
  printf("\n");
  */
  
  char input_fen[256] = {0};
  
  while(1)
  {
    printf("Enter FEN: ");
    fgets(input_fen, 256, stdin);
    
    //set_fen(board, input_fen);
    //search(board, 6);
    
    for(i = 0; i <= 8; ++i)
    {
      set_fen(board, input_fen);
      search(board, i);
    }
  }
  
  /*
  char input_fen[256] = {0};
  
  while(1)
  {
    printf("Enter FEN: ");
    fgets(input_fen, 256, stdin);
    set_fen(board, input_fen);
    int score = eval(board);
    printf("Eval: %i\n\n", score);
  }
  */
  
  
  /*
  // search
  for(i = 0; i <= 9; ++i)
  {
    set_fen(board, "Q1b5/4B1pk/4p2p/pp2P3/4N3/5N2/PP2K1PP/R4R2 w - - 1 23");
    search(board, i);
  }
  */
  
  
  printf("Done\n");
  getchar();
  return 0;
}
