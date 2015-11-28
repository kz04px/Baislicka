#include "defs.h"

void Add_Piece(s_board *board, int type, int Location)
{
	SETBIT(board->pieces[type], Location);
	SETBIT(board->all_pieces[COLOUR(type)], Location);
	SETBIT(board->all_pieces[BOTH], Location);
	board->locations[type][board->num_pieces[type]] = Location;
	board->num_pieces[type]++;
}

void Init_Board(s_board *board)
{
}

int U64_to_Col(U64 pos)
{
	     if(pos&U64_COL_A) {return 0;}
	else if(pos&U64_COL_B) {return 1;}
	else if(pos&U64_COL_C) {return 2;}
	else if(pos&U64_COL_D) {return 3;}
	else if(pos&U64_COL_E) {return 4;}
	else if(pos&U64_COL_F) {return 5;}
	else if(pos&U64_COL_G) {return 6;}
	else if(pos&U64_COL_H) {return 7;}
	else {return -1;}
}

int U64_to_Row(U64 pos)
{
	     if(pos&U64_ROW_1) {return 0;}
	else if(pos&U64_ROW_2) {return 1;}
	else if(pos&U64_ROW_3) {return 2;}
	else if(pos&U64_ROW_4) {return 3;}
	else if(pos&U64_ROW_5) {return 4;}
	else if(pos&U64_ROW_6) {return 5;}
	else if(pos&U64_ROW_7) {return 6;}
	else if(pos&U64_ROW_8) {return 7;}
	else {return -1;}
}