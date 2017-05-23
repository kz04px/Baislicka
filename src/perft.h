#ifndef PERFT_H_INCLUDED
#define PERFT_H_INCLUDED

void perft(s_board *board, int max_depth, char *fen);
int perft_split(s_board *board, int depth, char *fen);
int perft_suite(s_board *board, int max_depth, char *filepath, int output);
void perft_suite_search(s_board *board, int max_depth, char *filepath);
int perft_movegen(s_board *board, const char *filepath);
int perft_movegen_sides(s_board *board, const char *filepath);

#endif
