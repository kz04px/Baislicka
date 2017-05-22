#include "defs.h"
#include <string.h>

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

enum {GEN_HASHMOVE, GEN_CAPTURES, GEN_QUIETS};

int is_same_move(s_move move_1, s_move move_2)
{
  return (move_1 == move_2);
}

int is_no_move(s_move move)
{
  return is_same_move(move, NO_MOVE);
}

int is_promo_move(s_move move)
{
  return move_get_type(move)&8;
}

int is_capture_move(s_move move)
{
  return move_get_type(move)&4;
}

// To
int move_get_to(s_move move)
{
  return move & 0x3F;
}
void move_set_to(s_move *move, uint8_t to)
{
  *move &= ~0x3F;
  *move |= to & 0x3F;
}

// From
int move_get_from(s_move move)
{
  return (move>>6) & 0x3F;
}
void move_set_from(s_move *move, uint8_t from)
{
  *move &= ~(0x3F<<6);
  *move |= (from & 0x3F) << 6;
}

// Type
int move_get_type(s_move move)
{
  return (move>>12) & 0xF;
}
void move_set_type(s_move *move, uint8_t type)
{
  *move &= ~(0xF<<12);
  *move |= (type & 0xF) << 12;
}

// Captured
int move_get_captured(s_move move)
{
  return (move>>16) & 0x7;
}
void move_set_captured(s_move *move, uint8_t piece)
{
  *move &= ~(0x7<<16);
  *move |= (piece & 0x7) << 16;
}

// Piece
int move_get_piece(s_move move)
{
  return (move>>20) & 0x7;
}
void move_set_piece(s_move *move, uint8_t piece)
{
  *move &= ~(0x7<<20);
  *move |= (piece & 0x7) << 20;
}

int next_move(s_board *board, s_move_generator *generator, s_move *move)
{
  assert(board);
  assert(generator);
  assert(move);

  while(generator->move_num >= generator->num_moves)
  {
    generator->move_num = 0;
    generator->num_moves = 0;

    if(generator->stage == GEN_HASHMOVE)
    {
      generator->stage++;
      if(!is_legal_move(board, &generator->hash_move)) {continue;}
      generator->moves[0] = generator->hash_move;
      generator->num_moves = 1;
      generator->scores[0] = 0;
    }
    else if(generator->stage == GEN_CAPTURES)
    {
      generator->stage++;
      generator->num_moves = find_moves_captures(board, &generator->moves[0], board->turn);
      #ifdef SORT_MOVES
        int i;
        for(i = 0; i < generator->num_moves; ++i)
        {
          #if defined(CAPTURE_SORT_SEE)
            generator->scores[i] = 50000 + see_capture(board, generator->moves[i]);
          #elif defined(CAPTURE_SORT_MVVLVA)
            generator->scores[i] = 0;
          #else
            generator->scores[i] = 0;
          #endif
          assert(generator->scores[i] >= 0);
        }

        // Insert the killer moves if they're legal
        #ifdef KILLER_MOVES
          if(is_legal_move(board, &generator->killer_move))
          {
            generator->moves[generator->num_moves] = generator->killer_move;
            generator->scores[generator->num_moves] = 50000;
            generator->num_moves++;
          }
        #endif
        #ifdef KILLER_MOVES_2
          if(is_legal_move(board, &generator->killer_move_2))
          {
            generator->moves[generator->num_moves] = generator->killer_move_2;
            generator->scores[generator->num_moves] = 50000;
            generator->num_moves++;
          }
        #endif
      #endif
    }
    else if(generator->stage == GEN_QUIETS)
    {
      generator->stage++;
      generator->num_moves = find_moves_quiet(board, &generator->moves[0], board->turn);
      int endgame = is_endgame(board);
      #ifdef SORT_MOVES
        int i;
        for(i = 0; i < generator->num_moves; ++i)
        {
          #if defined(QUIET_SORT_HISTORY_HEURISTIC)
            int to = move_get_to(generator->moves[i]);
            int from = move_get_from(generator->moves[i]);
            generator->scores[i] = hh_score[from][to] / bf_score[from][to];
          #elif defined(QUIET_SORT_PST)
            generator->scores[i] = 500 +
                                   pst_value(move_get_piece(generator->moves[i]), move_get_to(generator->moves[i]),   endgame) -
                                   pst_value(move_get_piece(generator->moves[i]), move_get_from(generator->moves[i]), endgame);
          #elif defined(QUIET_SORT_SEE)
            generator->scores[i] = 50000 + see_quiet(board, generator->moves[i]);
          #else
            generator->scores[i] = 0;
          #endif
        }
      #endif
    }
    else
    {
      return 0;
    }
  }

  int best_move = -1;
  int best_score = INT_MIN;

  int i;
  for(i = 0; i < generator->num_moves; ++i)
  {
    if(generator->scores[i] > best_score)
    {
      best_move = i;
      best_score = generator->scores[i];
    }
  }

  if(best_move == -1)
  {
    generator->move_num = 0;
    generator->num_moves = 0;
    return next_move(board, generator, move);
  }

  // Try not to repeat moves
  if(generator->stage-1 > GEN_HASHMOVE)
  {
    if(is_same_move(generator->moves[best_move], generator->hash_move))
    {
      generator->scores[best_move] = INT_MIN;
      generator->move_num++;
      return next_move(board, generator, move);
    }
  }
  if(generator->stage-1 > GEN_CAPTURES)
  {
    if(is_same_move(generator->moves[best_move], generator->killer_move))
    {
      generator->scores[best_move] = INT_MIN;
      generator->move_num++;
      return next_move(board, generator, move);
    }
  }

  *move = generator->moves[best_move];
  generator->scores[best_move] = INT_MIN;
  generator->move_num++;
  return 1;
}

// SEE (Static Exchange Evaluation)
int moves_sort_see(s_board *board, s_move *moves, int num_moves)
{
  assert(moves != NULL);
  assert(num_moves >= 0);
  assert(num_moves < MAX_MOVES);

  if(num_moves < 2) {return 0;}

  int scores[MAX_MOVES] = {0};

  int a;
  for(a = 0; a < num_moves; ++a)
  {
    scores[a] = see_capture(board, moves[a]);
  }

  for(a = 0; a < num_moves-1; ++a)
  {
    int b;
    for(b = a+1; b < num_moves; ++b)
    {
      if(scores[a] < scores[b])
      {
        s_move store = moves[a];
        moves[a] = moves[b];
        moves[b] = store;

        int store2 = scores[a];
        scores[a] = scores[b];
        scores[b] = store2;
      }
    }
  }

  return 0;
}

int null_make(s_board *board)
{
  assert(board);

  board->ep = 0;
  #ifdef HASHTABLE
    board->key ^= key_turn;
  #endif
  board->num_halfmoves = 0;
  board->key_history[board->history_size] = board->key;
  board->history_size++;
  board->turn = 1-(board->turn);
  return 0;
}

int null_undo(s_board *board)
{
  assert(board);

  board->history_size--;
  board->turn = 1-(board->turn);

  return 0;
}

int move_add_pawn(s_board *board, s_move *moves, int from, int to)
{
  assert(board != NULL);
  assert(moves != NULL);
  assert(from >= 0);
  assert(from <= 63);
  assert(to >= 0);
  assert(to <= 63);

  if(((uint64_t)1<<to)&(U64_RANK_1|U64_RANK_8))
  {
    moves[0] = add_promotion_move(board, from, to, QUEEN_PROMO);
    moves[1] = add_promotion_move(board, from, to, KNIGHT_PROMO);
    moves[2] = add_promotion_move(board, from, to, ROOK_PROMO);
    moves[3] = add_promotion_move(board, from, to, BISHOP_PROMO);
    return 4;
  }
  else
  {
    moves[0] = move_add(board, from, to, QUIET, PAWNS);
    return 1;
  }
}

s_move add_promotion_move(s_board *board, int from, int to, int type)
{
  assert(board != NULL);
  assert(from >= 0);
  assert(from <= 63);
  assert(to >= 0);
  assert(to <= 63);

  s_move move = NO_MOVE;

  move_set_captured(&move, EMPTY);
  int i;
  for(i = 1; i < 5; ++i)
  {
    if(board->pieces[i] & ((uint64_t)1<<to))
    {
      move_set_captured(&move, i);
      break;
    }
  }

  move_set_from(&move, from);
  move_set_to(&move, to);
  move_set_type(&move, type);
  move_set_piece(&move, PAWNS);
  return move;
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


  s_move move = NO_MOVE;
  move_set_from(&move, from);
  move_set_to(&move, to);
  move_set_type(&move, type);
  move_set_captured(&move, taken);
  move_set_piece(&move, piece_type);
  return move;
}

void move_make(s_board *board, s_move *move)
{
  assert(board != NULL);
  assert(move != NULL);
  assert(move_get_from(*move) <= 63);
  assert(move_get_to(*move) <= 63);
  assert(move_get_piece(*move) <= KINGS);

  uint64_t from = (uint64_t)1<<(move_get_from(*move));
  uint64_t to   = (uint64_t)1<<(move_get_to(*move));

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
  board->castling &= castling_perms[move_get_from(*move)];
  board->castling &= castling_perms[move_get_to(*move)];

  #ifdef HASHTABLE
    board->key ^= key_castling[board->castling];
  #endif

  switch(move_get_type(*move))
  {
    case QUIET:
      board->pieces[move_get_piece(*move)] ^= to | from;
      board->colour[board->turn] ^= to | from;

      if(move_get_piece(*move) == PAWNS)
      {
        board->num_halfmoves = 0;
      }

      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_from(*move)];
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_to(*move)];
      #endif
      break;
    case CAPTURE:
      assert(move_get_captured(*move) != KINGS);
      assert(move_get_captured(*move) != EMPTY);

      board->pieces[move_get_piece(*move)] ^= to | from;
      board->colour[board->turn] ^= to | from;

      board->pieces[move_get_captured(*move)] ^= to;
      board->colour[!board->turn] ^= to;

      board->num_halfmoves = 0;

      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_from(*move)];
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_to(*move)];
        board->key ^= key_piece_positions[move_get_captured(*move)][board->turn][move_get_to(*move)];
      #endif
      break;
    case DOUBLE_PAWN:
      board->pieces[move_get_piece(*move)] ^= to | from;
      board->colour[board->turn] ^= to | from;

      board->num_halfmoves = 0;

      if(board->turn == WHITE)
      {
        board->ep = move_get_to(*move)-8;
      }
      else
      {
        board->ep = move_get_to(*move)+8;
      }

      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_from(*move)];
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_to(*move)];
        board->key ^= key_ep_file[SQ_TO_FILE(board->ep)];
      #endif
      break;
    case QUEEN_PROMO:
    case QUEEN_PROMO_CAPTURE:
      board->pieces[move_get_piece(*move)] ^= from;
      board->pieces[QUEENS] ^= to;
      board->colour[board->turn] ^= to | from;

      board->num_halfmoves = 0;

      if(move_get_captured(*move) != EMPTY)
      {
        board->pieces[move_get_captured(*move)] ^= to;
        board->colour[!board->turn] ^= to;
      }

      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_from(*move)];
        board->key ^= key_piece_positions[QUEENS][board->turn][move_get_to(*move)];
        if(move_get_captured(*move) != EMPTY)
        {
          board->key ^= key_piece_positions[move_get_captured(*move)][board->turn][move_get_to(*move)];
        }
      #endif
      break;
    case ROOK_PROMO:
    case ROOK_PROMO_CAPTURE:
      board->pieces[move_get_piece(*move)] ^= from;
      board->pieces[ROOKS] ^= to;
      board->colour[board->turn] ^= to | from;

      board->num_halfmoves = 0;

      if(move_get_captured(*move) != EMPTY)
      {
        board->pieces[move_get_captured(*move)] ^= to;
        board->colour[!board->turn] ^= to;
      }

      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_from(*move)];
        board->key ^= key_piece_positions[ROOKS][board->turn][move_get_to(*move)];
        if(move_get_captured(*move) != EMPTY)
        {
          board->key ^= key_piece_positions[move_get_captured(*move)][board->turn][move_get_to(*move)];
        }
      #endif
      break;
    case BISHOP_PROMO:
    case BISHOP_PROMO_CAPTURE:
      board->pieces[move_get_piece(*move)] ^= from;
      board->pieces[BISHOPS] ^= to;
      board->colour[board->turn] ^= to | from;

      board->num_halfmoves = 0;

      if(move_get_captured(*move) != EMPTY)
      {
        board->pieces[move_get_captured(*move)] ^= to;
        board->colour[!board->turn] ^= to;
      }

      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_from(*move)];
        board->key ^= key_piece_positions[BISHOPS][board->turn][move_get_to(*move)];
        if(move_get_captured(*move) != EMPTY)
        {
          board->key ^= key_piece_positions[move_get_captured(*move)][board->turn][move_get_to(*move)];
        }
      #endif
      break;
    case KNIGHT_PROMO:
    case KNIGHT_PROMO_CAPTURE:
      board->pieces[move_get_piece(*move)] ^= from;
      board->pieces[KNIGHTS] ^= to;
      board->colour[board->turn] ^= to | from;

      board->num_halfmoves = 0;

      if(move_get_captured(*move) != EMPTY)
      {
        board->pieces[move_get_captured(*move)] ^= to;
        board->colour[!board->turn] ^= to;
      }

      #ifdef HASHTABLE
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_from(*move)];
        board->key ^= key_piece_positions[KNIGHTS][board->turn][move_get_to(*move)];
        if(move_get_captured(*move) != EMPTY)
        {
          board->key ^= key_piece_positions[move_get_captured(*move)][board->turn][move_get_to(*move)];
        }
      #endif
      break;
    case EP:
      board->pieces[move_get_piece(*move)] ^= to | from;
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
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_from(*move)];
        board->key ^= key_piece_positions[move_get_piece(*move)][board->turn][move_get_to(*move)];
        if(board->turn == WHITE)
        {
          board->key ^= key_piece_positions[PAWNS][BLACK][move_get_to(*move)-8];
        }
        else
        {
          board->key ^= key_piece_positions[PAWNS][WHITE][move_get_to(*move)+8];
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

  // Turn
  board->turn = 1-(board->turn);
}

void move_undo(s_board *board, s_move *move)
{
  assert(board != NULL);
  assert(move != NULL);

  // Turn
  board->turn = 1-(board->turn);

  uint64_t from = (uint64_t)1<<(move_get_from(*move));
  uint64_t to   = (uint64_t)1<<(move_get_to(*move));

  switch(move_get_type(*move))
  {
    case QUIET:
      board->pieces[move_get_piece(*move)] ^= to | from;
      board->colour[board->turn] ^= to | from;
      break;
    case CAPTURE:
      board->pieces[move_get_piece(*move)] ^= to | from;
      board->colour[board->turn] ^= to | from;
      board->pieces[move_get_captured(*move)] ^= to;
      board->colour[!board->turn] ^= to;
      break;
    case DOUBLE_PAWN:
      board->pieces[move_get_piece(*move)] ^= to | from;
      board->colour[board->turn] ^= to | from;
      break;
    case QUEEN_PROMO:
    case QUEEN_PROMO_CAPTURE:
      board->pieces[move_get_piece(*move)] ^= from;
      board->pieces[QUEENS] ^= to;
      board->colour[board->turn] ^= to | from;

      if(move_get_captured(*move) != EMPTY)
      {
        board->pieces[move_get_captured(*move)] ^= to;
        board->colour[!board->turn] ^= to;
      }
      break;
    case ROOK_PROMO:
    case ROOK_PROMO_CAPTURE:
      board->pieces[move_get_piece(*move)] ^= from;
      board->pieces[ROOKS] ^= to;
      board->colour[board->turn] ^= to | from;

      if(move_get_captured(*move) != EMPTY)
      {
        board->pieces[move_get_captured(*move)] ^= to;
        board->colour[!board->turn] ^= to;
      }
      break;
    case BISHOP_PROMO:
    case BISHOP_PROMO_CAPTURE:
      board->pieces[move_get_piece(*move)] ^= from;
      board->pieces[BISHOPS] ^= to;
      board->colour[board->turn] ^= to | from;

      if(move_get_captured(*move) != EMPTY)
      {
        board->pieces[move_get_captured(*move)] ^= to;
        board->colour[!board->turn] ^= to;
      }
      break;
    case KNIGHT_PROMO:
    case KNIGHT_PROMO_CAPTURE:
      board->pieces[move_get_piece(*move)] ^= from;
      board->pieces[KNIGHTS] ^= to;
      board->colour[board->turn] ^= to | from;

      if(move_get_captured(*move) != EMPTY)
      {
        board->pieces[move_get_captured(*move)] ^= to;
        board->colour[!board->turn] ^= to;
      }
      break;
    case EP:
      board->pieces[move_get_piece(*move)] ^= to | from;
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

int move_make_ascii(s_board *board, char *move_string)
{
  assert(board != NULL);
  assert(move_string != NULL);

  int from_file = move_string[0] - 'a';
  int from_rank = move_string[1] - '1';
  int to_file = move_string[2] - 'a';
  int to_rank = move_string[3] - '1';

  int to = 8*to_rank + to_file;
  int from = 8*from_rank + from_file;
  int promo = EMPTY;

  if(move_string[4] == 'Q' || move_string[4] == 'q')
  {
    promo = QUEENS;
  }
  else if(move_string[4] == 'R' || move_string[4] == 'r')
  {
    promo = ROOKS;
  }
  else if(move_string[4] == 'B' || move_string[4] == 'b')
  {
    promo = BISHOPS;
  }
  else if(move_string[4] == 'N' || move_string[4] == 'n')
  {
    promo = KNIGHTS;
  }

  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);

  s_move moves[MAX_MOVES];
  int num_moves = find_moves_all(board, &moves[0], board->turn);

  int i;
  for(i = 0; i < num_moves; ++i)
  {
    int move_to = move_get_to(moves[i]);
    int move_from = move_get_from(moves[i]);
    int move_type = move_get_type(moves[i]);

    if(move_to != to) {continue;}
    if(move_from != from) {continue;}

    if(promo == KNIGHTS && move_type != KNIGHT_PROMO && move_type != KNIGHT_PROMO_CAPTURE)
    {
      continue;
    }
    if(promo == BISHOPS && move_type != BISHOP_PROMO && move_type != BISHOP_PROMO_CAPTURE)
    {
      continue;
    }
    if(promo == ROOKS && move_type != ROOK_PROMO && move_type != ROOK_PROMO_CAPTURE)
    {
      continue;
    }
    if(promo == QUEENS && move_type != QUEEN_PROMO && move_type != QUEEN_PROMO_CAPTURE)
    {
      continue;
    }

    move_make(board, &moves[i]);

    if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
    {
      // Restore old permissions
      restore_irreversible(&permissions, board);
      move_undo(board, &moves[i]);
      continue;
    }

    return 1;
  }

  return 0;
}

int move_to_string(char *string, s_move *move)
{
  assert(string != NULL);
  assert(move != NULL);

  string[0] = SQ_TO_FILE_CHAR(move_get_from(*move));
  string[1] = SQ_TO_RANK_CHAR(move_get_from(*move));
  string[2] = SQ_TO_FILE_CHAR(move_get_to(*move));
  string[3] = SQ_TO_RANK_CHAR(move_get_to(*move));
  string[4] = '\0';

  // FIX ME
  if(is_promo_move(*move))
  {
    int type = move_get_type(*move);

    switch(type)
    {
      case QUEEN_PROMO:
      case QUEEN_PROMO_CAPTURE:
        string[4] = 'Q';
        break;
      case ROOK_PROMO:
      case ROOK_PROMO_CAPTURE:
        string[4] = 'R';
        break;
      case BISHOP_PROMO:
      case BISHOP_PROMO_CAPTURE:
        string[4] = 'B';
        break;
      case KNIGHT_PROMO:
      case KNIGHT_PROMO_CAPTURE:
        string[4] = 'N';
        break;
    }
    string[5] = '\0';
  }

  return 0;
}

int find_move(s_board *board, s_move *move)
{
  assert(board);
  assert(move);

  uint64_t moves = 0;
  uint64_t to_bb = (uint64_t)1<<(move_get_to(*move));
  uint64_t from_bb = (uint64_t)1<<(move_get_from(*move));
  uint64_t occupancy = board->colour[WHITE]|board->colour[BLACK];

  // Can't capture our own pieces
  if(to_bb & board->colour[board->turn]) {return 0;}
  // The piece to move has to be there
  if(!(from_bb & board->colour[board->turn])) {return 0;}
  if(!(from_bb & board->pieces[move_get_piece(*move)])) {return 0;}
  // The captured piece (if any) has to be there - except during EP
  if(move_get_captured(*move) != EMPTY && move_get_type(*move) != EP)
  {
    if(!(board->pieces[move_get_captured(*move)] & to_bb)) {return 0;}
  }
  else
  {
    if(to_bb & (board->colour[WHITE]|board->colour[BLACK])) {return 0;}
  }

  switch(move_get_piece(*move))
  {
    case PAWNS:
      if(move_get_type(*move) == DOUBLE_PAWN)
      {
        if(board->turn == WHITE)
        {
          if((to_bb>>8) & (board->colour[WHITE]|board->colour[BLACK])) {return 0;}
        }
        else
        {
          if((to_bb<<8) & (board->colour[WHITE]|board->colour[BLACK])) {return 0;}
        }
      }
      else if(move_get_type(*move) == EP)
      {
        if(board->ep != move_get_to(*move)) {return 0;}
      }
      return 1;
      break;
    case KNIGHTS:
      moves = magic_moves_knight(move_get_from(*move)) & to_bb;
      break;
    case BISHOPS:
      moves = magic_moves_bishop(occupancy, move_get_from(*move)) & to_bb;
      break;
    case ROOKS:
      moves = magic_moves_rook(occupancy, move_get_from(*move)) & to_bb;
      break;
    case QUEENS:
      moves = (magic_moves_bishop(occupancy, move_get_from(*move)) | magic_moves_rook(occupancy, move_get_from(*move))) & to_bb;
      break;
    case KINGS:
      if(move_get_type(*move) == KSC)
      {
        return can_castle(board, board->turn, KSC);
      }
      else if(move_get_type(*move) == QSC)
      {
        return can_castle(board, board->turn, QSC);
      }
      else
      {
        moves = magic_moves_king(move_get_from(*move)) & to_bb;
      }
      break;
    default:
      assert(0);
      break;
  }

  return (moves != 0);
}

int is_legal_move(s_board *board, s_move *move)
{
  assert(board != NULL);
  assert(move != NULL);

  /*
  s_move moves[MAX_MOVES];
  int num_moves = find_moves_captures(board, &moves[0], board->turn);
  num_moves += find_moves_quiet(board, &moves[num_moves], board->turn);

  int i;
  for(i = 0; i < num_moves; ++i)
  {
    if(is_same_move(moves[i], *move))
    {
      return 1;
    }
  }

  return 0;
  */

  /*
  */
  return find_move(board, move);

  /*
  uint64_t allowed = (uint64_t)1<<(move_get_to(*move));
  s_move move_list[MAX_MOVES];

  int num_moves = 0;
  switch(move_get_piece(move))
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
    if(is_same_move(move_list[i], *move))
    {
      return 1;
    }
  }
  */

  return 0;
}
