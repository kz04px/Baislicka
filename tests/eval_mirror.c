#include <assert.h>
#include "tests.h"
#include "eval/eval.h"
#include "chess/board.h"
#include "chess/move.h"
#include "chess/zobrist.h"
#include "eval/king_safety.h"

int board_flip(s_board *board)
{
    assert(board);

    // Side to move
    board->turn = 1-board->turn;

    // En passant square
    board->ep = board->ep^56;

    // Colours
    uint64_t store = board->colour[WHITE];
    board->colour[WHITE]   = __builtin_bswap64(board->colour[BLACK]);
    board->colour[BLACK]   = __builtin_bswap64(store);

    // Pieces
    board->pieces[PAWNS]   = __builtin_bswap64(board->pieces[PAWNS]);
    board->pieces[KNIGHTS] = __builtin_bswap64(board->pieces[KNIGHTS]);
    board->pieces[BISHOPS] = __builtin_bswap64(board->pieces[BISHOPS]);
    board->pieces[ROOKS]   = __builtin_bswap64(board->pieces[ROOKS]);
    board->pieces[QUEENS]  = __builtin_bswap64(board->pieces[QUEENS]);
    board->pieces[KINGS]   = __builtin_bswap64(board->pieces[KINGS]);

    // Castling perms
    uint8_t castling_old = board->castling;
    board->castling = 0;
    if(castling_old&wKSC) {board->castling ^= bKSC;}
    if(castling_old&wQSC) {board->castling ^= bQSC;}
    if(castling_old&bKSC) {board->castling ^= wKSC;}
    if(castling_old&bQSC) {board->castling ^= wQSC;}

#ifdef HASHTABLE
    board->key = create_key_board(board);
#endif

    return 0;
}

int test_eval_mirroring()
{
    const char positions[16][256] = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"},
        {"k7/8/8/4p3/4P3/8/2P5/K7 w - -"},
        {"k7/1p3p1p/8/8/4P3/5P2/2P5/K7 w - -"},
        {"5B2/6P1/1p6/8/1N6/kP6/2K5/8 w - - 0 1"}
    };
    s_board board;

    for(int i = 0; i < 16; ++i)
    {
        int r = set_fen(&board, positions[i]);
        if(r) {continue;}

        int eval_normal = evaluate(&board);

        board_flip(&board);

        int eval_mirror = evaluate(&board);

        if(eval_normal != eval_mirror) {return -2;}
    }

    return 0;
}
