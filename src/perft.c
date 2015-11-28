#include "defs.h"
#include <string.h>
#include <time.h>
#include <stdint.h> // uint64_t
#include <pthread.h>

void perft_search(s_board *board, int depth, s_moves_found *results)
{
	int made_move = -1;
	s_move moves[MAX_MOVES];
	int num_moves = find_moves(board, moves);
	
	int n;
	for(n = 0; n < num_moves; ++n)
	{
		make_move(board, &moves[n]);
		
		if(board->turn == BLACK)
		{
			if(calculate_attacked(board, board->pieces[wK], BLACK) == TRUE)
			{
				undo_move(board, &moves[n]);
				continue;
			}
		}
		else
		{
			if(calculate_attacked(board, board->pieces[bK], WHITE) == TRUE)
			{
				undo_move(board, &moves[n]);
				continue;
			}
		}
		
		/*
		update_attacking(board);
		
		if(board->turn == BLACK)
		{
			if(black_attacking(board, board->pieces[wK]))
			{			
				undo_move(board, &moves[n]);
				//update_attacking(board);
				continue;
			}
		}
		else
		{
			if(white_attacking(board, board->pieces[bK]))
			{
				undo_move(board, &moves[n]);
				//update_attacking(board);
				continue;
			}
		}
		*/
		
		made_move = 0;
		
		if(depth > 1)
		{
			perft_search(board, depth-1, results);
		}
		else
		{
			results->total++;
			
			switch(moves[n].type)
			{
				case NORMAL:  {results->normal++; break;}
				case CAPTURE: {results->takes++; break;}
				case EP:      {results->takes++; results->eps++; break;}
				case wKSC:    {results->wKSCs++; break;}
				case wQSC:    {results->wQSCs++; break;}
				case bKSC:    {results->bKSCs++; break;}
				case bQSC:    {results->bQSCs++; break;}
				case PROMOTE:
				{
					if(moves[n].taken != EMPTY)
					{
						results->takes++;
					}
					results->promotions++;
					break;
				}
				default: {break;}
			}
			
			if(board->turn == BLACK) // FIXME: Do we really need to check colour here?
			{
				if(calculate_attacked(board, board->pieces[bK], WHITE))
				//if(board->pieces[bK]&board->all_attacking[WHITE])
				{
					results->checks++;
				}
			}
			else
			{
				if(calculate_attacked(board, board->pieces[wK], BLACK))
				//if(board->pieces[wK]&board->all_attacking[BLACK])
				{
					results->checks++;
				}
			}
		}
		
		undo_move(board, &moves[n]);
		//update_attacking(board); // FIXME: This isn't necessary?
	}

	if(made_move != 0 && depth == 1)
	{
		results->checkmates++;
	}
}

s_moves_found perft_exact(s_board *board, int depth)
{
	ASSERT(board != NULL);
	ASSERT(depth < MAX_DEPTH);

	s_moves_found results;
	results.total = 0;
	results.normal = 0;
	results.takes = 0;
	results.checks = 0;
	results.eps = 0;
	results.checkmates = 0;
	results.promotions = 0;
	results.wKSCs = 0;
	results.wQSCs = 0;
	results.bKSCs = 0;
	results.bQSCs = 0;
	perft_search(board, depth, &results);
	return results;
}

void perft(s_board *board, int depth)
{
	ASSERT(board != NULL);
	ASSERT(depth < MAX_DEPTH);

	printf("Depth Time     Nodes      nps  (Take : EP : Castle : Promotion: Check : CM)\n");
	//printf("Depth\tTime\tNodes\t(Takes : EPs : Castles : Promotions: Checks : CMs)\n");

	s_moves_found results;
	
	time_t start;
	double time_taken;

	int i;
	for(i = 1; i <= depth; ++i)
	{
		results.total = 0;
		results.normal = 0;
		results.takes = 0;
		results.checks = 0;
		results.eps = 0;
		results.checkmates = 0;
		results.promotions = 0;
		results.wKSCs = 0;
		results.wQSCs = 0;
		results.bKSCs = 0;
		results.bQSCs = 0;
		
		start = clock();
		perft_search(board, i, &results);
		time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
		
		printf("%i    ", i);
		if(i < 10) {printf(" ");}
		
		printf("%.3fs ", time_taken);
		     if(time_taken < 10.0) {printf("  ");}
		else if(time_taken < 100.0) {printf(" ");}
		
		printf("%I64u ", results.total);
		     if(results.total < 10) {printf("         ");}
		else if(results.total < 100) {printf("        ");}
		else if(results.total < 1000) {printf("       ");}
		else if(results.total < 10000) {printf("      ");}
		else if(results.total < 100000) {printf("     ");}
		else if(results.total < 1000000) {printf("    ");}
		else if(results.total < 10000000) {printf("   ");}
		else if(results.total < 100000000) {printf("  ");}
		else if(results.total < 1000000000) {printf(" ");}
		
		// nps
		if(time_taken < 0.001)
		{
			printf("-    ");
		}
		else
		{
			results.total = results.total/time_taken;
			int count = 0;
			while(results.total >= 1000)
			{
				count++;
				results.total = results.total/1000;
			}
			
			printf("%I64u", results.total);
			
			     if(count == 1) {printf("k ");}
			else if(count == 2) {printf("m ");}
			else if(count == 3) {printf("b ");}
			else if(count == 4) {printf("t ");}
			else                {printf("? ");}
			
			     if(results.total < 10) {printf("  ");}
			else if(results.total < 100) {printf(" ");}
		}
		
		printf("(%I64u : %I64u : %I64u : %I64u : %I64u : %I64u)\n", results.takes, results.eps, (results.wKSCs+results.wQSCs+results.bKSCs+results.bQSCs), results.promotions, results.checks, results.checkmates);
	}
}

void perft_suite(s_board *board, char *filename, int max_depth)
{
	ASSERT(board != NULL);
	ASSERT(filename != NULL);
	ASSERT(max_depth >= 0);

	FILE *our_file = NULL;
	our_file = fopen(filename, "r");
	if(our_file == NULL)
	{
		printf("\"%s\" not found\n", filename);
		return;
	}
	printf("%s loaded\n", filename);
	printf("Depth: %i\n\n", max_depth);

	int tests = 0;
	int passed = 0;
	int failed = 0;
	int errors = 0;
	char line[512];
	char found[32];
	time_t start = clock();
	while(fgets(line, sizeof(line), our_file))
	{
		tests++;
		char *current = strtok(line, ";");
		//printf("Test %i (%s):\n", tests, current);
		int current_depth = 1;
		
		int r = set_fen(board, current);
		if(set_fen(board, current) != 0)
		{
			printf("Current: %s\n", current);
			printf("Test: %i\n", tests);
			printf("Line: %s\n", line);
			printf("Error: %i\n", r);
			errors++;
			continue;
		}
		
		//display_board(board);
		current = strtok(NULL, ";");

		while(current != NULL)
		{
			s_moves_found results;
			results.total = 0;
			sprintf(found, "%I64u", results.total);
			perft_search(board, current_depth, &results);
			
			int i;
			for(i = strlen(current+3); i > 1; --i)
			{
				if((char)(results.total%10+'0') != *(current+i+1))
				{
					failed++;
					printf("depth(%i) got(%I64u) expected(%s)\n", current_depth, results.total, current+3);
					current = NULL;
					break;
				}
				results.total = results.total/10;
				if(i == 2 && current_depth >= max_depth) {passed++;}
			}

			if(current_depth >= max_depth) {break;}
			current_depth++;
			current = strtok(NULL, ";");
		}
	}

	failed = tests-passed-errors;

	printf("Tests:	%i\n", tests);
	printf("Passed: %i(%.4g%%)\n", passed, 100*(float)passed/tests);
	printf("Failed: %i(%.4g%%)\n", failed, 100*(float)failed/tests);
	printf("Errors: %i(%.4g%%)\n", errors, 100*(float)errors/tests);
	printf("Time: %.3fs\n", (double)(clock()-start)/CLOCKS_PER_SEC);
}

typedef struct
{
	int depth;
	int counter;
	int num_boards;
	s_moves_found results;
	s_board boards[MAX_MOVES];
} s_queue;

s_queue queue;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

void perft_thread()
{
	int board_num;
	U64 found = 0;
	s_moves_found results;
	
	while(TRUE)
	{	
		pthread_mutex_lock(&running_mutex);
		board_num = queue.counter;
		queue.counter++;
		pthread_mutex_unlock(&running_mutex);
		
		if(board_num >= queue.num_boards) {break;}
		//printf("Searching: %i/%i\n", board_num+1, queue.num_boards);
		//results = perft_exact(&queue.boards[board_num], queue.depth);
		results.total = 0;
		results.normal = 0;
		results.takes = 0;
		results.checks = 0;
		results.checkmates = 0;
		results.eps = 0;
		results.promotions = 0;
		results.wKSCs = 0;
		results.wQSCs = 0;
		results.bKSCs = 0;
		results.bQSCs = 0;
		perft_search(&queue.boards[board_num], queue.depth, &results);
		
		pthread_mutex_lock(&running_mutex);
		queue.results.total += results.total;
		queue.results.normal += results.normal;
		queue.results.takes += results.takes;
		queue.results.checks += results.checks;
		queue.results.checkmates += results.checkmates;
		queue.results.eps += results.eps;
		queue.results.promotions += results.promotions;
		queue.results.wKSCs += results.wKSCs;
		queue.results.wQSCs += results.wQSCs;
		queue.results.bKSCs += results.bKSCs;
		queue.results.bQSCs += results.bQSCs;
		pthread_mutex_unlock(&running_mutex);
	}
}

void perft_threaded_search(char *fen, int depth, int num_threads)
{
	int i;
	s_board *board = (s_board*) malloc(1*sizeof(s_board));
	set_fen(board, fen);
	s_move moves[MAX_MOVES];
	int num_moves = find_moves(board, moves);
	time_t start;
	
	printf("Board:\n");
	display_board(board);
	printf("FEN: %s\n", fen);
	printf("Depth: %i\n", depth);
	printf("Threads: %i\n", num_threads);
	printf("Depth\tTime\tNodes\t(Takes : EPs : Castles : Promotions: Checks : CMs)\n");
	
	int n;
	for(n = 0; n < depth; ++n)
	{
		// Set queue
		queue.counter = 0;
		queue.depth = n;
		queue.num_boards = num_moves;
		// Reset results
		queue.results.total = 0;
		queue.results.normal = 0;
		queue.results.takes = 0;
		queue.results.checks = 0;
		queue.results.checkmates = 0;
		queue.results.eps = 0;
		queue.results.promotions = 0;
		queue.results.wKSCs = 0;
		queue.results.wQSCs = 0;
		queue.results.bKSCs = 0;
		queue.results.bQSCs = 0;
		
		if(n == 0)
		{
			perft_search(board, 1, &queue.results);
		}
		else
		{
			// Set boards
			for(i = 0; i < queue.num_boards; ++i)
			{
				set_fen(&queue.boards[i], fen);
				make_move(&queue.boards[i], &moves[i]);
				update_attacking(&queue.boards[i]);
			}
			
			// Create threads
			pthread_t tid[num_threads];
			for(i = 0; i < num_threads; ++i)
			{
				pthread_create(&tid[i], NULL, (void *)perft_thread, NULL);
			}
			
			// Wait for search to finish
			start = clock();
			for(i = 0; i < num_threads; ++i)
			{
				pthread_join(tid[i], NULL);
			}
		}
		
		printf("%i\t", n+1);
		if(n == 0)
		{
			printf("%.3fs\t", 0.0);
		}
		else
		{
			printf("%.3fs\t", (double)(clock()-start)/CLOCKS_PER_SEC);
		}
		printf("%I64u\t", queue.results.total);
		printf("(%I64u : %I64u : %I64u : %I64u : %I64u : %I64u)\n", queue.results.takes, queue.results.eps, (queue.results.wKSCs+queue.results.wQSCs+queue.results.bKSCs+queue.results.bQSCs), queue.results.promotions, queue.results.checks, queue.results.checkmates);
	}
}
