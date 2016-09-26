#include "defs.h"
#include <string.h>

int main()
{
  srand(time(0));
  printf("%s %s\n\n", ENGINE_NAME, ENGINE_VERSION);
  
  // Initialise magic bitboards
  bitboards_init();
  
  #ifdef HASHTABLE
    key_init();
    hashtable = NULL;
  #endif
  
  #ifndef NDEBUG
    printf("Search debug info:\n");
    printf("Alphabeta search\n");
    printf("Max search depth: %i\n", MAX_DEPTH);
    
    #ifdef QUIESCENCE_SEARCH
      printf("Quiescence search - enabled\n");
    #else
      printf("Quiescence search - disabled\n");
    #endif
    
    #ifdef HASHTABLE
      printf("Transposition table - enabled\n");
    #else
      printf("Transposition table - disabled\n");
    #endif
    
    #ifdef GET_PV
      printf("Principal variation - enabled\n");
    #else
      printf("Principal variation - disabled\n");
    #endif
    printf("\n");
    
    printf("Other info:\n");
    printf("sizeof(s_board): %"PRIdPTR"B\n", sizeof(s_board)); // 160B
    printf("sizeof(s_move):  %"PRIdPTR"B\n", sizeof(s_move));  //  64B
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
  
  // kill buffering
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);
  
  char message[4096];
  fgets(message, 4096, stdin);
  if(strncmp(message, "uci", 3) == 0)
  {
    GUI_Send("id name %s\n", ENGINE_NAME);
    GUI_Send("id author %s\n", ENGINE_AUTHOR);
    #ifdef HASHTABLE
      GUI_Send("option name Hash type spin default 0 min %i max %i\n", HASHTABLE_SIZE_MIN, HASHTABLE_SIZE_MAX);
    #endif
    GUI_Send("uciok\n");
    
    uci_listen();
  }
  else if(strncmp(message, "xboard", 6) == 0)
  {
    GUI_Send("xboard protocol not implemented\n");
  }
  else if(strncmp(message, "test", 4) == 0)
  {
    s_board* board = (s_board*) malloc(1*sizeof(s_board));
    if(board == NULL) {return -1;}
    
    perft_suite(board, 5, "perftsuite.epd");
    //set_fen(board, START_FEN);
    //perft_movegen(board, "perftsuite.epd");
    //perft_movegen_sides(board, "perftsuite.epd");
    //perft(board, 7, START_FEN);
    //perft_split(board, 2, START_FEN);
    getchar();
  }
  else
  {
    GUI_Send("Unknown protocol\n");
  }
  
  return 0;
}
