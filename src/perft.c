#include "defs.h"

#include <string.h>
#include <time.h>

U64 moves_total;
U64 moves_normal;
U64 moves_capture;
U64 moves_ep;
U64 moves_check;
U64 moves_wKSC;
U64 moves_wQSC;
U64 moves_bKSC;
U64 moves_bQSC;

s_move history[10];

void perft_search(s_board* board, int d)
{
  ASSERT(board != NULL);
  ASSERT(d > 0);
  ASSERT(board->turn == WHITE || board->turn == BLACK);
  
  /*
  if(board->pieces[bP] == U64_ROW_5)
  {
    printf("Depth: %i\n", d);
    print_move_list(history, 10);
    getchar();
  }
  */
  
  s_move move_list[MAX_MOVES];
  int num_moves = find_moves(board, move_list, board->turn);
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &move_list[m]);
    
    if(board->turn == WHITE)
    {
      if(calculate_attacked(board, board->pieces[wK], BLACK) == TRUE)
      {
        move_undo(board, &move_list[m]);
        continue;
      }
    }
    else
    {
      if(calculate_attacked(board, board->pieces[bK], WHITE) == TRUE)
      {
        move_undo(board, &move_list[m]);
        continue;
      }
    }
    
    history[d] = move_list[m];
    
    /*
    printf("Lala:\n");
    display_board(board);
    printf("\n");
    getchar();
    */
    
    /*
    if(board->pieces[wQ]&U64_H5)
    {
      display_board(board);
      printf("White in check: %i\n", calculate_attacked(board, board->pieces[bK], WHITE) == TRUE);
      printf("Black in check: %i\n", calculate_attacked(board, board->pieces[wK], BLACK) == TRUE);
      getchar();
    }
    */
  
    if(d == 1)
    {
      moves_total++;
      
      if(calculate_attacked(board, board->pieces[bK], WHITE) == TRUE || calculate_attacked(board, board->pieces[wK], BLACK) == TRUE)
      {
        moves_check++;
        //display_board(board);
        //getchar();
      }
      
      switch(move_list[m].type)
      {
        case NORMAL:
          moves_normal++;
          break;
        case CAPTURE:
          moves_capture++;
          break;
        case EP:
          moves_capture++;
          moves_ep++;
          break;
        case wKSC:
          moves_wKSC++;
          break;
        case wQSC:
          moves_wQSC++;
          break;
        case bKSC:
          moves_bKSC++;
          break;
        case bQSC:
          moves_bQSC++;
          break;
        default:
          break;
      }
    }
    else
    {      
      board->turn = 1-(board->turn);
      perft_search(board, d-1);
      board->turn = 1-(board->turn);
    }
    
    move_undo(board, &move_list[m]);
  }
  
  return;
}

void perft_split(s_board* board, int depth, const char* fen)
{
  ASSERT(board != NULL);
  ASSERT(depth > 0);
  ASSERT(fen != NULL);
  
  int combined_total = 0;
  set_fen(board, fen);
  
  printf("Board:\n");
  display_board(board);
  printf("\n");
  print_u64(board->ep);
  
  s_move move_list[MAX_MOVES];
  int num_moves = find_moves(board, move_list, board->turn);
  
  printf("Num moves: %i\n\n", num_moves);
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &move_list[m]);
    
    if(board->turn == WHITE)
    {
      if(calculate_attacked(board, board->pieces[wK], BLACK) == TRUE)
      {
        move_undo(board, &move_list[m]);
        continue;
      }
    }
    else
    {
      if(calculate_attacked(board, board->pieces[bK], WHITE) == TRUE)
      {
        move_undo(board, &move_list[m]);
        continue;
      }
    }
  
    // Reset position & stats
    moves_total = 0;
    moves_normal = 0;
    moves_capture = 0;
    moves_ep = 0;
    moves_check = 0;
    moves_wKSC = 0;
    moves_wQSC = 0;
    moves_bKSC = 0;
    moves_bQSC = 0;
    
    board->turn = 1-(board->turn);
    perft_search(board, depth-1);
    board->turn = 1-(board->turn);
    
    print_move(move_list[m]);
    printf("%I64u\n\n", moves_total);
    
    move_undo(board, &move_list[m]);
    
    combined_total += moves_total;
  }
  
  printf("Total: %i\n", combined_total);
  
  printf("Board:\n");
  display_board(board);
  printf("\n");
}

void perft_suite(s_board* board, int max_depth, const char* filepath)
{
  ASSERT(board != NULL);
  ASSERT(max_depth > 0);
  ASSERT(filepath != NULL);
  
  FILE* file = fopen(filepath, "r");
  if(!file)
  {
    printf("Failed to find %s\n", filepath);
    printf("Aborting test suite\n");
    return;
  }
  
  printf("Starting test suite\n");
  printf("Location: %s\n", filepath);
  printf("Depth: %i\n", max_depth);
  printf("\n");
  
  int num_tests = 0;
  int num_passed = 0;
  int num_failed = 0;
  
  char line[1024];
  while(fgets(line, sizeof(line), file))
  {
    char* pch = NULL;
    pch = strtok(line, ";");
    
    if(set_fen(board, line) == FALSE)
    {
      printf("Invalid fen (%s)\n", line);
      printf("Skipping test\n");
      continue;
    }
    num_tests++;
    
    printf("Test %i: ", num_tests);
    
    int pass = TRUE;
    
    while((pch = strtok(NULL, ";\n")))
    {
      int depth = -1;
      U64 result = -1;
      sscanf(pch+1, "%i %I64u", &depth, &result);
      
      if(depth > max_depth)
      {
        break;
      }
      
      moves_total = 0;  
      perft_search(board, depth);
      
      if(moves_total != result)
      {
        pass = FALSE;
      }
    }
    
    if(pass == TRUE)
    {
      printf("passed\n");
      num_passed++;
    }
    else
    {
      printf("failed\n");
      num_failed++;
    }
  }
  
  printf("\n");
  printf("Results:\n");
  printf("Tests:  %i\n", num_tests);
  printf("Passed: %i (%.1f%%)\n", num_passed, 100*(float)num_passed/num_tests);
  printf("Failed: %i (%.1f%%)\n", num_failed, 100*(float)num_failed/num_tests);
}

void perft(s_board* board, int max_depth, const char* fen)
{
  ASSERT(board != NULL);
  ASSERT(max_depth > 0);
  ASSERT(fen != NULL);
  
	time_t start;
	double time_taken;
  
  printf("D   Time     Moves       Captures  EP     Castles Checks\n");
  int d;
  for(d = 1; d <= max_depth; ++d)
  {
    // Reset position & stats
    moves_total = 0;
    moves_normal = 0;
    moves_capture = 0;
    moves_ep = 0;
    moves_check = 0;
    moves_wKSC = 0;
    moves_wQSC = 0;
    moves_bKSC = 0;
    moves_bQSC = 0;
    
    set_fen(board, fen);
    
    // Test position
    start = clock();
    perft_search(board, d);
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    
    printf("%i  ", d);
    if(d < 10)  {printf(" ");}
    
    // Time taken
    printf("%.3f  ", time_taken);
         if(time_taken < 10.0)  {printf("  ");}
    else if(time_taken < 100.0) {printf(" ");}
    
    printf("%I64u", moves_total);
         if(moves_total < 10)           {printf("           ");}
    else if(moves_total < 100)          {printf("          ");}
    else if(moves_total < 1000)         {printf("         ");}
    else if(moves_total < 10000)        {printf("        ");}
    else if(moves_total < 100000)       {printf("       ");}
    else if(moves_total < 1000000)      {printf("      ");}
    else if(moves_total < 10000000)     {printf("     ");}
    else if(moves_total < 100000000)    {printf("    ");}
    else if(moves_total < 1000000000)   {printf("   ");}
    else if(moves_total < 10000000000)  {printf("  ");}
    else if(moves_total < 100000000000) {printf(" ");}
    
    printf("%I64u", moves_capture);
         if(moves_capture < 10)         {printf("         ");}
    else if(moves_capture < 100)        {printf("        ");}
    else if(moves_capture < 1000)       {printf("       ");}
    else if(moves_capture < 10000)      {printf("      ");}
    else if(moves_capture < 100000)     {printf("     ");}
    else if(moves_capture < 1000000)    {printf("    ");}
    else if(moves_capture < 10000000)   {printf("   ");}
    else if(moves_capture < 100000000)  {printf("  ");}
    else if(moves_capture < 1000000000) {printf(" ");}
    
    printf("%I64u", moves_ep);
         if(moves_ep < 10)        {printf("      ");}
    else if(moves_ep < 100)       {printf("     ");}
    else if(moves_ep < 1000)      {printf("    ");}
    else if(moves_ep < 10000)     {printf("   ");}
    else if(moves_ep < 100000)    {printf("  ");}
    else if(moves_ep < 1000000)   {printf(" ");}
    
    printf("%I64u", moves_wKSC + moves_wQSC + moves_bKSC + moves_bQSC);
         if(moves_wKSC + moves_wQSC + moves_bKSC + moves_bQSC < 10)        {printf("       ");}
    else if(moves_wKSC + moves_wQSC + moves_bKSC + moves_bQSC < 100)       {printf("      ");}
    else if(moves_wKSC + moves_wQSC + moves_bKSC + moves_bQSC < 1000)      {printf("     ");}
    else if(moves_wKSC + moves_wQSC + moves_bKSC + moves_bQSC < 10000)     {printf("    ");}
    else if(moves_wKSC + moves_wQSC + moves_bKSC + moves_bQSC < 100000)    {printf("   ");}
    else if(moves_wKSC + moves_wQSC + moves_bKSC + moves_bQSC < 1000000)   {printf("  ");}
    else if(moves_wKSC + moves_wQSC + moves_bKSC + moves_bQSC < 10000000)  {printf(" ");}
    //printf("(%i %i %i %i)", moves_wKSC, moves_wQSC, moves_bKSC, moves_bQSC);
    
    printf("%I64u", moves_check);
         if(moves_check < 10)        {printf("      ");}
    else if(moves_check < 100)       {printf("     ");}
    else if(moves_check < 1000)      {printf("    ");}
    else if(moves_check < 10000)     {printf("   ");}
    else if(moves_check < 100000)    {printf("  ");}
    else if(moves_check < 1000000)   {printf(" ");}
    
    printf("\n");
  }
  printf("\n");
  
  return;
}