#include <assert.h>
#include "defs.h"
#include "attack.h"
#include "bitboards.h"
#include "eval.h"

int get_smallest_attacker(s_board *board, int sq, int side)
{
    assert(board != NULL);
    assert(side == WHITE || side == BLACK);

    // Pawns
    if(magic_moves_pawns(1-side, sq) & board->pieces[PAWNS] & board->colour[side]) {return PAWNS;}

    // Knights
    if((board->colour[side] & board->pieces[KNIGHTS]) & magic_moves_knight(sq)) {return KNIGHTS;}

    // Bishops
    if(magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & board->pieces[BISHOPS])) {return BISHOPS;}

    // Rooks
    if(magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & board->pieces[ROOKS])) {return ROOKS;}

    // Queens
    if(magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & board->pieces[QUEENS])) {return QUEENS;}
    if(magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & board->pieces[QUEENS])) {return QUEENS;}

    // King
    if(magic_moves_king(sq) & board->pieces[KINGS] & board->colour[side]) {return KINGS;}

    return EMPTY;
}

int square_attacked(s_board *board, uint64_t pos, int side)
{
    assert(board != NULL);
    assert(pos);
    assert(side == WHITE || side == BLACK);

    int sq = __builtin_ctzll(pos);

    // Pawns
    if(magic_moves_pawns(1-side, sq) & board->pieces[PAWNS] & board->colour[side]) {return 1;}

    // Knights
    if((board->colour[side] & board->pieces[KNIGHTS]) & magic_moves_knight(sq)) {return 1;}

    // Bishops & Queens
    if(magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & (board->pieces[BISHOPS] | board->pieces[QUEENS]))) {return 1;}

    // Rooks & Queens
    if(magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->colour[side] & (board->pieces[ROOKS] | board->pieces[QUEENS]))) {return 1;}

    // King
    if(magic_moves_king(sq) & board->pieces[KINGS] & board->colour[side]) {return 1;}

    return 0;
}

uint64_t calculate_attacking(s_board *board, uint64_t pos, int side)
{
    assert(board != NULL);
    assert(pos);
    assert(side == WHITE || side == BLACK);

    int sq = __builtin_ctzll(pos);
    uint64_t attackers = 0;

    // Pawns
    attackers |= board->colour[side] & board->pieces[PAWNS] & magic_moves_pawns(1-side, sq);

    // Knights
    attackers |= board->colour[side] & board->pieces[KNIGHTS] & magic_moves_knight(sq);

    // Bishops & Queens
    attackers |= magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE] & (board->pieces[BISHOPS] | board->pieces[QUEENS]);

    // Rooks & Queens
    attackers |= magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE] & (board->pieces[ROOKS] | board->pieces[QUEENS]);

    // King
    attackers |= magic_moves_king(sq) & board->pieces[KINGS] & board->colour[side];

    return attackers;
}

int eval_attackers(s_board *board, uint64_t pos, int side)
{
    assert(board != NULL);
    assert(pos);
    assert(side == WHITE || side == BLACK);

    int eval = 0;

    int sq = __builtin_ctzll(pos);
    uint64_t attackers = 0;

    // Pawns
    attackers = board->colour[side] & board->pieces[PAWNS] & magic_moves_pawns(1-side, sq);

    while(attackers)
    {
        eval += piece_value(PAWNS);
        attackers &= 1-attackers;
    }

    // Knights
    attackers = board->colour[side] & board->pieces[KNIGHTS] & magic_moves_knight(sq);

    while(attackers)
    {
        eval += piece_value(KNIGHTS);
        attackers &= 1-attackers;
    }

    // Bishops
    attackers = magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE] & board->pieces[BISHOPS];

    while(attackers)
    {
        eval += piece_value(BISHOPS);
        attackers &= 1-attackers;
    }

    // Rooks
    attackers = magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE] & board->pieces[ROOKS];

    while(attackers)
    {
        eval += piece_value(ROOKS);
        attackers &= 1-attackers;
    }

    // Queens
    attackers =  magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE] & board->pieces[QUEENS];
    attackers |= magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE] & board->pieces[QUEENS];

    while(attackers)
    {
        eval += piece_value(QUEENS);
        attackers &= 1-attackers;
    }

    return eval;
}

int count_attackers(s_board *board, uint64_t pos, int side)
{
    assert(board != NULL);
    assert(pos);
    assert(side == WHITE || side == BLACK);

    int sq = __builtin_ctzll(pos);
    uint64_t attackers = 0;

    // Pawns
    attackers |= board->colour[side] & board->pieces[PAWNS] & magic_moves_pawns(1-side, sq);

    // Knights
    attackers |= board->colour[side] & board->pieces[KNIGHTS] & magic_moves_knight(sq);

    // Bishops & Queens
    attackers |= board->colour[side] & magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->pieces[BISHOPS] | board->pieces[QUEENS]);

    // Rooks & Queens
    attackers |= board->colour[side] & magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & (board->pieces[ROOKS] | board->pieces[QUEENS]);

    return __builtin_popcountll(attackers);
}
