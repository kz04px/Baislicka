#include "defs.h"
#include <string.h>

int board_flip(s_board *board)
{
  assert(board);
  
  // Side to move
  board->turn = 1-board->turn;
  
  // En passant square
  board->ep = board->ep^56;
  
  // Colours
  uint64_t store = board->colour[WHITE];
  board->colour[WHITE]   = __builtin_bswap64(board->colour[BLACK]);
  board->colour[BLACK]   = __builtin_bswap64(store);
  
  // Pieces
  board->pieces[PAWNS]   = __builtin_bswap64(board->pieces[PAWNS]);
  board->pieces[KNIGHTS] = __builtin_bswap64(board->pieces[KNIGHTS]);
  board->pieces[BISHOPS] = __builtin_bswap64(board->pieces[BISHOPS]);
  board->pieces[ROOKS]   = __builtin_bswap64(board->pieces[ROOKS]);
  board->pieces[QUEENS]  = __builtin_bswap64(board->pieces[QUEENS]);
  board->pieces[KINGS]   = __builtin_bswap64(board->pieces[KINGS]);
  
  // Castling perms
  uint8_t castling_old = board->castling;
  board->castling = 0;
  if(castling_old&wKSC) {board->castling ^= bKSC;}
  if(castling_old&wQSC) {board->castling ^= bQSC;}
  if(castling_old&bKSC) {board->castling ^= wKSC;}
  if(castling_old&bQSC) {board->castling ^= wQSC;}
  
  #ifdef HASHTABLE
    board->key = create_key_board(board);
  #endif
  
  return 0;
}

int test_move_legality(s_board *board, const char *filepath)
{
  assert(board);
  assert(filepath);
  
  set_fen(board, START_FEN);
  if(is_legal_move(board, &NO_MOVE)) {return -1;}
  
  FILE *file = fopen(filepath, "r");
  if(!file)
  {
    return -1;
  }
  
  char line[1024];
  while(fgets(line, sizeof(line), file))
  {
    if(line[0] == '#' || strlen(line) < 5) {continue;}
    
    int r = set_fen(board, line);
    if(r) {continue;}
    
    s_move moves[MAX_MOVES];
    int num_moves = find_moves_captures(board, &moves[0], board->turn);
    num_moves += find_moves_quiet(board, &moves[num_moves], board->turn);
    
    int m;
    for(m = 0; m < num_moves; ++m)
    {
      int legal = is_legal_move(board, &moves[m]);
      if(!legal) {fclose(file); return -2;}
    }
  }
  
  fclose(file);
  return 0;
}

int test_eval_mirroring(s_board *board, const char *filepath)
{
  assert(board);
  assert(filepath);
  
  FILE *file = fopen(filepath, "r");
  if(!file)
  {
    return -1;
  }
  
  char line[1024];
  while(fgets(line, sizeof(line), file))
  {
    if(line[0] == '#' || strlen(line) < 5) {continue;}
    
    int r = set_fen(board, line);
    if(r) {continue;}
    
    int eval_normal = eval(board);
    
    board_flip(board);
    
    int eval_mirror = eval(board);
    
    if(eval_normal != eval_mirror) {fclose(file); return -2;}
  }
  
  fclose(file);
  return 0;
}

int test_backward_pawns(s_board *board)
{
  assert(board);
  
  const char positions[16][256] = {{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"},
                                   {"k7/8/3p4/4p3/4P3/8/8/K7 w - -"},
                                   {"k7/8/8/4p3/4P3/8/3P4/K7 w - -"},
                                   {"k7/8/4p3/3p2p1/4P3/5P2/8/K7 w - -"}};
  const uint64_t passed_white[16] = {0, U64_E4, 0, U64_F3};
  const uint64_t passed_black[16] = {0, U64_D6, 0, U64_G5};
  
  int i;
  for(i = 0; i < 16; ++i)
  {
    int r = set_fen(board, positions[i]);
    if(r) {continue;}
    
    uint64_t found_white = 0;
    uint64_t found_black = 0;
    uint64_t copy;
    
    // White pawns
    copy = board->pieces[PAWNS] & board->colour[WHITE];
    while(copy)
    {
      int sq = __builtin_ctzll(copy);
      uint64_t pos = (copy & ~(copy-1));
      
      if(is_backward_pawn_white(sq, board->pieces[PAWNS]&board->colour[WHITE], board->pieces[PAWNS]&board->colour[BLACK]))
      {
        found_white ^= pos;
      }
      
      copy ^= pos;
    }
    
    // Black pawns
    copy = board->pieces[PAWNS] & board->colour[BLACK];
    while(copy)
    {
      int sq = __builtin_ctzll(copy);
      uint64_t pos = (copy & ~(copy-1));
      
      if(is_backward_pawn_black(sq, board->pieces[PAWNS]&board->colour[BLACK], board->pieces[PAWNS]&board->colour[WHITE]))
      {
        found_black ^= pos;
      }
      
      copy ^= pos;
    }
    
    if(found_white != passed_white[i] || found_black != passed_black[i])
    {
      return i+1;
    }
  }
  
  return 0;
}

int test_passed_pawns(s_board *board)
{
  assert(board);
  
  const char positions[16][256] = {{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"},
                                   {"k7/8/8/4p3/4P3/8/2P5/K7 w - -"},
                                   {"k7/1p3p1p/8/8/4P3/5P2/2P5/K7 w - -"}};
  const uint64_t passed_white[16] = {0, U64_C2, 0};
  const uint64_t passed_black[16] = {0, 0, U64_H7};
  
  int i;
  for(i = 0; i < 16; ++i)
  {
    int r = set_fen(board, positions[i]);
    if(r) {continue;}
    
    uint64_t found_white = 0;
    uint64_t found_black = 0;
    uint64_t copy;
    
    // White pawns
    copy = board->pieces[PAWNS] & board->colour[WHITE];
    while(copy)
    {
      int sq = __builtin_ctzll(copy);
      uint64_t pos = (copy & ~(copy-1));
      
      if(is_passed_pawn(WHITE, sq, board->pieces[PAWNS]|board->colour[BLACK]))
      {
        found_white ^= pos;
      }
      
      copy ^= pos;
    }
    
    // Black pawns
    copy = board->pieces[PAWNS] & board->colour[BLACK];
    while(copy)
    {
      int sq = __builtin_ctzll(copy);
      uint64_t pos = (copy & ~(copy-1));
      
      if(is_passed_pawn(BLACK, sq, board->pieces[PAWNS]|board->colour[WHITE]))
      {
        found_black ^= pos;
      }
      
      copy ^= pos;
    }
    
    if(found_white != passed_white[i] || found_black != passed_black[i])
    {
      return i+1;
    }
  }
  
  return 0;
}

int test_all()
{
  s_board *board = (s_board*) malloc(1*sizeof(s_board));
  if(board == NULL) {return -1;}
  
  #ifdef NDEBUG
    printf("Suggest not compiling with -DNDEBUG\n");
  #endif
  
  #define TEST_PASS(x) (((x) == 0) ? "Passed" : "Failed")
  int r;
  
  printf("Tests:\n");
  
  printf(" 1) Move legality:  ");
  r = test_move_legality(board, "perftsuite.epd");
  printf("%s (%i)\n", TEST_PASS(r), r);
  
  printf(" 2) Eval mirroring: ");
  r = test_eval_mirroring(board, "perftsuite.epd");
  printf("%s (%i)\n", TEST_PASS(r), r);
  
  printf(" 3) Perftsuite 3:   ");
  r = perft_suite(board, 3, "perftsuite.epd", 0);
  printf("%s (%i)\n", TEST_PASS(r), r);
  
  printf(" 4) Backward pawns: ");
  r = test_backward_pawns(board);
  printf("%s (%i)\n", TEST_PASS(r), r);
  
  printf(" 5) Passed pawns:   ");
  r = test_passed_pawns(board);
  printf("%s (%i)\n", TEST_PASS(r), r);
  
  printf("\n");
  
  free(board);
  return 0;
}
