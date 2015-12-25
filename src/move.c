#include "defs.h"

int move_add_pawn_white(s_board* board, s_move* move_list, uint64_t from, uint64_t to)
{
  ASSERT(board != NULL);
  ASSERT(move_list != NULL);
  
  if(to&U64_ROW_8)
  {
    move_list[0] = add_promotion_move(board, from, to, wP, wQ);
    move_list[1] = add_promotion_move(board, from, to, wP, wN);
    move_list[2] = add_promotion_move(board, from, to, wP, wR);
    move_list[3] = add_promotion_move(board, from, to, wP, wB);
    return 4;
  }
  else
  {
    move_list[0] = add_movecapture_white(board, from, to, wP);
    return 1;
  }
}

int move_add_pawn_black(s_board* board, s_move* move_list, uint64_t from, uint64_t to)
{
  ASSERT(board != NULL);
  ASSERT(move_list != NULL);
  
  if(to&U64_ROW_1)
  {
    move_list[0] = add_promotion_move(board, from, to, bP, bQ);
    move_list[1] = add_promotion_move(board, from, to, bP, bN);
    move_list[2] = add_promotion_move(board, from, to, bP, bR);
    move_list[3] = add_promotion_move(board, from, to, bP, bB);
    return 4;
  }
  else
  {
    move_list[0] = add_movecapture_black(board, from, to, bP);
    return 1;
  }
}

s_move add_promotion_move(s_board *board, uint64_t from, uint64_t to, int piece_type, int promo_piece)
{
  ASSERT(board != NULL);
  ASSERT(from);
  ASSERT(to);
  
  s_move move;
  
  move.taken = EMPTY;
  int n;
  for(n = 0; n < 12; ++n)
  {
    if(board->pieces[n]&to)
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

s_move add_movecapture_white(s_board* board, uint64_t from, uint64_t to, int piece_type)
{
  ASSERT(board != NULL);
  ASSERT(from);
  ASSERT(to);
  ASSERT(piece_type == wP || piece_type == wN || piece_type == wB || piece_type == wR || piece_type == wQ || piece_type == wK);
  
  if(board->pieces_colour[BLACK]&to)
  {
    return move_add(board, from, to, CAPTURE, piece_type);
  }
  else
  {
    return move_add(board, from, to, QUIET, piece_type);
  }
}

s_move add_movecapture_black(s_board* board, uint64_t from, uint64_t to, int piece_type)
{
  ASSERT(board != NULL);
  ASSERT(from);
  ASSERT(to);
  ASSERT(piece_type == bP || piece_type == bN || piece_type == bB || piece_type == bR || piece_type == bQ || piece_type == bK);
  
  if(board->pieces_colour[WHITE]&to)
  {
    return move_add(board, from, to, CAPTURE, piece_type);
  }
  else
  {
    return move_add(board, from, to, QUIET, piece_type);
  }
}

s_move move_add(s_board *board, uint64_t from, uint64_t to, int type, int piece_type)
{
  ASSERT(board != NULL);
  ASSERT(from);
  ASSERT(to);
  
  int taken = EMPTY;
  if(type == CAPTURE)
  {
    int n;
    for(n = 0; n < 12; ++n)
    {
      if(board->pieces[n]&to) {taken = n; break;}
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
  ASSERT(board != NULL);
  ASSERT(move != NULL);
  
  // set old permissions
  move->ep_old = board->ep;
  move->castling[wKSC] = board->castling[wKSC];
  move->castling[wQSC] = board->castling[wQSC];
  move->castling[bKSC] = board->castling[bKSC];
  move->castling[bQSC] = board->castling[bQSC];
  board->ep = 0;
  
  // White king move
  if((move->from) & U64_E1)
  {
    board->castling[wQSC] = 0;
    board->castling[wKSC] = 0;
  }
  // A1 move
  else if((move->to&U64_A1) || (move->from&U64_A1))
  {
    board->castling[wQSC] = 0;
  }
  // H1 move
  else if((move->to&U64_H1) || (move->from&U64_H1))
  {
    board->castling[wKSC] = 0;
  }
  
  // Black king move
  if(move->from & U64_E8)
  {
    board->castling[bQSC] = 0;
    board->castling[bKSC] = 0;
  }
  // A8 move
  else if((move->to&U64_A8) || (move->from&U64_A8))
  {
    board->castling[bQSC] = 0;
  }
  // H8 move
  else if((move->to&U64_H8) || (move->from&U64_H8))
  {
    board->castling[bKSC] = 0;
  }
  
  switch(move->type)
  {
    case QUIET:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->piece_type] ^= move->to;
      break;
    case CAPTURE:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->piece_type] ^= move->to;
      board->pieces[move->taken] ^= move->to;
      break;
    case DOUBLE_PAWN:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->piece_type] ^= move->to;
      // Add ep square
      if(move->piece_type == wP)
      {
        board->ep = (move->to)>>8;
      }
      else
      {
        board->ep = (move->to)<<8;
      }      
      break;
    case PROMOTE:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->promotion] ^= move->to;
      if(move->taken != EMPTY)
      {
        board->pieces[move->taken] ^= move->to;
      }
      break;
    case EP:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->piece_type] ^= move->to;
      if(move->piece_type == wP)
      {
        board->pieces[bP] ^= (move->to)>>8;
      }
      else
      {
        board->pieces[wP] ^= (move->to)<<8;
      }
      break;
    case wKSC:
      board->pieces[wK] ^= U64_E1;
      board->pieces[wK] ^= U64_G1;
      board->pieces[wR] ^= U64_H1;
      board->pieces[wR] ^= U64_F1;
      break;
    case wQSC:
      board->pieces[wK] ^= U64_E1;
      board->pieces[wK] ^= U64_C1;
      board->pieces[wR] ^= U64_A1;
      board->pieces[wR] ^= U64_D1;
      break;
    case bKSC:
      board->pieces[bK] ^= U64_E8;
      board->pieces[bK] ^= U64_G8;
      board->pieces[bR] ^= U64_H8;
      board->pieces[bR] ^= U64_F8;
      break;
    case bQSC:
      board->pieces[bK] ^= U64_E8;
      board->pieces[bK] ^= U64_C8;
      board->pieces[bR] ^= U64_A8;
      board->pieces[bR] ^= U64_D8;
      break;
  }
  
  board->pieces_colour[WHITE] = board->pieces[wP] | board->pieces[wN] | board->pieces[wB] |
                                board->pieces[wR] | board->pieces[wQ] | board->pieces[wK];
  
  board->pieces_colour[BLACK] = board->pieces[bP] | board->pieces[bN] | board->pieces[bB] |
                                board->pieces[bR] | board->pieces[bQ] | board->pieces[bK];
  
  board->pieces_all = board->pieces_colour[WHITE] | board->pieces_colour[BLACK];
}

void move_undo(s_board *board, s_move *move)
{  
  switch(move->type)
  {
    case QUIET:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->piece_type] ^= move->to;
      break;
    case CAPTURE:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->piece_type] ^= move->to;
      board->pieces[move->taken] ^= move->to;
      break;
    case DOUBLE_PAWN:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->piece_type] ^= move->to;
      break;
    case PROMOTE:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->promotion] ^= move->to;
      if(move->taken != EMPTY)
      {
        board->pieces[move->taken] ^= move->to;
      }
      break;
    case EP:
      board->pieces[move->piece_type] ^= move->from;
      board->pieces[move->piece_type] ^= move->to;
      if(move->piece_type == wP)
      {
        board->pieces[bP] ^= (move->to)>>8;
      }
      else
      {
        board->pieces[wP] ^= (move->to)<<8;
      }
      break;
    case wKSC:
      board->pieces[wK] ^= U64_E1;
      board->pieces[wK] ^= U64_G1;
      board->pieces[wR] ^= U64_H1;
      board->pieces[wR] ^= U64_F1;
      break;
    case wQSC:
      board->pieces[wK] ^= U64_E1;
      board->pieces[wK] ^= U64_C1;
      board->pieces[wR] ^= U64_A1;
      board->pieces[wR] ^= U64_D1;
      break;
    case bKSC:
      board->pieces[bK] ^= U64_E8;
      board->pieces[bK] ^= U64_G8;
      board->pieces[bR] ^= U64_H8;
      board->pieces[bR] ^= U64_F8;
      break;
    case bQSC:
      board->pieces[bK] ^= U64_E8;
      board->pieces[bK] ^= U64_C8;
      board->pieces[bR] ^= U64_A8;
      board->pieces[bR] ^= U64_D8;
      break;
  }
  
  board->ep = move->ep_old;
  board->castling[wKSC] = move->castling[wKSC];
  board->castling[wQSC] = move->castling[wQSC];
  board->castling[bKSC] = move->castling[bKSC];
  board->castling[bQSC] = move->castling[bQSC];
  
  board->pieces_colour[WHITE] = board->pieces[wP] | board->pieces[wN] | board->pieces[wB] |
                                board->pieces[wR] | board->pieces[wQ] | board->pieces[wK];
  
  board->pieces_colour[BLACK] = board->pieces[bP] | board->pieces[bN] | board->pieces[bB] |
                                board->pieces[bR] | board->pieces[bQ] | board->pieces[bK];
  
  board->pieces_all = board->pieces_colour[WHITE] | board->pieces_colour[BLACK];
}