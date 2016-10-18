#include "defs.h"
#include <string.h>

uint64_t moves_total;
uint64_t moves_quiet;
uint64_t moves_capture;
uint64_t moves_ep;
uint64_t moves_check;
uint64_t moves_wKSC;
uint64_t moves_wQSC;
uint64_t moves_bKSC;
uint64_t moves_bQSC;

void perft_search(s_board* board, int d)
{
  assert(board != NULL);
  assert(d > 0);
  assert(board->turn == WHITE || board->turn == BLACK);
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves_captures(board, &moves[0], board->turn);
  num_moves += find_moves_quiet(board, &moves[num_moves], board->turn);
  
  // Set old permissions
  #ifdef HASHTABLE
    uint64_t key_old = board->key;
  #endif
  uint8_t num_halfmoves_old = board->num_halfmoves;
  uint8_t ep_old = board->ep;
  uint8_t castling_old = board->castling;
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &moves[m]);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn))
    {
      // Restore old permissions
      #ifdef HASHTABLE
        board->key = key_old;
      #endif
      board->ep = ep_old;
      board->num_halfmoves = num_halfmoves_old;
      board->castling = castling_old;
      
      move_undo(board, &moves[m]);
      continue;
    }
    
    if(d == 1)
    {
      moves_total++;
    
      if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
      {
        moves_check++;
      }
      
      switch(moves[m].type)
      {
        case QUIET:
          moves_quiet++;
          break;
        case CAPTURE:
          moves_capture++;
          break;
        case EP:
          moves_capture++;
          moves_ep++;
          break;
        case KSC:
          if(board->turn == WHITE) {moves_wKSC++;}
          else                     {moves_bKSC++;}
          break;
        case QSC:
          if(board->turn == WHITE) {moves_wQSC++;}
          else                     {moves_bQSC++;}
          break;
        case PROMOTE:
          if(moves[m].taken != EMPTY) {moves_capture++;}
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
    
    // Restore old permissions
    #ifdef HASHTABLE
      board->key = key_old;
    #endif
    board->ep = ep_old;
    board->num_halfmoves = num_halfmoves_old;
    board->castling = castling_old;
    
    move_undo(board, &moves[m]);
  }
  
  return;
}

int perft_split(s_board* board, int depth, char* fen)
{
  assert(board != NULL);
  assert(depth > 0);
  assert(fen != NULL);
  
  int combined_total = 0;
  int r = set_fen(board, fen);
  if(r != 0)
  {
    printf("Invalid fen (%s)\n", fen);
    printf("Error code: %i\n", r);
    return r;
  }
  
  printf("Board:\n");
  display_board(board);
  
  time_t start;
  start = clock();
  
  s_move moves[MAX_MOVES];
  int num_moves = find_moves_captures(board, &moves[0], board->turn);
  num_moves += find_moves_quiet(board, &moves[num_moves], board->turn);
  
  printf("Num moves: %i\n\n", num_moves);
  
  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &moves[m]);
    
    if(square_attacked(board, board->pieces[KINGS]&board->colour[board->turn], !board->turn))
    {
      move_undo(board, &moves[m]);
      continue;
    }
    
    // Reset position and stats
    moves_total = 0;
    moves_quiet = 0;
    moves_capture = 0;
    moves_ep = 0;
    moves_check = 0;
    moves_wKSC = 0;
    moves_wQSC = 0;
    moves_bKSC = 0;
    moves_bQSC = 0;
    
    if(depth > 1)
    {
      board->turn = 1-(board->turn);
      perft_search(board, depth-1);
      board->turn = 1-(board->turn);
    }
    
    printf("%I64u - ", moves_total);
    print_move(moves[m]);
    
    move_undo(board, &moves[m]);
    
    combined_total += moves_total;
  }
  
  double time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
  printf("Time: %.3fs\n", time_taken);
  printf("Total: %i\n", combined_total);
  return 0;
}

void perft_suite(s_board* board, int max_depth, char* filepath)
{
  assert(board != NULL);
  assert(max_depth > 0);
  assert(filepath != NULL);
  
  FILE* file = fopen(filepath, "r");
  if(!file)
  {
    printf("Failed to find %s\n", filepath);
    printf("Aborting test suite\n");
    return;
  }
  
  time_t start;
  double time_taken;
  
  printf("Starting test suite\n");
  printf("\n");
  
  int num_tests = 0;
  int num_passed = 0;
  int num_failed = 0;
  
  start = clock();
  char line[1024];
  while(fgets(line, sizeof(line), file))
  {
    char* pch = strtok(line, ";");
    
    int r = set_fen(board, line);
    if(r != 0)
    {
      printf("Invalid fen (%s)\n", line);
      printf("Error code: %i\n", r);
      printf("Skipping test\n");
      continue;
    }
    num_tests++;
    
    printf("Test %i: ", num_tests);
    
    int pass = 1;
    
    while((pch = strtok(NULL, ";\n")))
    {
      int depth = 0;
      uint64_t result = 0;
      sscanf(pch+1, "%i %I64u", &depth, &result);
      
      if(depth > max_depth)
      {
        break;
      }
      
      moves_total = 0;  
      perft_search(board, depth);
      
      if(moves_total != result)
      {
        if(pass)
        {
          printf("failed\n");
        }
        
        printf(" %i: %I64u %I64u\n", depth, moves_total, result);
        
        pass = 0;
      }
    }
    
    if(pass)
    {
      printf("passed\n");
      num_passed++;
    }
    else
    {
      num_failed++;
    }
  }
  time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
  
  printf("\n");
  printf("Results:\n");
  printf("Location: %s\n", filepath);
  printf("Depth: %i\n", max_depth);
  printf("Time: %.3fs\n", time_taken);
  printf("Tests:  %i\n", num_tests);
  printf("Passed: %i (%.1f%%)\n", num_passed, 100*(float)num_passed/num_tests);
  printf("Failed: %i (%.1f%%)\n", num_failed, 100*(float)num_failed/num_tests);
}

void perft(s_board* board, int max_depth, char* fen)
{
  assert(board != NULL);
  assert(max_depth > 0);
  assert(fen != NULL);
  
  time_t start;
  double time_taken;
  
  printf("D   Time      NPS     Moves       Captures  EP     Castles Checks\n");
  int d;
  for(d = 1; d <= max_depth; ++d)
  {
    // Reset position & stats
    moves_total = 0;
    moves_quiet = 0;
    moves_capture = 0;
    moves_ep = 0;
    moves_check = 0;
    moves_wKSC = 0;
    moves_wQSC = 0;
    moves_bKSC = 0;
    moves_bQSC = 0;
    
    int r = set_fen(board, fen);
    if(r != 0)
    {
      printf("Invalid fen (%s)\n", fen);
      printf("Error code: %i\n", r);
      break;
    }
    
    // Test position
    start = clock();
    perft_search(board, d);
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    
    printf("%i  ", d);
    if(d < 10)  {printf(" ");}
    
    // Time taken
    printf("%.3fs  ", time_taken);
         if(time_taken < 10.0)  {printf("  ");}
    else if(time_taken < 100.0) {printf(" ");}
    
    // NPS
    if(time_taken < 0.005)
    {
      printf("-       ");
    }
    else
    {
      printf("%.1fm  ", (moves_total/time_taken)/1000000);
           if((moves_total/time_taken)/1000000 < 1)   {printf("   ");}
      else if((moves_total/time_taken)/1000000 < 10)  {printf("  ");}
      else if((moves_total/time_taken)/1000000 < 100) {printf(" ");}
    }
    
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

/*
 * The validity of this function might be questionable, as the
 * number of pieces and total moves for each side of the board
 * are not necessarily the same or even close. Use a large
 * enough number of positions and any positional variance should
 * balance out - if the times are still not approximately the
 * same then presumably the movegen functions are imbalanced
 */
int perft_movegen_sides(s_board* board, const char* filepath)
{
  assert(board != NULL);
  assert(filepath != NULL);
  
  time_t start;
  double time_taken;
  double time_total = 0;
  double time_white = 0;
  double time_black = 0;
  s_move moves[MAX_MOVES];
  int repeats = 5000000;
  int i;
  int test = 0;
  
  FILE* file = fopen(filepath, "r");
  if(file == NULL) {return -1;}
  
  printf("Test White  Black\n");
  char line[1024];
  while(fgets(line, sizeof(line), file))
  {
    //if(test == 20) {break;}
    
    char* pch = strtok(line, ";");
    
    int r = set_fen(board, pch);
    if(r != 0)
    {
      printf("Invalid fen (%s)\n", pch);
      printf("Error code: %i\n", r);
      printf("Skipping test\n");
      continue;
    }
    
    printf("%i", test);
         if(test < 10)   {printf("    ");}
    else if(test < 100)  {printf("   ");}
    else if(test < 1000) {printf("  ");}
    
    // White
    start = clock();
    for(i = 0; i < repeats; ++i)
    {
      //find_moves_white(board, moves); // FIXME
    }
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    time_total += time_taken;
    time_white += time_taken;
    printf("%.3f", time_taken);
    
    // Black
    start = clock();
    for(i = 0; i < repeats; ++i)
    {
      //find_moves_black(board, moves); // FIXME
    }
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    time_total += time_taken;
    time_black += time_taken;
    printf("  %.3f", time_taken);
    
    printf("\n");
    test++;
  }
  
  printf("\n");
  
  printf("Total: %.3f\n", time_total);
  printf("White: %.3f\n", time_white);
  printf("Black: %.3f\n", time_black);
  
  printf("\n");
  
  return 0;
}

int perft_movegen(s_board* board, const char* filepath)
{
  assert(board != NULL);
  assert(filepath != NULL);
  
  time_t start;
  double time_taken;
  double time_total;
  double time_pawn = 0;
  double time_knight = 0;
  double time_bishop = 0;
  double time_rook = 0;
  double time_queen = 0;
  double time_king = 0;
  s_move moves[MAX_MOVES];
  int repeats = 5000000;
  int i;
  int test = 0;
  
  FILE* file = fopen(filepath, "r");
  if(file == NULL) {return -1;}
  
  printf("Test Pawn   Knight Bishop Rook  King   Total\n");
  char line[1024];
  while(fgets(line, sizeof(line), file))
  {
    if(test == 20) {break;}
    time_total = 0;
    
    char* pch = strtok(line, ";");
    
    int r = set_fen(board, pch);
    if(r != 0)
    {
      printf("Invalid fen (%s)\n", pch);
      printf("Error code: %i\n", r);
      printf("Skipping test\n");
      continue;
    }
    
    printf("%i", test);
         if(test < 10)   {printf("    ");}
    else if(test < 100)  {printf("   ");}
    else if(test < 1000) {printf("  ");}
    
    // Pawns
    start = clock();
    for(i = 0; i < repeats; ++i)
    {
      find_moves_wP_quiet(board, moves);
      find_moves_pawn_captures(board, moves, board->colour[WHITE]);
      find_moves_bP_quiet(board, moves);
      find_moves_pawn_captures(board, moves, board->colour[BLACK]);
    }
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    time_total += time_taken;
    time_pawn += time_taken;
    printf("%.3f", time_taken);
    
    // Knight
    start = clock();
    for(i = 0; i < repeats; ++i)
    {
      //find_moves_wN(board, moves); // FIX ME
      //find_moves_bN(board, moves); // FIX ME
    }
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    time_total += time_taken;
    time_knight += time_taken;
    printf("  %.3f", time_taken);
    
    // Bishop & Queen
    start = clock();
    for(i = 0; i < repeats; ++i)
    {
      //find_moves_wB_wQ(board, moves); // FIX ME
      //find_moves_bB_bQ(board, moves); // FIX ME
    }
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    time_total += time_taken;
    time_bishop += time_taken;
    printf("  %.3f", time_taken);
    
    // Rook & Queen
    start = clock();
    for(i = 0; i < repeats; ++i)
    {
      //find_moves_wR_wQ(board, moves); // FIX ME
      //find_moves_bR_bQ(board, moves); // FIX ME
    }
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    time_total += time_taken;
    time_rook += time_taken;
    printf("  %.3f", time_taken);
    
    // King
    start = clock();
    for(i = 0; i < repeats; ++i)
    {
      //find_moves_wK(board, moves); // FIX ME
      //find_moves_bK(board, moves); // FIX ME
    }
    time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
    time_total += time_taken;
    time_king += time_taken;
    printf("  %.3f", time_taken);
    
    printf("  %.3f", time_total);
    
    printf("\n");
    test++;
  }
  
  printf("Sum  ");
  printf("%.3f", time_pawn);   if(time_pawn < 10)   {printf(" ");} printf(" ");
  printf("%.3f", time_knight); if(time_knight < 10) {printf(" ");} printf(" ");
  printf("%.3f", time_bishop); if(time_bishop < 10) {printf(" ");} printf(" ");
  printf("%.3f", time_rook);   if(time_rook < 10)   {printf(" ");} printf(" ");
  printf("%.3f", time_queen);  if(time_queen < 10)  {printf(" ");} printf(" ");
  printf("%.3f", time_king);   if(time_king < 10)   {printf(" ");} printf(" ");
  printf("%.3f", time_pawn + time_knight + time_bishop + time_rook + time_queen + time_king);
  
  printf("\n");
  
  return 0;
}

void perft_suite_search(s_board* board, int max_depth, char* filepath)
{
  assert(board != NULL);
  assert(max_depth > 0);
  assert(filepath != NULL);
  
  FILE* file = fopen(filepath, "r");
  if(!file)
  {
    printf("Failed to find %s\n", filepath);
    printf("Aborting test suite\n");
    return;
  }
  
  time_t start;
  double time_taken;
  
  printf("Starting test suite\n");
  printf("\n");
  
  int num_tests = 0;
  uint64_t nodes = 0;
  s_search_results results;
  
  s_move *bestmove = NULL;
  s_pv *bestmove_pv = NULL;
  int *bestmove_eval = NULL;
  
  start = clock();
  char line[1024];
  while(fgets(line, sizeof(line), file))
  {
    int r = set_fen(board, line);
    if(r != 0)
    {
      printf("Invalid fen (%s)\n", line);
      printf("Error code: %i\n", r);
      printf("Skipping test\n");
      continue;
    }
    num_tests++;
    
    printf("Test %i:  ", num_tests);
    
    s_search_info info;
    info.time_max = 10000000;
    search_info_set(info);
    
    hashtable_clear(hashtable);
    
    int i;
    for(i = 1; i <= max_depth; ++i)
    {
      results = search(board, i);
    }
    
    bestmove = &results.moves[results.best_move_num];
    bestmove_pv = &results.pvs[results.best_move_num];
    bestmove_eval = &results.evals[results.best_move_num];
    
         if(results.time_taken < 10) {printf("     ");}
    else if(results.time_taken < 100) {printf("    ");}
    else if(results.time_taken < 1000) {printf("   ");}
    else if(results.time_taken < 10000) {printf("  ");}
    else if(results.time_taken < 100000) {printf(" ");}
    printf("%ims", results.time_taken);
    
         if(abs(*bestmove_eval) < 10) {printf("    ");}
    else if(abs(*bestmove_eval) < 100) {printf("   ");}
    else if(abs(*bestmove_eval) < 1000) {printf("  ");}
    else if(abs(*bestmove_eval) < 10000) {printf(" ");}
    if(*bestmove_eval >= 0) {printf(" ");}
    printf("%icp   ", *bestmove_eval);
    
    print_move(*bestmove);
    
    nodes += results.nodes;
  }
  time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
  
  printf("\n");
  printf("Results:\n");
  printf("Location: %s\n", filepath);
  printf("Depth: %i\n", max_depth);
  printf("Nodes: %"PRIdPTR"\n", nodes);
  printf("kNPS:  %"PRIdPTR"\n", (uint64_t)(nodes/time_taken/1000.0));
  printf("Time:  %.3fs\n", time_taken);
  printf("Tests: %i\n", num_tests);
}