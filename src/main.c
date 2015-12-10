#include "defs.h"

int main()
{
  printf("Baislicka\n\n");
  
  printf("Bitboards:\n");
  printf("- generateOccupancyVariations()\n");
  printf("- generateMoveDatabase()\n");
  generateOccupancyVariations(0);
  generateMoveDatabase(0);
  generateOccupancyVariations(1);
  generateMoveDatabase(1);
  printf("\n");
  
  s_board* board = (s_board*) malloc(1*sizeof(s_board));
  if(board == NULL) {return -1;}
  
  /*
  printf("Game:\n");
  printf("- creating game\n");
  printf("- setting fen\n");
  set_fen(board, START_FEN);
  printf("\n");
  */
  
  // perft
  perft(board, 7, START_FEN);
  //perft_split(board, 3, "rnbqkbnr/1ppppppp/p7/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");
  //perft_split(board, 2, "r1bqkbnr/pppppppp/2n5/8/1P6/2N5/P1PPPPPP/R1BQKBNR b KQkq b3 0 2");
  
  
  printf("Board:\n");
  display_board(board);
  printf("\n");
  
  
  /*
  set_fen(board, START_FEN);
  s_move* move_list = (s_move*) malloc(MAX_MOVES*sizeof(s_move));
  printf("Generating moves:\n");
  int num_moves = find_moves(board, move_list, WHITE);
  printf("- %i moves found\n", num_moves);
  printf("\n");
  printf("Moves:\n");
  print_move_list(move_list, num_moves);
  */
  
  printf("Done\n");
  getchar();
  return 0;
}

