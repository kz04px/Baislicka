#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include "defs.h"
#include "movegen.h"
#include "attack.h"
#include "bitboards.h"
#include "move.h"

int find_moves_pawn_ep(const s_board *board, s_move *move_list)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    // ep
    if(board->ep)
    {
        uint64_t moves = magic_moves_pawns(!board->turn, board->ep) & board->pieces[PAWNS] & board->colour[board->turn];

        while(moves)
        {
            int from = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, board->ep, EP, PAWNS);
            num_moves++;
            moves &= moves-1;
        }
    }

    return num_moves;
}

int find_moves_pawn_captures(const s_board *board, s_move *move_list, uint64_t allowed)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    uint64_t copy = board->pieces[PAWNS] & board->colour[board->turn];
    while(copy)
    {
        int from = __builtin_ctzll(copy);
        uint64_t moves = magic_moves_pawns(board->turn, from) & allowed;

        while(moves)
        {
            int to = __builtin_ctzll(moves);
            num_moves += move_add_pawn(board, &move_list[num_moves], from, to);
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    return num_moves;
}

int find_moves_wP_quiet(const s_board *board, s_move *move_list)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    // Up 1
    uint64_t moves = ((board->pieces[PAWNS] & board->colour[WHITE])<<8) & (~(board->colour[WHITE]|board->colour[BLACK]));
    while(moves)
    {
        int to = __builtin_ctzll(moves);
        num_moves += move_add_pawn(board, &move_list[num_moves], to-8, to);

        // Up 2
        if(to <= 23 && 16 <= to && (((uint64_t)1<<(to+8))&(~(board->colour[WHITE]|board->colour[BLACK]))))
        {
            move_list[num_moves] = move_add(board, to-8, to+8, DOUBLE_PAWN, PAWNS);
            num_moves++;
        }

        moves &= moves-1;
    }

    return num_moves;
}

int find_moves_bP_quiet(const s_board *board, s_move *move_list)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    // Down 1
    uint64_t moves = ((board->pieces[PAWNS] & board->colour[BLACK])>>8) & (~(board->colour[WHITE]|board->colour[BLACK]));
    while(moves)
    {
        int to = __builtin_ctzll(moves);
        num_moves += move_add_pawn(board, &move_list[num_moves], to+8, to);

        // Down 2
        if(40 <= to && to <= 47 && (((uint64_t)1<<(to-8))&(~(board->colour[WHITE]|board->colour[BLACK]))))
        {
            move_list[num_moves] = move_add(board, to+8, to-8, DOUBLE_PAWN, PAWNS);
            num_moves++;
        }

        moves &= moves-1;
    }

    return num_moves;
}

int can_castle(const s_board *board, int turn, int side)
{
    if(turn == WHITE)
    {
        if(side == KSC)
        {
            if((board->castling & wKSC) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_F1) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_G1) &&
               square_attacked(board, U64_E1, BLACK) == 0 &&
               square_attacked(board, U64_F1, BLACK) == 0 &&
               square_attacked(board, U64_G1, BLACK) == 0)
            {
                return 1;
            }
        }
        else
        {
            if((board->castling & wQSC) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_B1) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_C1) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_D1) &&
               square_attacked(board, U64_E1, BLACK) == 0 &&
               square_attacked(board, U64_D1, BLACK) == 0 &&
               square_attacked(board, U64_C1, BLACK) == 0)
            {
                return 1;
            }
        }
    }
    else
    {
        if(side == KSC)
        {
            if((board->castling & bKSC) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_F8) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_G8) &&
               square_attacked(board, U64_E8, WHITE) == 0 &&
               square_attacked(board, U64_F8, WHITE) == 0 &&
               square_attacked(board, U64_G8, WHITE) == 0)
            {
                return 1;
            }
        }
        else
        {
            if((board->castling & bQSC) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_B8) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_C8) &&
               !((board->colour[WHITE]|board->colour[BLACK])&U64_D8) &&
               square_attacked(board, U64_E8, WHITE) == 0 &&
               square_attacked(board, U64_D8, WHITE) == 0 &&
               square_attacked(board, U64_C8, WHITE) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int find_moves_kings_castles(const s_board *board, s_move *move_list)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    // castling
    if(board->turn == WHITE)
    {
        if(can_castle(board, WHITE, KSC))
        {
            move_list[num_moves] = move_add(board, E1, G1, KSC, KINGS);
            num_moves++;
        }
        if(can_castle(board, WHITE, QSC))
        {
            move_list[num_moves] = move_add(board, E1, C1, QSC, KINGS);
            num_moves++;
        }
    }
    else
    {
        if(can_castle(board, BLACK, KSC))
        {
            move_list[num_moves] = move_add(board, E8, G8, KSC, KINGS);
            num_moves++;
        }
        if(can_castle(board, BLACK, QSC))
        {
            move_list[num_moves] = move_add(board, E8, C8, QSC, KINGS);
            num_moves++;
        }
    }

    return num_moves;
}

int find_moves_kings(const s_board *board, s_move *move_list, uint64_t allowed)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    uint64_t from = __builtin_ctzll(board->colour[board->turn] & board->pieces[KINGS]);
    uint64_t moves = magic_moves_king(from) & allowed;
    while(moves)
    {
        int to = __builtin_ctzll(moves);
        move_list[num_moves] = move_add(board, from, to, QUIET, KINGS);
        num_moves++;
        moves &= moves-1;
    }

    return num_moves;
}

int find_moves_knights(const s_board *board, s_move *move_list, uint64_t allowed)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    uint64_t copy = board->pieces[KNIGHTS] & board->colour[board->turn];
    while(copy)
    {
        int from = __builtin_ctzll(copy);
        uint64_t moves = magic_moves_knight(from) & allowed;

        while(moves)
        {
            int to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, KNIGHTS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    return num_moves;
}

int find_moves_bishops(const s_board *board, s_move *move_list, uint64_t allowed)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    // Bishops
    uint64_t copy = board->pieces[BISHOPS] & board->colour[board->turn];
    while(copy)
    {
        int from = __builtin_ctzll(copy);
        uint64_t moves = magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        while(moves)
        {
            int to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, BISHOPS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    return num_moves;
}

int find_moves_rooks(const s_board *board, s_move *move_list, uint64_t allowed)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;

    // Rook
    uint64_t copy = board->pieces[ROOKS] & board->colour[board->turn];
    while(copy)
    {
        int from = __builtin_ctzll(copy);
        uint64_t moves = magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        while(moves)
        {
            int to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, ROOKS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    return num_moves;
}

int find_moves_queens(const s_board *board, s_move *move_list, uint64_t allowed)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;
    uint64_t moves;
    int from;
    int to;
    uint64_t copy;

    // Queens (Bishop)
    copy = board->pieces[QUEENS] & board->colour[board->turn];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves = magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        while(moves)
        {
            to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, QUEENS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    // Queens (Rook)
    copy = board->pieces[QUEENS] & board->colour[board->turn];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves = magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        while(moves)
        {
            to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, QUEENS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    return num_moves;
}

int find_moves_bishops_queens(const s_board *board, s_move *move_list, uint64_t allowed)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;
    uint64_t moves;
    int from;
    int to;
    uint64_t copy;

    // Bishops
    copy = board->pieces[BISHOPS] & board->colour[board->turn];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves = magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        while(moves)
        {
            to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, BISHOPS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    // Queens (Bishop)
    copy = board->pieces[QUEENS] & board->colour[board->turn];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves = magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        while(moves)
        {
            to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, QUEENS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    return num_moves;
}

int find_moves_rooks_queens(const s_board *board, s_move *move_list, uint64_t allowed)
{
    assert(board != NULL);
    assert(move_list != NULL);

    int num_moves = 0;
    uint64_t moves;
    int from;
    int to;
    uint64_t copy;

    // Rook
    copy = board->pieces[ROOKS] & board->colour[board->turn];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves = magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        while(moves)
        {
            to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, ROOKS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    // Queens (Rook)
    copy = board->pieces[QUEENS] & board->colour[board->turn];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves = magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        while(moves)
        {
            to = __builtin_ctzll(moves);
            move_list[num_moves] = move_add(board, from, to, QUIET, QUEENS);
            num_moves++;
            moves &= moves-1;
        }
        copy &= copy-1;
    }

    return num_moves;
}

int find_moves_all(const s_board *board, s_move *move_list, int colour)
{
    assert(board != NULL);
    assert(move_list != NULL);
    assert(colour == WHITE || colour == BLACK);

    uint64_t allowed = ~board->colour[board->turn];
    int num_moves = 0;

    if(colour == WHITE)
    {
        num_moves += find_moves_wP_quiet(board, &move_list[num_moves]);
    }
    else
    {
        num_moves += find_moves_bP_quiet(board, &move_list[num_moves]);
    }

    num_moves += find_moves_bishops_queens(board, &move_list[num_moves], allowed);
    num_moves += find_moves_rooks_queens(board, &move_list[num_moves], allowed);
    num_moves += find_moves_knights(board, &move_list[num_moves], allowed);
    num_moves += find_moves_pawn_captures(board, &move_list[num_moves], board->colour[1-board->turn]);
    num_moves += find_moves_pawn_ep(board, &move_list[num_moves]);
    num_moves += find_moves_kings(board, &move_list[num_moves], allowed);
    num_moves += find_moves_kings_castles(board, &move_list[num_moves]);

    assert(num_moves >= 0);
    assert(num_moves < 256);

#ifndef NDEBUG
    for(int i = 0; i < num_moves; ++i)
    {
        assert(is_legal_move(board, &move_list[i]));

        int type = move_get_type(move_list[i]);
        switch(type)
        {
            case QUIET:
            case DOUBLE_PAWN:
            case KSC:
            case QSC:
                assert(!is_capture_move(move_list[i]));
                assert(!is_promo_move(move_list[i]));
                break;
            case KNIGHT_PROMO:
            case BISHOP_PROMO:
            case ROOK_PROMO:
            case QUEEN_PROMO:
                assert(!is_capture_move(move_list[i]));
                assert(is_promo_move(move_list[i]));
                break;
            case CAPTURE:
            case EP:
                assert(is_capture_move(move_list[i]));
                assert(!is_promo_move(move_list[i]));
                break;
            case KNIGHT_PROMO_CAPTURE:
            case BISHOP_PROMO_CAPTURE:
            case ROOK_PROMO_CAPTURE:
            case QUEEN_PROMO_CAPTURE:
                assert(is_capture_move(move_list[i]));
                assert(is_promo_move(move_list[i]));
                break;
        }
    }
#endif

    return num_moves;
}

int find_moves_captures(const s_board *board, s_move *move_list, int colour)
{
    assert(board != NULL);
    assert(move_list != NULL);
    assert(colour == WHITE || colour == BLACK);

    uint64_t allowed = board->colour[1-colour];

    int num_moves = 0;
    num_moves += find_moves_bishops_queens(board, &move_list[num_moves], allowed);
    num_moves += find_moves_rooks_queens(board, &move_list[num_moves], allowed);
    num_moves += find_moves_knights(board, &move_list[num_moves], allowed);
    num_moves += find_moves_pawn_captures(board, &move_list[num_moves], allowed);
    num_moves += find_moves_pawn_ep(board, &move_list[num_moves]);
    num_moves += find_moves_kings(board, &move_list[num_moves], allowed);

#ifndef NDEBUG
    for(int i = 0; i < num_moves; ++i)
    {
        assert(is_legal_move(board, &move_list[i]));
    }
#endif

    return num_moves;
}

int find_moves_quiet(const s_board *board, s_move *move_list, int colour)
{
    assert(board != NULL);
    assert(move_list != NULL);
    assert(colour == WHITE || colour == BLACK);

    uint64_t allowed = ~(board->colour[colour]|board->colour[1-colour]);

    int num_moves = 0;
    num_moves += find_moves_bishops_queens(board, &move_list[num_moves], allowed);
    num_moves += find_moves_rooks_queens(board, &move_list[num_moves], allowed);
    num_moves += find_moves_knights(board, &move_list[num_moves], allowed);

    if(colour == WHITE)
    {
        num_moves += find_moves_wP_quiet(board, &move_list[num_moves]);
    }
    else
    {
        num_moves += find_moves_bP_quiet(board, &move_list[num_moves]);
    }

    num_moves += find_moves_kings(board, &move_list[num_moves], allowed);
    num_moves += find_moves_kings_castles(board, &move_list[num_moves]);

#ifndef NDEBUG
    for(int i = 0; i < num_moves; ++i)
    {
        assert(is_legal_move(board, &move_list[i]));
    }
#endif

    return num_moves;
}

int next_move(s_board *board, s_move_generator *generator, s_move *move, const int *values)
{
    assert(board);
    assert(generator);
    assert(move);
    assert(values);

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

            for(int i = 0; i < generator->num_moves; ++i)
            {
                generator->scores[i] = 50000 + see_capture(board, generator->moves[i], values);
                assert(generator->scores[i] >= 0);
            }

            // Insert the killer moves if they're legal
            if(is_legal_move(board, &generator->killer_move))
            {
                generator->moves[generator->num_moves] = generator->killer_move;
                generator->scores[generator->num_moves] = 50000;
                generator->num_moves++;
            }
#ifdef KILLER_MOVES_2
            if(is_legal_move(board, &generator->killer_move_2))
            {
                generator->moves[generator->num_moves] = generator->killer_move_2;
                generator->scores[generator->num_moves] = 50000;
                generator->num_moves++;
            }
#endif
        }
        else if(generator->stage == GEN_QUIETS)
        {
            generator->stage++;
            generator->num_moves = find_moves_quiet(board, &generator->moves[0], board->turn);

            for(int i = 0; i < generator->num_moves; ++i)
            {
                generator->scores[i] = 50000 + see_quiet(board, generator->moves[i], values);
            }
        }
        else
        {
            return 0;
        }
    }

    int best_move = -1;
    int best_score = INT_MIN;

    for(int i = 0; i < generator->num_moves; ++i)
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
        return next_move(board, generator, move, values);
    }

    // Try not to repeat moves
    if(generator->stage-1 > GEN_HASHMOVE)
    {
        if(is_same_move(generator->moves[best_move], generator->hash_move))
        {
            generator->scores[best_move] = INT_MIN;
            generator->move_num++;
            return next_move(board, generator, move, values);
        }
    }
    if(generator->stage-1 > GEN_CAPTURES)
    {
        if(is_same_move(generator->moves[best_move], generator->killer_move))
        {
            generator->scores[best_move] = INT_MIN;
            generator->move_num++;
            return next_move(board, generator, move, values);
        }
    }

    *move = generator->moves[best_move];
    generator->scores[best_move] = INT_MIN;
    generator->move_num++;
    return 1;
}
