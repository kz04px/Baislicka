#include "defs.h"
#include <string.h>

int moves_sort(s_move* moves, int num)
{
  assert(moves != NULL);
  assert(num >= 0);
  assert(num < MAX_MOVES);
  
  s_move store;
  
  int pos = 0;
  
  int i;
  
  // Put all captures first
  for(i = 0; i < num; ++i)
  {
    if(moves[i].type == CAPTURE)
    {
      store = moves[pos];
      moves[pos] = moves[i];
      moves[i] = store;
      
      pos++;
    }
  }
  
  // Put all queen captures first
  int pos_queen = 0;
  for(i = 0; i < pos; ++i)
  {
    if(moves[i].taken == QUEENS)
    {
      store = moves[pos_queen];
      moves[pos_queen] = moves[i];
      moves[i] = store;
      
      pos_queen++;
    }
  }
  
  int capturing_piece = 0;
  
  // Put all pawns capturing first
  for(i = 0; i < pos_queen; ++i)
  {
    if(moves[i].piece_type == wP || moves[i].piece_type == bP)
    {
      store = moves[capturing_piece];
      moves[capturing_piece] = moves[i];
      moves[i] = store;
      
      capturing_piece++;
    }
  }
  
  // Put all knights & bishops next
  for(i = capturing_piece; i < pos_queen; ++i)
  {
    if(moves[i].piece_type == KNIGHTS || moves[i].piece_type == BISHOPS)
    {
      store = moves[capturing_piece];
      moves[capturing_piece] = moves[i];
      moves[i] = store;
        
      capturing_piece++;
    }
  }
  
  // Put all rook captures second
  int pos_rook = pos_queen;
  for(i = pos_queen; i < pos; ++i)
  {
    if(moves[i].taken == ROOKS)
    {
      store = moves[pos_rook];
      moves[pos_rook] = moves[i];
      moves[i] = store;
      
      pos_rook++;
    }
  }
  
  capturing_piece = pos_queen;
  
  // Put all pawns capturing first
  for(i = pos_queen; i < pos_rook; ++i)
  {
    if(moves[i].piece_type == wP || moves[i].piece_type == bP)
    {
      store = moves[capturing_piece];
      moves[capturing_piece] = moves[i];
      moves[i] = store;
        
      capturing_piece++;
    }
  }
  
  // Put all knights & bishops next
  for(i = capturing_piece; i < pos_rook; ++i)
  {
    if(moves[i].piece_type == KNIGHTS || moves[i].piece_type == BISHOPS)
    {
      store = moves[capturing_piece];
      moves[capturing_piece] = moves[i];
      moves[i] = store;
      
      capturing_piece++;
    }
  }
  
  return 0;
}

int move_add_pawn_white(s_board* board, s_move* moves, int from, int to)
{
  assert(board != NULL);
  assert(moves != NULL);
  assert(from >= 0);
  assert(from <= 63);
  assert(to >= 0);
  assert(to <= 63);
  
  if(((uint64_t)1<<to)&U64_ROW_8)
  {
    moves[0] = add_promotion_move(board, from, to, wP, QUEENS);
    moves[1] = add_promotion_move(board, from, to, wP, KNIGHTS);
    moves[2] = add_promotion_move(board, from, to, wP, ROOKS);
    moves[3] = add_promotion_move(board, from, to, wP, BISHOPS);
    return 4;
  }
  else
  {
    moves[0] = add_movecapture(board, from, to, wP);
    return 1;
  }
}

int move_add_pawn_black(s_board* board, s_move* moves, int from, int to)
{
  assert(board != NULL);
  assert(moves != NULL);
  assert(from >= 0);
  assert(from <= 63);
  assert(to >= 0);
  assert(to <= 63);
  
  if(((uint64_t)1<<to)&U64_ROW_1)
  {
    moves[0] = add_promotion_move(board, from, to, bP, QUEENS);
    moves[1] = add_promotion_move(board, from, to, bP, KNIGHTS);
    moves[2] = add_promotion_move(board, from, to, bP, ROOKS);
    moves[3] = add_promotion_move(board, from, to, bP, BISHOPS);
    return 4;
  }
  else
  {
    moves[0] = add_movecapture(board, from, to, bP);
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
  int n;
  for(n = 2; n < 6; ++n)
  {
    if(board->combined[n] & ((uint64_t)1<<to))
    {
      move.taken = n;
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
  assert(piece_type == wP || piece_type == bP || piece_type == KNIGHTS || piece_type == BISHOPS || piece_type == ROOKS || piece_type == QUEENS || piece_type == KINGS);
  
  if(!(((uint64_t)1<<from)&board->colour[board->turn]))
  {
    display_board(board);
    print_u64(board->colour[WHITE]);
    print_u64(board->colour[BLACK]);
    getchar();
  }
  
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
    int n;
    for(n = 0; n < 6; ++n)
    {
      if(board->combined[n] & ((uint64_t)1<<to))
      {
        type = CAPTURE;
        taken = n;
        break;
      }
    }
  }
  else if(type == EP)
  {
    if(piece_type == wP)
    {
      taken = bP;
    }
    else
    {
      taken = wP;
    }
  }
  
  /*
  if(type == CAPTURE)
  {
    int n;
    for(n = 0; n < 6; ++n)
    {
      if(board->combined[n] & ((uint64_t)1<<to))
      {
        taken = n;
        break;
      }
    }
  }
  else if(type == EP)
  {
    if(piece_type == wP)
    {
      taken = bP;
    }
    else
    {
      taken = wP;
    }
  }
  */
  
  s_move move;
  move.from = from;
  move.to = to;
  move.type = type;
  move.taken = taken;
  move.piece_type = piece_type;
  return move;
}

void move_make(s_board *board, s_move *move)
{
  assert(board != NULL);
  assert(move != NULL);
  assert(move->from >= 0);
  assert(move->from <= 63);
  assert(move->to >= 0);
  assert(move->to <= 63);
  
  uint64_t from = (uint64_t)1<<(move->from);
  uint64_t to   = (uint64_t)1<<(move->to);
  
  // set old permissions
  #ifdef HASHTABLE
    move->key_old = board->key;
  #endif
  move->ep_old = board->ep;
  move->castling[wKSC] = board->castling[wKSC];
  move->castling[wQSC] = board->castling[wQSC];
  move->castling[bKSC] = board->castling[bKSC];
  move->castling[bQSC] = board->castling[bQSC];
  
  #ifdef HASHTABLE
    if(board->ep)
    {
      board->key ^= key_ep_col[u64_col(board->ep)];
    }
  #endif
  
  board->ep = 0;
  
  /*
  board->castling[wQSC] = !(from & U64_E1);
  board->castling[wKSC] = !(from & U64_E1);
  board->castling[wQSC] = !((to&U64_A1) | (from&U64_A1));
  board->castling[wKSC] = !((to&U64_H1) | (from&U64_H1));
  */
  
  // White king move
  if(from & U64_E1)
  {
    board->castling[wQSC] = 0;
    board->castling[wKSC] = 0;
  }
  // A1 move
  else if((to&U64_A1) || (from&U64_A1))
  {
    board->castling[wQSC] = 0;
  }
  // H1 move
  else if((to&U64_H1) || (from&U64_H1))
  {
    board->castling[wKSC] = 0;
  }
  
  // Black king move
  if(from & U64_E8)
  {
    board->castling[bQSC] = 0;
    board->castling[bKSC] = 0;
  }
  // A8 move
  else if((to&U64_A8) || (from&U64_A8))
  {
    board->castling[bQSC] = 0;
  }
  // H8 move
  else if((to&U64_H8) || (from&U64_H8))
  {
    board->castling[bKSC] = 0;
  }
  
  switch(move->type)
  {
    case QUIET:
      board->combined[move->piece_type] ^= from;
      board->combined[move->piece_type] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->to];
      #endif
      break;
    case CAPTURE:
      board->combined[move->piece_type] ^= from;
      board->combined[move->piece_type] ^= to;
      board->combined[move->taken] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      board->colour[!board->turn] ^= to;
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->to];
        board->key ^= key_piece_positions[move->taken][board->turn][move->to];
      #endif
      break;
    case DOUBLE_PAWN:
      board->combined[move->piece_type] ^= from;
      board->combined[move->piece_type] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      
      if(board->turn == WHITE)
      {
        board->ep = to>>8;
      }
      else
      {
        board->ep = to<<8;
      }
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->to];
        board->key ^= key_ep_col[u64_col(board->ep)];
      #endif
      break;
    case PROMOTE:
      board->combined[move->piece_type] ^= from;
      board->combined[move->promotion] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      
      if(move->taken != EMPTY)
      {
        board->combined[move->taken] ^= to;
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
      board->combined[move->piece_type] ^= from;
      board->combined[move->piece_type] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      
      if(board->turn == WHITE)
      {
        board->combined[bP] ^= to>>8;
        board->colour[BLACK] ^= to>>8;
      }
      else
      {
        board->combined[wP] ^= to<<8;
        board->colour[WHITE] ^= to<<8;
      }
      
      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->from];
        board->key ^= key_piece_positions[move->piece_type][board->turn][move->to];
        if(board->turn == WHITE)
        {
          board->key ^= key_piece_positions[bP][BLACK][move->to-8];
        }
        else
        {
          board->key ^= key_piece_positions[wP][WHITE][move->to+8];
        }
      #endif
      break;
    case KSC:
      board->combined[KINGS] ^= ksc_king[board->turn];
      board->combined[ROOKS] ^= ksc_rook[board->turn];
      board->colour[board->turn] ^= (ksc_king[board->turn] | ksc_rook[board->turn]);
      
      #ifdef HASHTABLE
        board->key ^= key_ksc[board->turn];
      #endif
      break;
    case QSC:
      board->combined[KINGS] ^= qsc_king[board->turn];
      board->combined[ROOKS] ^= qsc_rook[board->turn];
      board->colour[board->turn] ^= (qsc_king[board->turn] | qsc_rook[board->turn]);
      
      #ifdef HASHTABLE
        board->key ^= key_qsc[board->turn];
      #endif
      break;
  }
  
  #ifdef HASHTABLE
    if(board->castling[wKSC] != move->castling[wKSC]) {board->key ^= key_castling[wKSC];}
    if(board->castling[wQSC] != move->castling[wQSC]) {board->key ^= key_castling[wQSC];}
    if(board->castling[bKSC] != move->castling[bKSC]) {board->key ^= key_castling[bKSC];}
    if(board->castling[bQSC] != move->castling[bQSC]) {board->key ^= key_castling[bQSC];}
    board->key ^= key_turn;
  #endif
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
      board->combined[move->piece_type] ^= from;
      board->combined[move->piece_type] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      break;
    case CAPTURE:
      board->combined[move->piece_type] ^= from;
      board->combined[move->piece_type] ^= to;
      board->combined[move->taken] ^= to;
      board->colour[board->turn] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[!board->turn] ^= to;
      break;
    case DOUBLE_PAWN:
      board->combined[move->piece_type] ^= from;
      board->combined[move->piece_type] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      break;
    case PROMOTE:
      board->combined[move->piece_type] ^= from;
      board->combined[move->promotion] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      
      if(move->taken != EMPTY)
      {
        board->combined[move->taken] ^= to;
        board->colour[!board->turn] ^= to;
      }
      break;
    case EP:
      board->combined[move->piece_type] ^= from;
      board->combined[move->piece_type] ^= to;
      board->colour[board->turn] ^= from;
      board->colour[board->turn] ^= to;
      
      if(board->turn == WHITE)
      {
        board->combined[bP] ^= to>>8;
        board->colour[BLACK] ^= to>>8;
      }
      else
      {
        board->combined[wP] ^= to<<8;
        board->colour[WHITE] ^= to<<8;
      }
      break;
    case KSC:
      board->combined[KINGS] ^= ksc_king[board->turn];
      board->combined[ROOKS] ^= ksc_rook[board->turn];
      board->colour[board->turn] ^= (ksc_king[board->turn] | ksc_rook[board->turn]);
      
      #ifdef HASHTABLE
        board->key ^= key_ksc[board->turn];
      #endif
      break;
    case QSC:
      board->combined[KINGS] ^= qsc_king[board->turn];
      board->combined[ROOKS] ^= qsc_rook[board->turn];
      board->colour[board->turn] ^= (qsc_king[board->turn] | qsc_rook[board->turn]);
      
      #ifdef HASHTABLE
        board->key ^= key_qsc[board->turn];
      #endif
      break;
  }
  
  #ifdef HASHTABLE
    board->key = move->key_old;
  #endif
  board->ep = move->ep_old;
  board->castling[wKSC] = move->castling[wKSC];
  board->castling[wQSC] = move->castling[wQSC];
  board->castling[bKSC] = move->castling[bKSC];
  board->castling[bQSC] = move->castling[bQSC];
}

void move_make_ascii(s_board *board, char *move_string)
{
  assert(board != NULL);
  assert(move_string != NULL);

  int from_col = move_string[0] - 'a';
  int from_row = move_string[1] - '1';
  int to_col = move_string[2] - 'a';
  int to_row = move_string[3] - '1';
  
  assert(from_col >= 0);
  assert(from_col < 8);
  assert(from_row >= 0);
  assert(from_row < 8);
  assert(to_col >= 0);
  assert(to_col < 8);
  assert(to_row >= 0);
  assert(to_row < 8);
  
  uint64_t from = ((uint64_t)1<<(7-from_col))<<(8*from_row);
  uint64_t to = ((uint64_t)1<<(7-to_col))<<(8*to_row);
  
  assert(from);
  assert(to);
  
  assert(u64_col(from) == from_col);
  assert(u64_row(from) == from_row);
  assert(u64_col(to) == to_col);
  assert(u64_row(to) == to_row);
  
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
  for(i = 0; i < 7; ++i)
  {
    if(board->combined[i] & board->colour[piece_colour] & from)
    {
      piece_type = i;
    }
    if(board->combined[i] & board->colour[!piece_colour] & to)
    {
      piece_taken = i;
    }
  }
  
  if(piece_taken != EMPTY)
  {
    move_type = CAPTURE;
  }
  
  assert(piece_type >= 0);
  assert(piece_type < 7);
  
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
  
  if(piece_type == wP)
  {
    if(from_col != to_col)
    {
      // en passant
      if(piece_taken == EMPTY)
      {
        piece_taken = bP;
        move_type = EP;
      }
      else
      {
        move_type = CAPTURE;
      }
    }
    else if(to_row - from_row == 2)
    {
      move_type = DOUBLE_PAWN;
    }
  }
  else if(piece_type == bP)
  {
    if(from_col != to_col)
    {
      // en passant
      if(piece_taken == EMPTY)
      {
        piece_taken = wP;
        move_type = EP;
      }
      else
      {
        move_type = CAPTURE;
      }
    }
    else if(to_row - from_row == -2)
    {
      move_type = DOUBLE_PAWN;
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
  
  string[0] = POS_TO_COL_CHAR(move->from);
  string[1] = POS_TO_ROW_CHAR(move->from);
  string[2] = POS_TO_COL_CHAR(move->to);
  string[3] = POS_TO_ROW_CHAR(move->to);
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
  
  if((board->combined[move->piece_type] & ((uint64_t)1<<(move->from))) == 0)
  {
    return -1;
  }
  
  s_move move_list[MAX_MOVES];
  uint64_t allowed = ~board->colour[board->turn];
  
  int num_moves = 0;
  switch(move->piece_type)
  {
    case wP:
      num_moves = find_moves_wP_quiet(board, move_list);
      num_moves += find_moves_wP_captures(board, move_list);
      break;
    case bP:
      num_moves = find_moves_bP_quiet(board, move_list);
      num_moves += find_moves_bP_captures(board, move_list);
      break;
    case KNIGHTS:
      num_moves = find_moves_knights(board, move_list, allowed);
      break;
    case BISHOPS:
      num_moves = find_moves_bishops_queens(board, move_list, allowed);
      break;
    case ROOKS:
      num_moves = find_moves_rooks_queens(board, move_list, allowed);
      break;
    case QUEENS:
      num_moves = find_moves_bishops_queens(board, move_list, allowed);
      num_moves += find_moves_rooks_queens(board, move_list, allowed);
      break;
    case KINGS:
      num_moves = find_moves_kings_quiet(board, move_list);
      num_moves += find_moves_kings_captures(board, move_list);
      break;
  }
  
  int i;
  for(i = 0; i < num_moves; ++i)
  {
    if(move_list[i].from == move->from &&
       move_list[i].to == move->to &&
       move_list[i].type == move->type)
    {
      return 1;
    }
  }
  
  return 0;
  
  /*
  switch(move->type)
  {
    case QUIET:
      if((board->pieces[move->piece_type] & move->from) == 0)
        return -1;
      if(((board->colour[WHITE]|board->colour[BLACK]) & move->to) == 0)
        return -1;
      break;
    case DOUBLE_PAWN:
      break;
    case CAPTURE:
      break;
    case PROMOTE:
      break;
    case EP:
      break;
    case wKSC:
      if(!board->castling[wKSC])
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_F1)
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_G1)
        return -1;
      break;
    case wQSC:
      if(!board->castling[wQSC])
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_D1)
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_C1)
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_B1)
        return -1;
      break;
    case bKSC:
      if(!board->castling[bKSC])
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_F8)
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_G8)
        return -1;
      break;
    case bQSC:
      if(!board->castling[bQSC])
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_D8)
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_C8)
        return -1;
      if((board->colour[WHITE]|board->colour[BLACK])&U64_B8)
        return -1;
      break;
  }
  */
}
