#include "defs.h"

#define INF 999999

int negaMax(s_board *board, int alpha, int beta, int depth_left)
{
	ASSERT(board != NULL);
	ASSERT(depth_left >= 0);
	
	if(depth_left == 0)
	{
		return evaluate(board);
	}
	
    int best_score = -INF;
	int move_made = FALSE;
	
	s_move moves[MAX_MOVES];
	int num_moves = find_moves(board, moves);
	
	int n;
	for(n = 0; n < num_moves; ++n)
	{
		make_move(board, &moves[n]);
		
		if(in_check(board, 1-board->turn) == TRUE)
		{
			undo_move(board, &moves[n]);
			continue;
		}
		
		move_made = TRUE;
		int score = -negaMax(board, -beta, -alpha, depth_left-1);
		
		undo_move(board, &moves[n]);
		
		if(score > best_score)
		{
			best_score = score;
		}
		if(score > alpha)
		{
			alpha = score;
		}
		if(alpha > beta)
		{
			break; // Remove to disable alpha-beta pruning apparently
		}
	}
	
	// if stalemate
	if(move_made == FALSE && (in_check(board, WHITE) == FALSE && in_check(board, BLACK) == FALSE))
	{
		return 0;
	}
	
	// if checkmate
	if(best_score == INF)
	{
		return best_score - (MAX_DEPTH - depth_left);
	}
	
	return best_score;
}

int negaMax_base(s_board *board)
{
	s_move moves[MAX_MOVES];
	s_move *best_move = NULL;
	int num_moves = find_moves(board, moves);
	int best_score = -INF;
	int search_depth = 3;
	
	int n;
	for(n = 0; n < num_moves; ++n)
	{
		make_move(board, &moves[n]);
		
		if(in_check(board, 1-board->turn) == TRUE)
		{
			undo_move(board, &moves[n]);
			continue;
		}
		
		int score = -negaMax(board, -INF, INF, search_depth-1);
		
		if(score > best_score)
		{
			best_score = score;
			best_move = &moves[n];
		}
		
		undo_move(board, &moves[n]);
	}
	
	move_print(best_move);
	
	return 0;
}
