#include "defs.h"
#include <string.h>

int main()
{
  srand(0x480ec5cb);
  
  bitboards_init();
  
  #ifdef HASHTABLE
    key_init();
    hashtable = malloc(1*sizeof(*hashtable));
    hashtable->entries = NULL;
    
    int size = HASHTABLE_SIZE_DEFAULT;
    while(size >= HASHTABLE_SIZE_MIN)
    {
      int r = hashtable_init(hashtable, size);
      
      if(r != -1) {break;}
      
      size = size>>1;
    }
  #endif
  
  ksc_king[WHITE] = U64_E1 | U64_G1;
  ksc_king[BLACK] = U64_E8 | U64_G8;
  
  ksc_rook[WHITE] = U64_F1 | U64_H1;
  ksc_rook[BLACK] = U64_F8 | U64_H8;
  
  qsc_king[WHITE] = U64_E1 | U64_C1;
  qsc_king[BLACK] = U64_E8 | U64_C8;
  
  qsc_rook[WHITE] = U64_D1 | U64_A1;
  qsc_rook[BLACK] = U64_D8 | U64_A8;
  
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
    
    printf("Principal variation - enabled\n");
    
    #ifdef NULL_MOVE
      printf("Null move pruning - enabled\n");
    #else
      printf("Null move pruning - disabled\n");
    #endif
    
    printf("\n");
    
    printf("Other info:\n");
    printf("sizeof(s_board): %"PRIdPTR"B\n", sizeof(s_board)); // 160B
    printf("sizeof(s_move):  %"PRIdPTR"B\n", sizeof(s_move));  //  64B
    #ifdef HASHTABLE
      printf("sizeof(s_hashtable_entry):  %"PRIdPTR"B\n", sizeof(s_hashtable_entry));
      printf("Hashtable entries per MB:  %"PRIdPTR"\n", 1024*1024/sizeof(s_hashtable_entry));
    #endif
    printf("\n");
  #endif
  
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
      GUI_Send("option name Hash type spin default %i min %i max %i\n", HASHTABLE_SIZE_DEFAULT, HASHTABLE_SIZE_MIN, HASHTABLE_SIZE_MAX);
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
    
    perft_suite_search(board, 8, "perftsuite_2.epd");
    //perft_suite(board, 6, "perftsuite.epd");
    //perft(board, 7, START_FEN);
    //perft_split(board, 6, START_FEN);
    //perft_movegen(board, "perftsuite.epd");
    //perft_movegen_sides(board, "perftsuite.epd");
    
    /*
    set_fen(board, START_FEN);
    
    s_search_info info;
    info.time_max = 10000000;
    search_info_set(info);
    uint64_t nodes_last = 0;
    
    printf("Search started\n");
    
    int i;
    for(i = 1; i <= 9; ++i)
    {
      printf("Depth: %i\n", i);
      s_search_results results = search(board, i);
      
      //int m;
      //for(m = 0; m < results.num_moves; ++m)
      //{
      //  printf("%i  ", results.evals[m]);
      //  print_move(results.moves[m]);
      //}
      
      printf("  Search time: %ims\n", results.time_taken);
      printf("  Nodes: %"PRIdPTR"\n", results.nodes);
      if(i == 1)
      {
        printf("  Branching: -\n");
      }
      else
      {
        printf("  Branching: %.3f\n", (float)results.nodes/nodes_last);
      }
      nodes_last = results.nodes;
      #ifdef HASHTABLE
        printf("  Hash entries: %i (%.2f%%)\n", hashtable->num_entries, 100.0*(float)hashtable->num_entries/hashtable->max_entries);
      #endif
      if(results.time_taken > 0)
      {
        printf("  kNPS: %"PRIdPTR"\n", results.nodes/results.time_taken);
      }
      
      printf("  Eval: %i\n", results.evals[results.best_move_num]);
      
      char move_string[4096];
      char temp[16];
      move_string[0] = '\0';
      int n;
      for(n = 0; n < results.pvs[results.best_move_num].num_moves; ++n)
      {
        move_to_string(temp, &results.pvs[results.best_move_num].moves[n]);
        sprintf(move_string, "%s %s", move_string, temp);
      }
      printf("  pv: %s\n", move_string);
      
      if(results.out_of_time == 1)
      {
        printf("  Out of time :<\n");
      }
      printf("\n");
      
      //display_board(board);
      //print_u64(board->colour[WHITE]);
      //printf("\n");
      //print_u64(board->colour[BLACK]);
      //printf("\n");
      //getchar();
    }
    */
    
    /*
    set_fen(board, START_FEN);
    
    while(1)
    {
      printf("Board:\n");
      display_board(board);
      //printf("White:\n");
      //print_u64(board->colour[WHITE]);
      //printf("Black:\n");
      //print_u64(board->colour[BLACK]);
      
      printf("is 3fold: %i\n", is_threefold(board));
      
      s_move moves[MAX_MOVES];
      int num_moves = find_moves(board, moves, board->turn);
      moves_sort(moves, num_moves);
      
      int i;
      for(i = 0; i < num_moves; ++i)
      {
        printf("%i: ", i);
        print_move(moves[i]);
      }
      
      int move_number;
      scanf("%d", &move_number);
      
      move_make(board, &moves[move_number]);
      board->turn = 1-board->turn;
    }
    */
    
    getchar();
  }
  else
  {
    GUI_Send("Unknown protocol\n");
  }
  
  return 0;
}
