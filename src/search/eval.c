#include <assert.h>
#include "defs.h"
#include "../chess/bitboards.h"
#include "../chess/attack.h"
#include "eval.h"

const int piece_location_bonus[2][6][64] = {{
{
// Pawns - early/mid
//  A   B   C   D   E   F   G   H
    0,  0,  0,  0,  0,  0,  0,  0, // 1
   -1, -7,-11,-35,-13,  5,  3, -5, // 2
    1,  1, -6,-19, -6, -7, -4, 10, // 3
    1, 14,  8,  4,  5,  4, 10,  7, // 4
    9, 30, 23, 31, 31, 23, 17, 11, // 5
   21, 54, 72, 56, 77, 95, 71, 11, // 6
  118,121,173,168,107, 82,-16, 22, // 7
    0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// Knights - early/mid
//  A   B   C   D   E   F   G   H
  -99,-30,-66,-64,-29,-19,-61,-81, // 1
  -56,-31,-28, -1, -7,-20,-42,-11, // 2
  -38,-16,  0, 14,  8,  3,  3,-42, // 3
  -14,  0,  2,  3, 19, 12, 33, -7, // 4
  -14, -4, 25, 33, 10, 33, 14, 43, // 5
  -22, 18, 60, 64,124,143, 55,  6, // 6
  -34, 24, 54, 74, 60,122,  2, 29, // 7
  -60,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// Bishops - early/mid
//  A   B   C   D   E   F   G   H
   -7, 12, -8,-37,-31, -8,-45,-67, // 1
   15,  5, 13,-10,  1,  2,  0, 15, // 2
    5, 12, 14, 13, 10, -1,  3,  4, // 3
    1,  5, 23, 32, 21,  8, 17,  4, // 4
   -1, 16, 29, 27, 37, 27, 17,  4, // 5
    7, 27, 20, 56, 91,108, 53, 44, // 6
  -24,-23, 30, 58, 65, 61, 69, 11, // 7
    0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// Rooks - early/mid
//  A   B   C   D   E   F   G   H
   -2, -1,  3,  1,  2,  1,  4, -8, // 1
  -26, -6,  2, -2,  2,-10, -1,-29, // 2
  -16,  0,  3, -3,  8, -1, 12,  3, // 3
   -9, -5,  8, 14, 18,-17, 13,-13, // 4
   19, 33, 46, 57, 53, 39, 53, 16, // 5
   24, 83, 54, 75,134,144, 85, 75, // 6
   46, 33, 64, 62, 91, 89, 70,104, // 7
   84,  0,  0, 37,124,  0,  0,153  // 8
},{
// Queens - early/mid
//  A   B   C   D   E   F   G   H
    1,-10,-11,  3,-15,-51,-83,-13, // 1
   -7,  3,  2,  5, -1,-10, -7, -2, // 2
  -11,  0, 12,  2,  8, 11,  7, -6, // 3
   -9,  5,  7,  9, 18, 17, 26,  4, // 4
   -6,  0, 15, 25, 32,  9, 26, 12, // 5
  -16, 10, 13, 25, 37, 30, 15, 26, // 6
    1, 11, 35,  0, 16, 55, 39, 57, // 7
  -13,  6,-42,  0, 29,  0,  0,102  // 8
},{
// Kings - early/mid
//  A   B   C   D   E   F   G   H
    0,  0,  0, -9,  0, -9, 25,  0, // 1
   -9, -9, -9, -9, -9, -9, -9, -9, // 2
   -9, -9, -9, -9, -9, -9, -9, -9, // 3
   -9, -9, -9, -9, -9, -9, -9, -9, // 4
   -9, -9, -9, -9, -9, -9, -9, -9, // 5
   -9, -9, -9, -9, -9, -9, -9, -9, // 6
   -9, -9, -9, -9, -9, -9, -9, -9, // 7
   -9, -9, -9, -9, -9, -9, -9, -9  // 8
}},{{
// Pawns - endgame
//  A   B   C   D   E   F   G   H
    0,  0,  0,  0,  0,  0,  0,  0, // 1
  -17,-17,-17,-17,-17,-17,-17,-17, // 2
  -11,-11,-11,-11,-11,-11,-11,-11, // 3
   -7, -7, -7, -7, -7, -7, -7, -7, // 4
   16, 16, 16, 16, 16, 16, 16, 16, // 5
   55, 55, 55, 55, 55, 55, 55, 55, // 6
   82, 82, 82, 82, 82, 82, 82, 82, // 7
    0,  0,  0,  0,  0,  0,  0,  0  // 8
},{
// Knights - endgame
//  A   B   C   D   E   F   G   H
  -99,-99,-94,-88,-88,-94,-99,-99, // 1
  -81,-62,-49,-43,-43,-49,-62,-81, // 2
  -46,-27,-15, -9, -9,-15,-27,-46, // 3
  -22, -3, 10, 16, 16, 10, -3,-22, // 4
   -7, 12, 25, 31, 31, 25, 12, -7, // 5
   -2, 17, 30, 36, 36, 30, 17, -2, // 6
   -7, 12, 25, 31, 31, 25, 12, -7, // 7
  -21, -3, 10, 16, 16, 10, -3,-21  // 8
},{
// Bishops - endgame
//  A   B   C   D   E   F   G   H
  -27,-21,-17,-15,-15,-17,-21,-27, // 1
  -10, -4,  0,  2,  2,  0, -4,-10, // 2
    2,  8, 12, 14, 14, 12,  8,  2, // 3
   11, 17, 21, 23, 23, 21, 17, 11, // 4
   14, 20, 24, 26, 26, 24, 20, 14, // 5
   13, 19, 23, 25, 25, 23, 19, 13, // 6
    8, 14, 18, 20, 20, 18, 14,  8, // 7
   -2,  4,  8, 10, 10,  8,  4, -2  // 8
},{
// Rooks - endgame
//  A   B   C   D   E   F   G   H
  -32,-31,-30,-29,-29,-30,-31,-32, // 1
  -27,-25,-24,-24,-24,-24,-25,-27, // 2
  -15,-13,-12,-12,-12,-12,-13,-15, // 3
    1,  2,  3,  4,  4,  3,  2,  1, // 4
   15, 17, 18, 18, 18, 18, 17, 15, // 5
   25, 27, 28, 28, 28, 28, 27, 25, // 6
   27, 28, 29, 30, 30, 29, 28, 27, // 7
   16, 17, 18, 19, 19, 18, 17, 16  // 8
},{
// Queens - endgame
//  A   B   C   D   E   F   G   H
  -61,-55,-52,-50,-50,-52,-55,-61, // 1
  -31,-26,-22,-21,-21,-22,-26,-31, // 2
   -8, -3,  1,  3,  3,  1, -3, -8, // 3
    9, 14, 17, 19, 19, 17, 14,  9, // 4
   19, 24, 28, 30, 30, 28, 24, 19, // 5
   23, 28, 32, 34, 34, 32, 28, 23, // 6
   21, 26, 30, 31, 31, 30, 26, 21, // 7
   12, 17, 21, 23, 23, 21, 17, 12  // 8
},{
// Kings - endgame
//  A   B   C   D   E   F   G   H
  -34,-30,-28,-27,-27,-28,-30,-34, // 1
  -17,-13,-11,-10,-10,-11,-13,-17, // 2
   -2,  2,  4,  5,  5,  4,  2, -2, // 3
   11, 15, 17, 18, 18, 17, 15, 11, // 4
   22, 26, 28, 29, 29, 28, 26, 22, // 5
   31, 34, 37, 38, 38, 37, 34, 31, // 6
   38, 41, 44, 45, 45, 44, 41, 38, // 7
   42, 46, 48, 50, 50, 48, 46, 42  // 8
}
}};

const int king_safety_table[100] = {
   0,   0,   1,   2,   3,   5,   7,   9,  12,  15,
  18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
  68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
 140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
 260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
 377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
 494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500
};

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
    
int get_phase(const int num_knights, const int num_bishops, const int num_rooks, const int num_queens)
{
    const int knight_phase = 1;
    const int bishop_phase = 1;
    const int rook_phase   = 2;
    const int queen_phase  = 4;

    const int total_phase = knight_phase*4 + bishop_phase*4 + rook_phase*4 + queen_phase*2;

    int phase = total_phase;

    phase -= knight_phase * num_knights;
    phase -= bishop_phase * num_bishops;
    phase -= rook_phase   * num_rooks;
    phase -= queen_phase  * num_queens;

    return (phase * 256 + (total_phase / 2)) / total_phase;
}

int king_safety(const s_board *board, int side)
{
    assert(board != NULL);
    assert(side == WHITE || side == BLACK);

    int eval = 0;
    int sq = __builtin_ctzll(board->pieces[KINGS] & board->colour[side]);
    const uint64_t surrounding = magic_moves_king(sq);

    // Positive: Nearby friendly pieces
    eval += 5*__builtin_popcountll(surrounding & board->colour[side]);

    // Negative: Nearby enemy pieces
    //eval -= 5*__builtin_popcountll(surrounding & board->colour[!side]);

    /*
    // Positive: Friendly rooks and queens on the same lines
    eval += 5*__builtin_popcountll(magic_moves_rook(0ULL, sq) & (board->colour[side] & (board->pieces[ROOKS] | board->pieces[QUEENS])));

    // Negative: Enemy rooks and queens on the same lines
    eval -= 5*__builtin_popcountll(magic_moves_rook(0ULL, sq) & (board->colour[!side] & (board->pieces[ROOKS] | board->pieces[QUEENS])));
    */

    // Positive: Defending nearby squares
    /*
    int count = 0;
    uint64_t copy = surrounding;
    while(copy)
    {
        uint64_t pos = copy & ~(copy-1);

        count += count_attackers(board, pos, side);

        copy ^= pos;
    }
    eval += king_safety_table[count];
    */

    // Negative: Opponent attacking nearby squares
    /*
    int count = 0;
    uint64_t copy = surrounding;
    while(copy)
    {
        uint64_t pos = copy & ~(copy-1);

        count += count_attackers(board, pos, !side);

        copy ^= pos;
    }
    eval -= king_safety_table[count];
    */

    return eval;
}

int piece_mobility(const s_board *board, int side)
{
    assert(board);
    assert(side == WHITE || side == BLACK);

    uint64_t moves = 0;
    uint64_t copy = 0;
    uint64_t allowed = ~board->colour[side];
    int from = 0;

    // Knights
    copy = board->pieces[KNIGHTS] & board->colour[side];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves |= magic_moves_knight(from) & allowed;

        copy &= copy-1;
    }

    // Bishops & Queens
    copy = (board->pieces[BISHOPS] | board->pieces[QUEENS]) & board->colour[side];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves |= magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        copy &= copy-1;
    }

    // Rooks & Queens
    copy = (board->pieces[ROOKS] | board->pieces[QUEENS]) & board->colour[side];
    while(copy)
    {
        from = __builtin_ctzll(copy);
        moves |= magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), from) & allowed;

        copy &= copy-1;
    }

    int count = __builtin_popcountll(moves);

    return 4*count;
}

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

int pst_value(int piece, int sq, int endgame)
{
    assert(sq >= 0);
    assert(sq < 64);
    assert(piece >= 0);
    assert(piece < 6);

    return piece_location_bonus[endgame][piece][sq];
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

#ifdef TAPERED_EVAL
    int opening_score = 0;
    int endgame_score = 0;
#endif

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

#ifdef PAWN_CHAINS
                if(board->colour[WHITE] & board->pieces[PAWNS] & magic_moves_pawns(BLACK, sq))
                {
                    score += pawn_chain_value;
                }
#endif
            }

#ifdef TAPERED_EVAL
            opening_score += piece_location_bonus[0][piece_type][sq];
            endgame_score += piece_location_bonus[1][piece_type][sq];
#else
            score += piece_location_bonus[endgame][piece_type][sq];
#endif

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

#ifdef PAWN_CHAINS
                if(board->colour[BLACK] & board->pieces[PAWNS] & magic_moves_pawns(WHITE, sq))
                {
                    score -= pawn_chain_value;
                }
#endif
            }

#ifdef TAPERED_EVAL
            opening_score -= piece_location_bonus[0][piece_type][sq^56];
            endgame_score -= piece_location_bonus[1][piece_type][sq^56];
#else
            score -= piece_location_bonus[endgame][piece_type][sq^56];
#endif

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

#ifdef TAPERED_EVAL
    int phase = get_phase(num_wN + num_bN, num_wB + num_bB, num_wR + num_bR, num_wQ + num_bQ);
    score += ((opening_score * (256 - phase)) + (endgame_score * phase)) / 256;
#endif

#ifdef KING_SAFETY
    // White King
    score += king_safety(board, WHITE);
    score -= king_safety(board, BLACK);
#endif

    // Piece mobility
#ifdef PIECE_MOBILITY
    score += piece_mobility(board, WHITE);
    score -= piece_mobility(board, BLACK);
#endif

    if(board->turn == BLACK)
    {
        score = -score;
    }

    return score;
}
