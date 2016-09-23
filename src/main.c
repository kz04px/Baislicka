#include "defs.h"

int main()
{
  srand(0);
  //srand(time(0));
  printf("%s %s\n\n", ENGINE_NAME, ENGINE_VERSION);
  
  // Initialise magic bitboards
  bitboards_init();
  
  #ifdef HASHTABLE
    hashtable = malloc(1*sizeof(*hashtable));
    key_init();
    
    int size = 256;
    while(size > 0)
    {
      int r = hashtable_init(hashtable, size);
      if(r > 0)
      {
        break;
      }
      else
      {
        size = size>>1;
      }
    }
  #endif
  
  #ifndef NDEBUG
    printf("Search debug info:\n");
    printf("Alphabeta search\n");
    
    #ifdef QUIESCENCE_SEARCH
      printf("Quiescence search enabled\n");
    #else
      printf("Quiescence search disabled\n");
    #endif
    
    printf("Max search depth: %i\n", MAX_DEPTH);
    
    printf("\n");
    
    printf("Transposition table:\n");
    #ifdef HASHTABLE
      printf("Total size: %iMB\n", hashtable->size_bytes/1024/1024);
      printf("Entry size: %"PRIdPTR"B\n", sizeof(s_hashtable_entry));
      printf("Max entries: %i\n", hashtable->max_entries);
    #else
      printf("Disabled\n");
    #endif
    printf("\n");
    
    printf("Principal variation:\n");
    #ifdef GET_PV
      printf("Enabled\n");
    #else
      printf("Disabled\n");
    #endif
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
  
  //perft_suite(board, 5, "perftsuite.epd");
  //getchar();
  
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
    
    /*
    set_fen(board, input_fen);
    search(board, 12);
    */
    
    for(i = 0; i < MAX_DEPTH; ++i)
    {
      set_fen(board, input_fen);
      if(search(board, i) != 0)
      {
        break;
      }
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
  set_fen(board, "8/4kp2/7p/1pq2p2/3p4/1P1Pb1P1/1BP3QP/7K w - -");
  search(board, 5);
  */
  
  /*
  // search
  for(i = 0; i <= 9; ++i)
  {
    set_fen(board, "rnqr2k1/pb2bppp/1p6/4p3/8/2N1BN2/PPP2PPP/R2Q1RK1 w - -");
    search(board, i);
  }
  */
  
  
  printf("Done\n");
  getchar();
  return 0;
}
