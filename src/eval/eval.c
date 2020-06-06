#include <assert.h>
#include <stdlib.h>
#include "defs.h"
#include "eval.h"
#include "mobility.h"
#include "phase.h"
#include "pst.h"
#include "../chess/bitboards.h"
#include "../chess/attack.h"

const int piece_values[6] = {100, 320, 330, 500, 900, 20000};
const int bishop_pair_value = 25;
const int knight_pair_value = 15;

const int doubled_pawn_value   = -10;
const int isolated_pawn_value  = -20;
const int backward_pawn_value  = -10;
const int pawn_chain_value     =  10;
const int passed_pawn_value[8] = {0, 10, 10, 15, 25, 40, 60, 0};

const int open_file_value = 25;
const int knight_outpost_value = 20;

const int knight_open_value[9] = {0, 0, 0, 2, 4, 6, 8, 10, 12};
const int bishop_open_value[9] = {12, 10, 8, 6, 4, 2, 0, 0, 0};
const int rook_open_value[9]   = {12, 10, 8, 6, 4, 2, 0, 0, 0};

int piece_value(int piece)
{
    assert(PAWNS <= piece);
    assert(piece <= KINGS);
    return piece_values[piece];
}

int dist(const int sq_1, const int sq_2)
{
    int file_dif = abs(SQ_TO_FILE(sq_1) - SQ_TO_FILE(sq_2));
    int rank_dif = abs(SQ_TO_RANK(sq_1) - SQ_TO_RANK(sq_2));
    return (file_dif >= rank_dif) ? file_dif : rank_dif;
}

int insufficient_material(const s_board *board)
{
    assert(board != NULL);

    // KP
    if(board->pieces[PAWNS])  {return 0;}
    // KQ
    if(board->pieces[QUEENS]) {return 0;}
    // KR
    if(board->pieces[ROOKS])  {return 0;}
    // KNB
    if(board->colour[WHITE]&board->pieces[KNIGHTS] && board->colour[WHITE]&board->pieces[BISHOPS]) {return 0;}
    if(board->colour[BLACK]&board->pieces[KNIGHTS] && board->colour[BLACK]&board->pieces[BISHOPS]) {return 0;}

    return 1;
}

int evaluate(const s_board *board)
{
    /*
    int endgame = is_endgame(board);
    if(endgame && insufficient_material(board))
    {
      return 0;
    }
    */

    int score = 0;

    // Piece counts
    const int num_wP = __builtin_popcountll(board->colour[WHITE] & board->pieces[PAWNS]);
    const int num_wN = __builtin_popcountll(board->colour[WHITE] & board->pieces[KNIGHTS]);
    const int num_wB = __builtin_popcountll(board->colour[WHITE] & board->pieces[BISHOPS]);
    const int num_wR = __builtin_popcountll(board->colour[WHITE] & board->pieces[ROOKS]);
    const int num_wQ = __builtin_popcountll(board->colour[WHITE] & board->pieces[QUEENS]);

    const int num_bP = __builtin_popcountll(board->colour[BLACK] & board->pieces[PAWNS]);
    const int num_bN = __builtin_popcountll(board->colour[BLACK] & board->pieces[KNIGHTS]);
    const int num_bB = __builtin_popcountll(board->colour[BLACK] & board->pieces[BISHOPS]);
    const int num_bR = __builtin_popcountll(board->colour[BLACK] & board->pieces[ROOKS]);
    const int num_bQ = __builtin_popcountll(board->colour[BLACK] & board->pieces[QUEENS]);

    // Piece values
    int white_pieces = num_wP*piece_values[PAWNS]   +
                       num_wN*piece_values[KNIGHTS] +
                       num_wB*piece_values[BISHOPS] +
                       num_wR*piece_values[ROOKS]   +
                       num_wQ*piece_values[QUEENS];

    int black_pieces = num_bP*piece_values[PAWNS]   +
                       num_bN*piece_values[KNIGHTS] +
                       num_bB*piece_values[BISHOPS] +
                       num_bR*piece_values[ROOKS]   +
                       num_bQ*piece_values[QUEENS];

    score += white_pieces - black_pieces;

    // Side to move
    if(board->turn == WHITE)
    {
        score += 10;
    }
    else
    {
        score -= 10;
    }

    // Piece pairs
    if(num_wB >= 2) {score += bishop_pair_value;}
    if(num_bB >= 2) {score -= bishop_pair_value;}
    if(num_wN >= 2) {score += knight_pair_value;}
    if(num_bN >= 2) {score -= knight_pair_value;}

    for(int colour = WHITE; colour <= BLACK; ++colour)
    {
        for(int i = 0; i <= 7; ++i)
        {
            uint64_t file;

            // Check if doubled
            file = get_file(i) & board->colour[colour] & board->pieces[PAWNS];
            if(file ^ (file & ~(file-1)))
            {
                score += doubled_pawn_value * ((colour == WHITE) ? 1 : -1);
            }

            // Check if isolated
            if(!(get_adj_files(i) & board->colour[colour] & board->pieces[PAWNS]))
            {
                score += isolated_pawn_value * ((colour == WHITE) ? 1 : -1);
            }

            // Rooks & Queens on open files
            file = get_file(i) & board->pieces[PAWNS];

            if(!file && (get_file(i) & ((board->pieces[ROOKS] | board->pieces[QUEENS]) & board->colour[colour])))
            {
                score += open_file_value * ((colour == WHITE) ? 1 : -1);
            }
        }
    }

    /*
    uint64_t white_pawn_blockers = ((board->colour[WHITE] & board->pieces[PAWNS])<<8) & board->colour[WHITE];
    uint64_t black_pawn_blockers = ((board->colour[BLACK] & board->pieces[PAWNS])>>8) & board->colour[BLACK];

    score -= 10*__builtin_popcountll(white_pawn_blockers);
    score += 10*__builtin_popcountll(black_pawn_blockers);
    */

#ifdef PAWN_BLOCKING
    if(U64_D2 & (board->colour[WHITE] & board->pieces[PAWNS]) && U64_D3 & (board->colour[WHITE] & ~board->pieces[PAWNS]))
    {
        score -= 20;
    }
    if(U64_E2 & (board->colour[WHITE] & board->pieces[PAWNS]) && U64_E3 & (board->colour[WHITE] & ~board->pieces[PAWNS]))
    {
        score -= 20;
    }

    if(U64_D7 & (board->colour[BLACK] & board->pieces[PAWNS]) && U64_D6 & (board->colour[BLACK] & ~board->pieces[PAWNS]))
    {
        score += 20;
    }
    if(U64_E7 & (board->colour[BLACK] & board->pieces[PAWNS]) && U64_E6 & (board->colour[BLACK] & ~board->pieces[PAWNS]))
    {
        score += 20;
    }
#endif

    // Piece in the middle majority
    //score += 20*(__builtin_popcountll(U64_CENTER & board->colour[WHITE]) - __builtin_popcountll(U64_CENTER & board->colour[BLACK]));

    int sq;

    int opening_score = 0;
    int endgame_score = 0;

    for(int piece_type = 0; piece_type < 6; ++piece_type)
    {
        uint64_t copy;

        // White
        copy = board->pieces[piece_type] & board->colour[WHITE];
        while(copy)
        {
            sq = __builtin_ctzll(copy);

            if(piece_type == PAWNS)
            {
#ifdef PASSED_PAWN_EVAL
                if(is_passed_pawn(WHITE, sq, board->colour[BLACK] & board->pieces[PAWNS]))
                {
                    const int rank = SQ_TO_RANK(sq);
                    opening_score += passed_pawn_value[rank]>>1;
                    endgame_score += passed_pawn_value[rank];

                    /*
                    // Check if not isolated
                    const int file = SQ_TO_FILE(sq);
                    if(get_adj_files(file) & board->colour[WHITE] & board->pieces[PAWNS])
                    {
                        opening_score += 20;
                        endgame_score += 20;
                    }
                    */

                    //opening_score += piece_location_bonus[0][PAWNS][sq];
                    //endgame_score += piece_location_bonus[1][PAWNS][sq];
                }
#endif

#ifdef BACKWARD_PAWN_EVAL
                if(is_backward_pawn_white(sq, board->pieces[PAWNS]&board->colour[WHITE], board->pieces[PAWNS]&board->colour[BLACK]))
                {
                    score += backward_pawn_value;
                }
#endif

                if(board->colour[WHITE] & board->pieces[PAWNS] & magic_moves_pawns(BLACK, sq))
                {
                    score += pawn_chain_value;
                }
            }

            opening_score += pst_value(0, piece_type, sq);
            endgame_score += pst_value(1, piece_type, sq);

            copy &= copy-1;
        }

        // Black
        copy = board->pieces[piece_type] & board->colour[BLACK];
        while(copy)
        {
            sq = __builtin_ctzll(copy);

            if(piece_type == PAWNS)
            {
#ifdef PASSED_PAWN_EVAL
                if(is_passed_pawn(BLACK, sq, board->colour[WHITE] & board->pieces[PAWNS]))
                {
                    const int rank = 7 - SQ_TO_RANK(sq);
                    opening_score -= passed_pawn_value[rank]>>1;
                    endgame_score -= passed_pawn_value[rank];

                    /*
                    // Check if not isolated
                    const int file = SQ_TO_FILE(sq);
                    if(get_adj_files(file) & board->colour[BLACK] & board->pieces[PAWNS])
                    {
                    opening_score -= 20;
                    endgame_score -= 20;
                    }
                    */

                    //opening_score -= piece_location_bonus[0][PAWNS][sq^56];
                    //endgame_score -= piece_location_bonus[1][PAWNS][sq^56];
                }
#endif

#ifdef BACKWARD_PAWN_EVAL
                if(is_backward_pawn_black(sq, board->pieces[PAWNS]&board->colour[BLACK], board->pieces[PAWNS]&board->colour[WHITE]))
                {
                    score -= backward_pawn_value;
                }
#endif

                if(board->colour[BLACK] & board->pieces[PAWNS] & magic_moves_pawns(WHITE, sq))
                {
                    score -= pawn_chain_value;
                }
            }

            opening_score -= pst_value(0, piece_type, sq^56);
            endgame_score -= pst_value(1, piece_type, sq^56);

            copy &= copy-1;
        }
    }

#ifdef KNIGHT_OUTPOSTS
    // White
    uint64_t white_knights = U64_CENTER & board->colour[WHITE] & board->pieces[KNIGHTS]; // (board->colour[WHITE] & ~board->pieces[PAWNS])
    while(white_knights)
    {
        sq = __builtin_ctzll(white_knights);

        if(magic_moves_pawns(BLACK, sq) & (board->colour[WHITE] & board->pieces[PAWNS]) &&
           is_outpost(WHITE, sq, board->colour[BLACK] & board->pieces[PAWNS]))
        {
            score += knight_outpost_value;
        }

        white_knights &= white_knights-1;
    }

    // Black
    uint64_t black_knights = U64_CENTER & board->colour[BLACK] & board->pieces[KNIGHTS];
    while(black_knights)
    {
        sq = __builtin_ctzll(black_knights);

        if(magic_moves_pawns(WHITE, sq) & (board->colour[BLACK] & board->pieces[PAWNS]) &&
           is_outpost(BLACK, sq, board->colour[WHITE] & board->pieces[PAWNS]))
        {
            score -= knight_outpost_value;
        }

        black_knights &= black_knights-1;
    }
#endif

#ifdef PINNED_PIECE_EVAL
    sq = __builtin_ctzll(board->pieces[KINGS] & board->colour[WHITE]);
    uint64_t white_pinned = pinned_pieces_white(board, sq);// & (board->colour[WHITE] & ~board->pieces[PAWNS]);
    assert(__builtin_popcountll(white_pinned) <= 8);
    score -= 20*__builtin_popcountll(white_pinned);

    sq = __builtin_ctzll(board->pieces[KINGS] & board->colour[BLACK]);
    uint64_t black_pinned = pinned_pieces_black(board, sq);// & (board->colour[BLACK] & ~board->pieces[PAWNS]);
    assert(__builtin_popcountll(black_pinned) <= 8);
    score += 20*__builtin_popcountll(black_pinned);
#endif

#ifdef PIECE_OPEN_SCALING
    // White
    score += num_wN*bishop_open_value[num_wP];
    score += num_wB*bishop_open_value[num_wP];
    score += num_wR*bishop_open_value[num_wP];

    // Black
    score -= num_bN*bishop_open_value[num_bP];
    score -= num_bB*bishop_open_value[num_bP];
    score -= num_bR*bishop_open_value[num_bP];
#endif

    int phase = get_phase(num_wN + num_bN, num_wB + num_bB, num_wR + num_bR, num_wQ + num_bQ);
    score += ((opening_score * (256 - phase)) + (endgame_score * phase)) / 256;

    // White King
    score += king_safety(board, WHITE);
    score -= king_safety(board, BLACK);

    // Piece mobility
    score += piece_mobility(board, WHITE);
    score -= piece_mobility(board, BLACK);

    if(board->turn == BLACK)
    {
        score = -score;
    }

    return score;
}
