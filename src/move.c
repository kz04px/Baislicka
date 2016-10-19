#include "defs.h"
#include <string.h>

/*
#define wKSC 1
#define bKSC 2
#define wQSC 4
#define bQSC 8
*/

const uint8_t castling_perms[64] = {
// A  B  C  D  E  F  G  H
  11,15,15,15,10,15,15,14, // 1
  15,15,15,15,15,15,15,15, // 2
  15,15,15,15,15,15,15,15, // 3
  15,15,15,15,15,15,15,15, // 4
  15,15,15,15,15,15,15,15, // 5
  15,15,15,15,15,15,15,15, // 6
  15,15,15,15,15,15,15,15, // 7
   7,15,15,15, 5,15,15,13  // 8
};

// MVV-LVA (Most Valuable Victim - Least Valuable Aggressor)
int moves_sort(s_move* moves, int num)
{
  assert(moves != NULL);
  assert(num >= 0);
  assert(num < MAX_MOVES);
  
  if(num < 2) {return 0;}
  
  int sq = 0;
  
  int p_taken;
  for(p_taken = 5; p_taken >= 0; --p_taken)
  {
    int p_taking;
    for(p_taking = 0; p_taking < 7; ++p_taking)
    {
      int i;
      for(i = sq; i < num; ++i)
      {
        if(moves[i].piece_type == p_taking && moves[i].taken == p_taken)
        {
          s_move store = moves[sq];
          moves[sq] = moves[i];
          moves[i] = store;
          
          sq++;
        }
      }
    }
  }
  
  return 0;
}

int move_add_pawn(s_board* board, s_move* moves, int from, int to)
{
  assert(board != NULL);
  assert(moves != NULL);
  assert(from >= 0);
  assert(from <= 63);
  assert(to >= 0);
  assert(to <= 63);
  
  if(((uint64_t)1<<to)&(U64_RANK_1|U64_RANK_8))
  {
    moves[0] = add_promotion_move(board, from, to, PAWNS, QUEENS);
    moves[1] = add_promotion_move(board, from, to, PAWNS, KNIGHTS);
    moves[2] = add_promotion_move(board, from, to, PAWNS, ROOKS);
    moves[3] = add_promotion_move(board, from, to, PAWNS, BISHOPS);
    return 4;
  }
  else
  {
    moves[0] = add_movecapture(board, from, to, PAWNS);
    return 1;
  }
}

s_move add_promotion_move(s_board *board, int from, int to, int piece_type, int promo_piece)
{
  assert(board != NULL);
  assert(from >= 0);
  assert(from <= 63);
  assert(to >= 0);
  assert(to <= 63);
  
  s_move move;
  
  move.taken = EMPTY;
  int i;
  for(i = 1; i < 5; ++i)
  {
    if(board->pieces[i] & ((uint64_t)1<<to))
    {
      move.taken = i;
      break;
    }
  }
  
  move.from = from;
  move.to = to;
  move.type = PROMOTE;
  move.piece_type = piece_type;
  move.promotion = promo_piece;
  return move;
}

s_move add_movecapture(s_board* board, int from, int to, int piece_type)
{
  assert(board != NULL);
  assert(from >= 0);
  assert(from <= 63);
  assert(to >= 0);
  assert(to <= 63);
  assert(piece_type == PAWNS || piece_type == KNIGHTS || piece_type == BISHOPS || piece_type == ROOKS || piece_type == QUEENS || piece_type == KINGS);
  assert(((uint64_t)1<<from)&board->colour[board->turn]);
  
  if(board->colour[!board->turn]&((uint64_t)1<<to))
  {
    return move_add(board, from, to, CAPTURE, piece_type);
  }
  else
  {
    return move_add(board, from, to, QUIET, piece_type);
  }
}

s_move move_add(s_board *board, int from, int to, int type, int piece_type)
{
  assert(board != NULL);
  assert(from >= 0);
  assert(from <= 63);
  assert(to >= 0);
  assert(to <= 63);
  
  int taken = EMPTY;
  
  if(((uint64_t)1<<to) & (board->colour[WHITE]|board->colour[BLACK]))
  {
    int i;
    for(i = 0; i < 6; ++i)
    {
      if(board->pieces[i] & ((uint64_t)1<<to))
      {
        type = CAPTURE;
        taken = i;
        break;
      }
    }
  }
  else if(type == EP)
  {
    taken = PAWNS;
  }
  
  s_move move;
  move.from = from;
  move.to = to;
  move.type = type;
  move.taken = taken;
  move.piece_type = piece_type;
  move.promotion = EMPTY;
  return move;
}

void move_make(s_board *board, s_move *move)
{
  assert(board != NULL);
  assert(move != NULL);
  assert(move->from <= 63);
  assert(move->to <= 63);
  assert(move->piece_type <= KINGS);
  
  uint64_t from = (uint64_t)1<<(move->from);
  uint64_t to   = (uint64_t)1<<(move->to);
  
  #ifdef HASHTABLE
    if(board->ep)
    {
      board->key ^= key_ep_file[SQ_TO_FILE(board->ep)];
    }
    board->key ^= key_castling[board->castling];
  #endif
  
  board->ep = 0;
  board->num_halfmoves++;
  
  // Castling permissions
  board->castling &= castling_perms[move->from];
  board->castling &= castling_perms[move->to];
  
  #ifdef HASHTABLE
    board->key ^= key_castling[board->castling];
  #endif
  
  switch(move->type)
  {
    case QUIET:
      board->pieces[move->piece_type] ^= to | from;
      board->colour[board->turn] ^= to | from;
      
      if(move->piece_type == PAWNS)
      {
        board->num_halfmoves = 0;
      }
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->to];
      #endif
      break;
    case CAPTURE:
      assert(move->taken != KINGS);
      assert(move->taken != EMPTY);
    
      board->pieces[move->piece_type] ^= to | from;
      board->colour[board->turn] ^= to | from;
      
      board->pieces[move->taken] ^= to;
      board->colour[!board->turn] ^= to;
      
      board->num_halfmoves = 0;
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->to];
        board->key ^= key_piece_positions[move->taken][board->turn][move->to];
      #endif
      break;
    case DOUBLE_PAWN:
      board->pieces[move->piece_type] ^= to | from;
      board->colour[board->turn] ^= to | from;
      
      board->num_halfmoves = 0;
      
      if(board->turn == WHITE)
      {
        board->ep = move->to-8;
      }
      else
      {
        board->ep = move->to+8;
      }
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->to];
        board->key ^= key_ep_file[SQ_TO_FILE(board->ep)];
      #endif
      break;
    case PROMOTE:
      board->pieces[move->piece_type] ^= from;
      board->pieces[move->promotion] ^= to;
      board->colour[board->turn] ^= to | from;
      
      board->num_halfmoves = 0;
      
      if(move->taken != EMPTY)
      {
        board->pieces[move->taken] ^= to;
        board->colour[!board->turn] ^= to;
      }
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->promotion][board->turn][move->to];
        if(move->taken != EMPTY)
        {
          board->key ^= key_piece_positions[move->taken][board->turn][move->to];
        }
      #endif
      break;
    case EP:
      board->pieces[move->piece_type] ^= to | from;
      board->colour[board->turn] ^= to | from;
      
      board->num_halfmoves = 0;
      
      if(board->turn == WHITE)
      {
        board->pieces[PAWNS] ^= to>>8;
        board->colour[BLACK] ^= to>>8;
      }
      else
      {
        board->pieces[PAWNS] ^= to<<8;
        board->colour[WHITE] ^= to<<8;
      }
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->to];
        if(board->turn == WHITE)
        {
          board->key ^= key_piece_positions[PAWNS][BLACK][move->to-8];
        }
        else
        {
          board->key ^= key_piece_positions[PAWNS][WHITE][move->to+8];
        }
      #endif
      break;
    case KSC:
      board->pieces[KINGS] ^= ksc_king[board->turn];
      board->pieces[ROOKS] ^= ksc_rook[board->turn];
      board->colour[board->turn] ^= (ksc_king[board->turn] | ksc_rook[board->turn]);
      
      board->num_halfmoves = 0;
      
      #ifdef HASHTABLE
        board->key ^= key_ksc[board->turn];
      #endif
      break;
    case QSC:
      board->pieces[KINGS] ^= qsc_king[board->turn];
      board->pieces[ROOKS] ^= qsc_rook[board->turn];
      board->colour[board->turn] ^= (qsc_king[board->turn] | qsc_rook[board->turn]);
      
      board->num_halfmoves = 0;
      
      #ifdef HASHTABLE
        board->key ^= key_qsc[board->turn];
      #endif
      break;
  }
  
  #ifdef HASHTABLE
    /*
    // FIX ME
    if((board->castling & wKSC) != (move->castling & wKSC)) {board->key ^= key_castling[0];}
    if((board->castling & wQSC) != (move->castling & wQSC)) {board->key ^= key_castling[1];}
    if((board->castling & bKSC) != (move->castling & bKSC)) {board->key ^= key_castling[2];}
    if((board->castling & bQSC) != (move->castling & bQSC)) {board->key ^= key_castling[3];}
    */
    board->key ^= key_turn;
  #endif
  
  // History
  assert(board->history_size < HISTORY_SIZE_MAX);
  assert(board->history_size >= 0);
  board->key_history[board->history_size] = board->key;
  board->history_size++;
}

void move_undo(s_board *board, s_move *move)
{
  assert(board != NULL);
  assert(move != NULL);
  
  uint64_t from = (uint64_t)1<<(move->from);
  uint64_t to   = (uint64_t)1<<(move->to);
  
  switch(move->type)
  {
    case QUIET:
      board->pieces[move->piece_type] ^= to | from;
      board->colour[board->turn] ^= to | from;
      break;
    case CAPTURE:
      board->pieces[move->piece_type] ^= to | from;
      board->colour[board->turn] ^= to | from;
      board->pieces[move->taken] ^= to;
      board->colour[!board->turn] ^= to;
      break;
    case DOUBLE_PAWN:
      board->pieces[move->piece_type] ^= to | from;
      board->colour[board->turn] ^= to | from;
      break;
    case PROMOTE:
      board->pieces[move->piece_type] ^= from;
      board->pieces[move->promotion] ^= to;
      board->colour[board->turn] ^= to | from;
      
      if(move->taken != EMPTY)
      {
        board->pieces[move->taken] ^= to;
        board->colour[!board->turn] ^= to;
      }
      break;
    case EP:
      board->pieces[move->piece_type] ^= to | from;
      board->colour[board->turn] ^= to | from;
      
      if(board->turn == WHITE)
      {
        board->pieces[PAWNS] ^= to>>8;
        board->colour[BLACK] ^= to>>8;
      }
      else
      {
        board->pieces[PAWNS] ^= to<<8;
        board->colour[WHITE] ^= to<<8;
      }
      break;
    case KSC:
      board->pieces[KINGS] ^= ksc_king[board->turn];
      board->pieces[ROOKS] ^= ksc_rook[board->turn];
      board->colour[board->turn] ^= (ksc_king[board->turn] | ksc_rook[board->turn]);
      
      #ifdef HASHTABLE
        board->key ^= key_ksc[board->turn];
      #endif
      break;
    case QSC:
      board->pieces[KINGS] ^= qsc_king[board->turn];
      board->pieces[ROOKS] ^= qsc_rook[board->turn];
      board->colour[board->turn] ^= (qsc_king[board->turn] | qsc_rook[board->turn]);
      
      #ifdef HASHTABLE
        board->key ^= key_qsc[board->turn];
      #endif
      break;
  }
  
  // History
  board->history_size--;
  assert(board->history_size >= 0);
}

void move_make_ascii(s_board *board, char *move_string)
{
  assert(board != NULL);
  assert(move_string != NULL);

  int from_file = move_string[0] - 'a';
  int from_rank = move_string[1] - '1';
  int to_file = move_string[2] - 'a';
  int to_rank = move_string[3] - '1';
  
  assert(from_file >= 0);
  assert(from_file < 8);
  assert(from_rank >= 0);
  assert(from_rank < 8);
  assert(to_file >= 0);
  assert(to_file < 8);
  assert(to_rank >= 0);
  assert(to_rank < 8);
  
  uint64_t from = ((uint64_t)1<<from_file)<<(8*from_rank);
  uint64_t to = ((uint64_t)1<<to_file)<<(8*to_rank);
  
  assert(from);
  assert(to);
  
  assert(u64_file(from) == from_file);
  assert(u64_rank(from) == from_rank);
  assert(u64_file(to) == to_file);
  assert(u64_rank(to) == to_rank);
  
  int piece_type = -1;
  int piece_colour = -1;
  int piece_taken = EMPTY;
  int move_type = QUIET;
  int promo_piece = -1;
  
  // Find piece colour
  if(board->colour[WHITE] & from)
  {
    piece_colour = WHITE;
  }
  else if(board->colour[BLACK] & from)
  {
    piece_colour = BLACK;
  }
  
  int i;
  for(i = 0; i < 6; ++i)
  {
    if(board->pieces[i] & board->colour[piece_colour] & from)
    {
      piece_type = i;
    }
    if(board->pieces[i] & board->colour[!piece_colour] & to)
    {
      piece_taken = i;
    }
  }
  
  if(piece_taken != EMPTY)
  {
    move_type = CAPTURE;
  }
  
  assert(piece_type >= 0);
  assert(piece_type < 6);
  
  if(strlen(move_string) > 4)
  {
    if(move_string[4] == 'Q' || move_string[4] == 'q')
    {
      promo_piece = QUEENS;
    }
    else if(move_string[4] == 'R' || move_string[4] == 'r')
    {
      promo_piece = ROOKS;
    }
    else if(move_string[4] == 'B' || move_string[4] == 'b')
    {
      promo_piece = BISHOPS;
    }
    else if(move_string[4] == 'N' || move_string[4] == 'n')
    {
      promo_piece = KNIGHTS;
    }
  }
  
  if(piece_type == KINGS && piece_colour == WHITE)
  {
    if(from == U64_E1 && to == U64_G1)
    {
      move_type = KSC;
    }
    else if(from == U64_E1 && to == U64_C1)
    {
      move_type = QSC;
    }
  }
  else if(piece_type == KINGS && piece_colour == BLACK)
  {
    if(from == U64_E8 && to == U64_G8)
    {
      move_type = KSC;
    }
    else if(from == U64_E8 && to == U64_C8)
    {
      move_type = QSC;
    }
  }
  
  if(piece_type == PAWNS)
  {
    if(piece_colour == WHITE)
    {
      if(from_file != to_file)
      {
        // en passant
        if(piece_taken == EMPTY)
        {
          piece_taken = PAWNS;
          move_type = EP;
        }
        else
        {
          move_type = CAPTURE;
        }
      }
      else if(to_rank - from_rank == 2)
      {
        move_type = DOUBLE_PAWN;
      }
    }
    else
    {
      if(from_file != to_file)
      {
        // en passant
        if(piece_taken == EMPTY)
        {
          piece_taken = PAWNS;
          move_type = EP;
        }
        else
        {
          move_type = CAPTURE;
        }
      }
      else if(to_rank - from_rank == -2)
      {
        move_type = DOUBLE_PAWN;
      }
    }
  }
  
  if(promo_piece != -1)
  {
    move_type = PROMOTE;
  }
  
  s_move move;
  move.from = __builtin_ctzll(from);
  move.to = __builtin_ctzll(to);
  move.type = move_type;
  move.taken = piece_taken;
  move.piece_type = piece_type;
  move.promotion = promo_piece;
  
  move_make(board, &move);
}

int move_to_string(char* string, s_move *move)
{
  assert(string != NULL);
  assert(move != NULL);
  
  string[0] = SQ_TO_FILE_CHAR(move->from);
  string[1] = SQ_TO_RANK_CHAR(move->from);
  string[2] = SQ_TO_FILE_CHAR(move->to);
  string[3] = SQ_TO_RANK_CHAR(move->to);
  string[4] = '\0';
  
  if(move->type == PROMOTE)
  {
    if(move->promotion == QUEENS)  {string[4] = 'Q';}
    if(move->promotion == ROOKS)   {string[4] = 'R';}
    if(move->promotion == BISHOPS) {string[4] = 'B';}
    if(move->promotion == KNIGHTS) {string[4] = 'N';}
    string[5] = '\0';
  }
  
  return 0;
}

int move_is_legal(s_board* board, s_move* move)
{
  assert(board != NULL);
  assert(move != NULL);
  
  /*
  uint64_t from = (uint64_t)1<<move->from;
  uint64_t to = (uint64_t)1<<move->to;
  
  if(!(board->pieces[move->piece_type] & from)) {return 0;}
  if(!(board->colour[board->turn] & from)) {return 0;}
  
  return 1;
  */
  
  s_move move_list[MAX_MOVES];
  uint64_t allowed = ~board->colour[board->turn];
  
  int num_moves = 0;
  switch(move->piece_type)
  {
    case PAWNS:
      if(board->turn == WHITE)
      {
        num_moves = find_moves_wP_quiet(board, &move_list[0]);
      }
      else
      {
        num_moves = find_moves_bP_quiet(board, &move_list[0]);
      }
      num_moves += find_moves_pawn_ep(board, &move_list[num_moves]);
      num_moves += find_moves_pawn_captures(board, &move_list[num_moves], allowed);
      break;
    case KNIGHTS:
      num_moves = find_moves_knights(board, &move_list[0], allowed);
      break;
    case BISHOPS:
      num_moves = find_moves_bishops_queens(board, &move_list[0], allowed);
      break;
    case ROOKS:
      num_moves = find_moves_rooks_queens(board, &move_list[0], allowed);
      break;
    case QUEENS:
      num_moves = find_moves_bishops_queens(board, &move_list[0], allowed);
      num_moves += find_moves_rooks_queens(board, &move_list[num_moves], allowed);
      break;
    case KINGS:
      num_moves = find_moves_kings(board, &move_list[0], allowed);
      num_moves += find_moves_kings_castles(board, &move_list[num_moves]);
      break;
  }
  
  int i;
  for(i = 0; i < num_moves; ++i)
  {
    if(move_list[i].from == move->from &&
       move_list[i].to == move->to &&
       move_list[i].type == move->type &&
       move_list[i].taken == move->taken &&
       move_list[i].piece_type == move->piece_type &&
       move_list[i].promotion == move->promotion)
    {
      return 1;
    }
  }
  
  return 0;
}
