#include "defs.h"

typedef struct
{
	U64 mask;
	U64 number;
	int shifts;
	U64 moves[64];
} s_square;

int main(int argc, char *argv[])
{
	//printf("Ready\n");
	//getchar();
	
	// Initialise stuff
	s_board *board = (s_board*) malloc(1*sizeof(s_board));
	
	// Magic bitboard test
	set_fen(board, START_FEN);
	
	//set_fen(board, START_FEN);
	//negaMax_base(board);
	//perft(board, 7);
	
	//perft_threaded_search(START_FEN, 7, 1);
  printf("Starting\n");
	perft_suite(board, "perftsuite.epd", 5);
	printf("Finishing\n");
  
	/*
	printf("Board:\n");
	display_board(board);
	printf("\n");
	
	printf("White attacking:\n");
	display_bitboard(board->attacking[wB]);
	printf("\n");
	
	printf("White attacking:\n");
	display_bitboard(board->attacking[wQ]);
	printf("\n");
	*/
	
	/*
	printf("Black attacking:\n");
	display_bitboard(board->all_attacking[BLACK]);
	printf("\n");
	*/
	
	/*
	// Check the moves of a specific position
	printf("White attacking:\n");
	display_bitboard(board->all_attacking[WHITE]);
	printf("\n");
	
	printf("Black attacking:\n");
	display_bitboard(board->all_attacking[BLACK]);
	printf("\n");
	
	s_move moves[MAX_MOVES];
	int num_moves = find_moves(board, moves);
	printf("Total moves: %i\n", num_moves);
	
	int i;
	for(i = 0; i < num_moves; ++i)
	{
		make_move(board, &moves[i]);
		//update_attacking(board);
		
		if(board->turn == BLACK)
		{
			if(calculate_attacked(board, board->pieces[wK], BLACK) == TRUE)
			{
				printf("Invalid: ");
				//undo_move(board, &moves[i]);
				//continue;
			}
		}
		else
		{
			if(calculate_attacked(board, board->pieces[bK], WHITE) == TRUE)
			{
				printf("Invalid: ");
				//undo_move(board, &moves[i]);
				//continue;
			}
		}
		
		move_print(&moves[i]);
		undo_move(board, &moves[i]);
	}*/
	
	/*
	// Standard perft
	//set_fen(board, "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
	set_fen(board, START_FEN);
	printf("Board:\n");
	display_board(board);
	perft(board, 7);
	printf("Board:\n");
	display_board(board);
	*/
	
	/*
	// Print individual move counts for each starting move
	set_fen(board, "rn1qkbnr/p1pppppp/b7/1p6/8/P3P3/1PPP1PPP/RNBQKBNR w KQkq - 0 3");
	s_move moves[MAX_MOVES];
	int num_moves = find_moves(board, moves);
	int i;
	U64 total = 0;
	display_board(board);
	printf("\n");
	printf("Num moves: %i\n", num_moves);
	for(i = 0; i < num_moves; ++i)
	{
		make_move(board, &moves[i]);
		update_attacking(board);
		move_print(&moves[i]);
		s_moves_found found = perft_exact(board, 0);
		total += found.total;
		printf("Found: %I64u\n", found.total);
		printf("\n");
		undo_move(board, &moves[i]);
		update_attacking(board);
	}
	printf("Total: %I64u\n", total);
	*/
	
	/*
	// Manual play
	int r = set_fen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1");
	printf("r: %i\n", r);
	printf("wP:\n");
	display_bitboard(board->pieces[wP]);
	printf("bP:\n");
	display_bitboard(board->pieces[bP]);
	while(TRUE)
	{
		printf("Board:\n");
		display_board(board);
		printf("\n");
		U64 total = 0;
		
		s_move moves[MAX_MOVES];
		int num_moves = find_moves(board, moves);
		int i;
		printf("Num moves: %i\n", num_moves);
		for(i = 0; i < num_moves; ++i)
		{
			make_move(board, &moves[i]);
			update_attacking(board);
			move_print(&moves[i]);
			U64 found = perft_exact(board, 1);
			total += found;
			printf("Found: %I64u\n", found);
			printf("\n");
			undo_move(board, &moves[i]);
			update_attacking(board);
		}
		printf("Total: %I64u\n", total);
		int choice;
		scanf("%i", &choice);
		
		printf("Chosen move:\n");
		printf("type: %i\n", moves[choice].type);
		printf("piece: %i\n", moves[choice].piece);
		printf("promotion: %i\n", moves[choice].promotion);
		printf("taken: %i\n", moves[choice].taken);
		printf("to:\n");
		display_bitboard(moves[choice].to);
		printf("from:\n");
		display_bitboard(moves[choice].from);
		
		printf("wP before:\n");
		display_bitboard(board->pieces[wP]);
		printf("bP before:\n");
		display_bitboard(board->pieces[bP]);
		make_move(board, &moves[choice]);
		printf("wP after:\n");
		display_bitboard(board->pieces[wP]);
		printf("bP after:\n");
		display_bitboard(board->pieces[bP]);
		update_attacking(board);
	}
	*/
	
	/*
	set_fen(board, "8/8/8/8/8/8/6k1/4K2R w K - 0 1");
	printf("Board:\n");
	display_board(board);
	printf("\n");
	
	s_move moves[MAX_MOVES];
	int num_moves = find_moves(board, moves);
	
	printf("\nmoves found: %i\n", num_moves);
	int i;
	for(i = 0; i < num_moves; ++i)
	{
		if(i < 9) {printf(" %i: ", i+1);}
		else      {printf("%i: ", i+1);}
		move_print(&moves[i]);
	}
	*/
	
	getchar();
	return 0;
}