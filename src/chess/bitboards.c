#include <assert.h>
#include <stdlib.h>
#include "defs.h"
#include "bitboards.h"

uint64_t magic_moves[89524];
uint64_t pawn_mask[2][64];
uint64_t knight_mask[64];
uint64_t bishop_mask[64];
uint64_t rook_mask[64];
uint64_t king_mask[64];

uint64_t passed_pawn_blockers[2][64];
uint64_t outpost_attackers[2][64];

const uint64_t files[8] = {U64_FILE_A, U64_FILE_B, U64_FILE_C, U64_FILE_D, U64_FILE_E, U64_FILE_F, U64_FILE_G, U64_FILE_H};
const uint64_t adj_files[8] = {U64_FILE_B, U64_FILE_A|U64_FILE_C, U64_FILE_B|U64_FILE_D, U64_FILE_C|U64_FILE_E,
                               U64_FILE_D|U64_FILE_F, U64_FILE_E|U64_FILE_G, U64_FILE_F|U64_FILE_H, U64_FILE_G};

const uint64_t bishop_magic[64] = {
    0x404040404040ULL,   0xa060401007fcULL,   0x401020200000ULL,  0x806004000000ULL,
    0x440200000000ULL,   0x80100800000ULL,    0x104104004000ULL,  0x20020820080ULL,
    0x40100202004ULL,    0x20080200802ULL,    0x10040080200ULL,   0x8060040000ULL,
    0x4402000000ULL,     0x21c100b200ULL,     0x400410080ULL,     0x3f7f05fffc0ULL,
    0x4228040808010ULL,  0x200040404040ULL,   0x400080808080ULL,  0x200200801000ULL,
    0x240080840000ULL,   0x18000c03fff8ULL,   0xa5840208020ULL,   0x58408404010ULL,
    0x2022000408020ULL,  0x402000408080ULL,   0x804000810100ULL,  0x100403c0403ffULL,
    0x78402a8802000ULL,  0x101000804400ULL,   0x80800104100ULL,   0x400480101008ULL,
    0x1010102004040ULL,  0x808090402020ULL,   0x7fefe08810010ULL, 0x3ff0f833fc080ULL,
    0x7fe08019003042ULL, 0x202040008040ULL,   0x1004008381008ULL, 0x802003700808ULL,
    0x208200400080ULL,   0x104100200040ULL,   0x3ffdf7f833fc0ULL, 0x8840450020ULL,
    0x20040100100ULL,    0x7fffdd80140028ULL, 0x202020200040ULL,  0x1004010039004ULL,
    0x40041008000ULL,    0x3ffefe0c02200ULL,  0x1010806000ULL,    0x08403000ULL,
    0x100202000ULL,      0x40100200800ULL,    0x404040404000ULL,  0x6020601803f4ULL,
    0x3ffdfdfc28048ULL,  0x820820020ULL,      0x10108060ULL,      0x00084030ULL,
    0x01002020ULL,       0x40408020ULL,       0x4040404040ULL,    0x404040404040ULL
};

const uint64_t *bishop_offsets[64] = {
    magic_moves+33104, magic_moves+4094,  magic_moves+24764, magic_moves+13882,
    magic_moves+23090, magic_moves+32640, magic_moves+11558, magic_moves+32912,
    magic_moves+13674, magic_moves+6109,  magic_moves+26494, magic_moves+17919,
    magic_moves+25757, magic_moves+17338, magic_moves+16983, magic_moves+16659,
    magic_moves+13610, magic_moves+2224,  magic_moves+60405, magic_moves+7983,
    magic_moves+17,    magic_moves+34321, magic_moves+33216, magic_moves+17127,
    magic_moves+6397,  magic_moves+22169, magic_moves+42727, magic_moves+155,
    magic_moves+8601,  magic_moves+21101, magic_moves+29885, magic_moves+29340,
    magic_moves+19785, magic_moves+12258, magic_moves+50451, magic_moves+1712,
    magic_moves+78475, magic_moves+7855,  magic_moves+13642, magic_moves+8156,
    magic_moves+4348,  magic_moves+28794, magic_moves+22578, magic_moves+50315,
    magic_moves+85452, magic_moves+32816, magic_moves+13930, magic_moves+17967,
    magic_moves+33200, magic_moves+32456, magic_moves+7762,  magic_moves+7794,
    magic_moves+22761, magic_moves+14918, magic_moves+11620, magic_moves+15925,
    magic_moves+32528, magic_moves+12196, magic_moves+32720, magic_moves+26781,
    magic_moves+19817, magic_moves+24732, magic_moves+25468, magic_moves+10186
};

const uint64_t rook_magic[64] = {
    0x280077ffebfffeULL,   0x2004010201097fffULL, 0x10020010053fffULL, 0x30002ff71ffffaULL,
    0x7fd00441ffffd003ULL, 0x4001d9e03ffff7ULL,   0x4000888847ffffULL, 0x6800fbff75fffdULL,
    0x28010113ffffULL,     0x20040201fcffffULL,   0x7fe80042ffffe8ULL, 0x1800217fffe8ULL,
    0x1800073fffe8ULL,     0x7fe8009effffe8ULL,   0x1800602fffe8ULL,   0x30002fffffa0ULL,
    0x300018010bffffULL,   0x3000c0085fffbULL,    0x4000802010008ULL,  0x2002004002002ULL,
    0x2002020010002ULL,    0x1002020008001ULL,    0x4040008001ULL,     0x802000200040ULL,
    0x40200010080010ULL,   0x80010040010ULL,      0x4010008020008ULL,  0x40020200200ULL,
    0x10020020020ULL,      0x10020200080ULL,      0x8020200040ULL,     0x200020004081ULL,
    0xfffd1800300030ULL,   0x7fff7fbfd40020ULL,   0x3fffbd00180018ULL, 0x1fffde80180018ULL,
    0xfffe0bfe80018ULL,    0x1000080202001ULL,    0x3fffbff980180ULL,  0x1fffdff9000e0ULL,
    0xfffeebfeffd800ULL,   0x7ffff7ffc01400ULL,   0x408104200204ULL,   0x1ffff01fc03000ULL,
    0xfffe7f8bfe800ULL,    0x8001002020ULL,       0x3fff85fffa804ULL,  0x1fffd75ffa802ULL,
    0xffffec00280028ULL,   0x7fff75ff7fbfd8ULL,   0x3fff863fbf7fd8ULL, 0x1fffbfdfd7ffd8ULL,
    0xffff810280028ULL,    0x7ffd7f7feffd8ULL,    0x3fffc0c480048ULL,  0x1ffffafd7ffd8ULL,
    0xffffe4ffdfa3baULL,   0x7fffef7ff3d3daULL,   0x3fffbfdfeff7faULL, 0x1fffeff7fbfc22ULL,
    0x20408001001ULL,      0x7fffeffff77fdULL,    0x3ffffbf7dfeecULL,  0x1ffff9dffa333ULL
};

const uint64_t *rook_offsets[64] = {
    magic_moves+41305, magic_moves+14326, magic_moves+24477, magic_moves+8223,
    magic_moves+49795, magic_moves+60546, magic_moves+28543, magic_moves+79282,
    magic_moves+6457,  magic_moves+4125,  magic_moves+81021, magic_moves+42341,
    magic_moves+14139, magic_moves+19465, magic_moves+9514,  magic_moves+71090,
    magic_moves+75419, magic_moves+33476, magic_moves+27117, magic_moves+85964,
    magic_moves+54915, magic_moves+36544, magic_moves+71854, magic_moves+37996,
    magic_moves+30398, magic_moves+55939, magic_moves+53891, magic_moves+56963,
    magic_moves+77451, magic_moves+12319, magic_moves+88500, magic_moves+51405,
    magic_moves+72878, magic_moves+676,   magic_moves+83122, magic_moves+22206,
    magic_moves+75186, magic_moves+681,   magic_moves+36453, magic_moves+20369,
    magic_moves+1981,  magic_moves+13343, magic_moves+10650, magic_moves+57987,
    magic_moves+26302, magic_moves+58357, magic_moves+40546, magic_moves+0,
    magic_moves+14967, magic_moves+80361, magic_moves+40905, magic_moves+58347,
    magic_moves+20381, magic_moves+81868, magic_moves+59381, magic_moves+84404,
    magic_moves+45811, magic_moves+62898, magic_moves+45796, magic_moves+66994,
    magic_moves+67204, magic_moves+32448, magic_moves+62946, magic_moves+17005
};

uint64_t permute(uint64_t set, uint64_t subset)
{
    return (subset - set) & set;
}

uint64_t calculate_rook_mask(int sq)
{
    uint64_t result = 0;
    int file = SQ_TO_FILE(sq);
    int rank = SQ_TO_RANK(sq);
    int i;

    // Right
    for(i = rank+1; i <= 6; ++i)
    {
        result |= ((uint64_t)1 << (file + i*8));
    }

    // Left
    for(i = rank-1; i >= 1; --i)
    {
        result |= ((uint64_t)1 << (file + i*8));
    }

    // Up
    for(i = file+1; i <= 6; ++i)
    {
        result |= ((uint64_t)1 << (i + rank*8));
    }

    // Down
    for(i = file-1; i >= 1; --i)
    {
        result |= ((uint64_t)1 << (i + rank*8));
    }

    return result;
}

uint64_t calculate_bishop_mask(int sq)
{
    uint64_t result = 0;
    int file = SQ_TO_FILE(sq);
    int rank = SQ_TO_RANK(sq);
    int x;
    int y;

    // Up 1 Right 1
    for(y = rank+1, x = file+1; y <= 6 && x <= 6; ++y, ++x)
    {
        result |= ((uint64_t)1 << (x + y*8));
    }

    // Up 1 Left 1
    for(y = rank+1, x = file-1; y <= 6 && x >= 1; ++y, --x)
    {
        result |= ((uint64_t)1 << (x + y*8));
    }

    // Down 1 Right 1
    for(y = rank-1, x = file+1; y >= 1 && x <= 6; --y, ++x)
    {
        result |= ((uint64_t)1 << (x + y*8));
    }

    // Down 1 Left 1
    for(y = rank-1, x = file-1; y >= 1 && x >= 1; --y, --x)
    {
        result |= ((uint64_t)1 << (x + y*8));
    }

    return result;
}

uint64_t calculate_rook_moves(int sq, uint64_t blockers)
{
    uint64_t result = 0;
    int file = SQ_TO_FILE(sq);
    int rank = SQ_TO_RANK(sq);
    int i;

    // Up
    for(i = rank+1; i <= 7; ++i)
    {
        result |= ((uint64_t)1 << (file + i*8));
        if(blockers & ((uint64_t)1 << (file + i*8)))
        {
            break;
        }
    }

    // Down
    for(i = rank-1; i >= 0; --i)
    {
        result |= ((uint64_t)1 << (file + i*8));
        if(blockers & ((uint64_t)1 << (file + i*8)))
        {
            break;
        }
    }

    // Right
    for(i = file+1; i <= 7; ++i)
    {
        result |= ((uint64_t)1 << (i + rank*8));
        if(blockers & ((uint64_t)1 << (i + rank*8)))
        {
            break;
        }
    }

    // Left
    for(i = file-1; i >= 0; --i)
    {
        result |= ((uint64_t)1 << (i + rank*8));
        if(blockers & ((uint64_t)1 << (i + rank*8)))
        {
            break;
        }
    }

    return result;
}

uint64_t calculate_bishop_moves(int sq, uint64_t blockers)
{
    uint64_t result = 0;
    int file = SQ_TO_FILE(sq);
    int rank = SQ_TO_RANK(sq);
    int x;
    int y;

    // Up 1 Right 1
    for(y = rank+1, x = file+1; y <= 7 && x <= 7; ++y, ++x)
    {
        result |= ((uint64_t)1 << (x + y*8));
        if(blockers & ((uint64_t)1 << (x + y*8)))
        {
            break;
        }
    }

    // Up 1 Left 1
    for(y = rank+1, x = file-1; y <= 7 && x >= 0; ++y, --x)
    {
        result |= ((uint64_t)1 << (x + y*8));
        if(blockers & ((uint64_t)1 << (x + y*8)))
        {
            break;
        }
    }

    // Down 1 Right 1
    for(y = rank-1, x = file+1; y >= 0 && x <= 7; --y, ++x)
    {
        result |= ((uint64_t)1 << (x + y*8));
        if(blockers & ((uint64_t)1 << (x + y*8)))
        {
            break;
        }
    }

    // Down 1 Left 1
    for(y = rank-1, x = file-1; y >= 0 && x >= 0; --y, --x)
    {
        result |= ((uint64_t)1 << (x + y*8));
        if(blockers & ((uint64_t)1 << (x + y*8)))
        {
            break;
        }
    }

    return result;
}

uint64_t magic_moves_pawns(int side, int sq)
{
    return pawn_mask[side][sq];
}

uint64_t magic_moves_knight(int sq)
{
    return knight_mask[sq];
}

uint64_t magic_moves_bishop(uint64_t occ, int sq)
{
    return *(bishop_offsets[sq] + (((occ & bishop_mask[sq]) * bishop_magic[sq]) >> 55));
}

uint64_t magic_moves_rook(uint64_t occ, int sq)
{
    return *(rook_offsets[sq] + (((occ & rook_mask[sq]) * rook_magic[sq]) >> 52));
}

uint64_t magic_moves_queen(uint64_t occ, int sq)
{
    return magic_moves_rook(occ, sq) | magic_moves_bishop(occ, sq);
}

uint64_t magic_moves_king(int sq)
{
    return king_mask[sq];
}

uint64_t is_passed_pawn(int side, int sq, uint64_t blockers)
{
    return !(passed_pawn_blockers[side][sq] & blockers);
}

uint64_t is_outpost(int side, int sq, uint64_t enemy_pawns)
{
    return !(outpost_attackers[side][sq] & enemy_pawns);
}

uint64_t is_backward_pawn_white(int sq, uint64_t friendly, uint64_t enemy)
{
    if(!(magic_moves_pawns(WHITE, sq+8) & enemy)) {return 0;}
    return !(passed_pawn_blockers[BLACK][sq-8] & friendly);
}

uint64_t is_backward_pawn_black(int sq, uint64_t friendly, uint64_t enemy)
{
    if(!(magic_moves_pawns(BLACK, sq-8) & enemy)) {return 0;}
    return !(passed_pawn_blockers[WHITE][sq+8] & friendly);
}

uint64_t get_file(int file)
{
    return files[file];
}

uint64_t get_adj_files(int file)
{
    return adj_files[file];
}

int u64_file(uint64_t pos)
{
    assert(pos);

    if(pos&U64_FILE_A) {return 0;}
    if(pos&U64_FILE_B) {return 1;}
    if(pos&U64_FILE_C) {return 2;}
    if(pos&U64_FILE_D) {return 3;}
    if(pos&U64_FILE_E) {return 4;}
    if(pos&U64_FILE_F) {return 5;}
    if(pos&U64_FILE_G) {return 6;}
    return 7;
}

int u64_rank(uint64_t pos)
{
    assert(pos);

    if(pos&U64_RANK_1) {return 0;}
    if(pos&U64_RANK_2) {return 1;}
    if(pos&U64_RANK_3) {return 2;}
    if(pos&U64_RANK_4) {return 3;}
    if(pos&U64_RANK_5) {return 4;}
    if(pos&U64_RANK_6) {return 5;}
    if(pos&U64_RANK_7) {return 6;}
    return 7;
}

void bitboards_init()
{
    uint64_t *index = NULL;

    int sq;
    for(sq = 0; sq < 64; ++sq)
    {
        uint64_t perm;
        uint64_t from;
        int f = SQ_TO_FILE(sq);
        int r = SQ_TO_RANK(sq);

        // Pawns
        from = (uint64_t)1<<sq;
        pawn_mask[WHITE][sq] = ((from << 7) &(~U64_FILE_H)) | ((from << 9) &(~U64_FILE_A));
        pawn_mask[BLACK][sq] = ((from >> 7) &(~U64_FILE_A)) | ((from >> 9) &(~U64_FILE_H));

        // Calculate passed pawn blockers
        if(r == 0 || r == 7)
        {
            passed_pawn_blockers[WHITE][sq] = 0;
            passed_pawn_blockers[BLACK][sq] = 0;
        }
        else
        {
            passed_pawn_blockers[WHITE][sq] = (~U64_RANK_8)&((get_file(f) | get_adj_files(f))<<(r*8+8));
            passed_pawn_blockers[BLACK][sq] = (~U64_RANK_1)&((get_file(f) | get_adj_files(f))>>((7-r)*8+8));
        }

        // Calculate pawns that can attack a square
        if(r == 0)
        {
            outpost_attackers[WHITE][sq] = get_adj_files(f)<<(r*8+8);
            outpost_attackers[BLACK][sq] = 0;
        }
        else if(r == 7)
        {
            outpost_attackers[WHITE][sq] = 0;
            outpost_attackers[BLACK][sq] = get_adj_files(f)>>((7-r)*8+8);
        }
        else
        {
            outpost_attackers[WHITE][sq] = get_adj_files(f)<<(r*8+8);
            outpost_attackers[BLACK][sq] = get_adj_files(f)>>((7-r)*8+8);
        }

        // Knights
        from = (uint64_t)1<<sq;
        knight_mask[sq]  = (from<<17) & (~U64_FILE_A); // Up 2 right 1
        knight_mask[sq] |= (from<<15) & (~U64_FILE_H); // Up 2 left 1
        knight_mask[sq] |= (from>>17) & (~U64_FILE_H); // Down 2 left 1
        knight_mask[sq] |= (from>>15) & (~U64_FILE_A); // Down 2 right 1
        knight_mask[sq] |= (from<<10) & ~(U64_FILE_B|U64_FILE_A); // Right 2 up 1
        knight_mask[sq] |= (from>>6)  & ~(U64_FILE_B|U64_FILE_A); // Right 2 down 1
        knight_mask[sq] |= (from<<6)  & ~(U64_FILE_H|U64_FILE_G); // Left 2 up 1
        knight_mask[sq] |= (from>>10) & ~(U64_FILE_H|U64_FILE_G); // Left 2 down 1

        // Bishops
        perm = 0;
        bishop_mask[sq] = calculate_bishop_mask(sq);
        do
        {
            index = (uint64_t*)(bishop_offsets[sq] + (((perm & bishop_mask[sq]) * bishop_magic[sq]) >> 55));
            *index = calculate_bishop_moves(sq, perm);
        } while ((perm = permute(bishop_mask[sq], perm)));

        // Rooks
        perm = 0;
        rook_mask[sq] = calculate_rook_mask(sq);
        do
        {
            index = (uint64_t*)(rook_offsets[sq] + (((perm & rook_mask[sq]) * rook_magic[sq]) >> 52));
            *index = calculate_rook_moves(sq, perm);
        } while ((perm = permute(rook_mask[sq], perm)));

        // King
        from = (uint64_t)1<<sq;
        king_mask[sq]  = (from<<8); // Up 1
        king_mask[sq] |= (from>>8); // Down 1
        king_mask[sq] |= (from<<1) & (~U64_FILE_A); // Right 1
        king_mask[sq] |= (from>>1) & (~U64_FILE_H); // Left 1
        king_mask[sq] |= (from<<9) & (~U64_FILE_A); // Up 1 Right 1
        king_mask[sq] |= (from<<7) & (~U64_FILE_H); // Up 1 Left 1
        king_mask[sq] |= (from>>9) & (~U64_FILE_H); // Down 1 Left 1
        king_mask[sq] |= (from>>7) & (~U64_FILE_A); // Down 1 Right 1
    }
}

uint64_t pinned_pieces_white(const s_board *board, int sq)
{
    assert(board != NULL);
    assert(0 <= sq);
    assert(sq < 64);

    uint64_t pinned = 0;
    uint64_t pos;
    uint64_t blockers;

    // Bishops and Queens
    blockers = magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE];

    while(blockers)
    {
        pos = blockers & ~(blockers-1);

        if(magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]) ^ pos, sq) & (board->colour[BLACK] & (board->pieces[BISHOPS]|board->pieces[QUEENS])))
        {
            pinned ^= pos;
        }

        blockers = blockers ^ pos;
    }

    // Rooks and Queens
    blockers = magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE];

    while(blockers)
    {
        pos = blockers & ~(blockers-1);

        if(magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]) ^ pos, sq) & (board->colour[BLACK] & (board->pieces[ROOKS]|board->pieces[QUEENS])))
        {
            pinned ^= pos;
        }

        blockers = blockers ^ pos;
    }

    return pinned;
}

uint64_t pinned_pieces_black(const s_board *board, int sq)
{
    assert(board != NULL);
    assert(0 <= sq);
    assert(sq < 64);

    uint64_t pinned = 0;
    uint64_t pos;
    uint64_t blockers;

    // Bishops and Queens
    blockers = magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[BLACK];

    while(blockers)
    {
        pos = blockers & ~(blockers-1);

        if(magic_moves_bishop((board->colour[WHITE]|board->colour[BLACK]) ^ pos, sq) & (board->colour[WHITE] & (board->pieces[BISHOPS]|board->pieces[QUEENS])))
        {
            pinned ^= pos;
        }

        blockers = blockers ^ pos;
    }

    // Rooks and Queens
    blockers = magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[BLACK];

    while(blockers)
    {
        pos = blockers & ~(blockers-1);

        if(magic_moves_rook((board->colour[WHITE]|board->colour[BLACK]) ^ pos, sq) & (board->colour[WHITE] & (board->pieces[ROOKS]|board->pieces[QUEENS])))
        {
            pinned ^= pos;
        }

        blockers = blockers ^ pos;
    }

    return pinned;
}

int error_check(const s_board *board)
{
    assert(board != NULL);

    if(__builtin_popcountll(board->colour[WHITE]) > 16) {return 1;}
    if(__builtin_popcountll(board->colour[BLACK]) > 16) {return 2;}

    if(board->colour[WHITE] & board->colour[BLACK]) {return 3;}

    if(board->pieces[PAWNS] & board->pieces[KNIGHTS]) {return 4;}
    if(board->pieces[PAWNS] & board->pieces[BISHOPS]) {return 5;}
    if(board->pieces[PAWNS] & board->pieces[ROOKS]) {return 6;}
    if(board->pieces[PAWNS] & board->pieces[QUEENS]) {return 7;}
    if(board->pieces[PAWNS] & board->pieces[KINGS]) {return 8;}

    if(board->pieces[KNIGHTS] & board->pieces[BISHOPS]) {return 9;}
    if(board->pieces[KNIGHTS] & board->pieces[ROOKS]) {return 10;}
    if(board->pieces[KNIGHTS] & board->pieces[QUEENS]) {return 11;}
    if(board->pieces[KNIGHTS] & board->pieces[KINGS]) {return 12;}

    if(board->pieces[BISHOPS] & board->pieces[ROOKS]) {return 13;}
    if(board->pieces[BISHOPS] & board->pieces[QUEENS]) {return 14;}
    if(board->pieces[BISHOPS] & board->pieces[KINGS]) {return 15;}

    if(board->pieces[ROOKS] & board->pieces[QUEENS]) {return 16;}
    if(board->pieces[ROOKS] & board->pieces[KINGS]) {return 17;}

    if(board->pieces[QUEENS] & board->pieces[KINGS]) {return 18;}

    if(__builtin_popcountll(board->pieces[PAWNS]) > 16) {return 19;}
    if(__builtin_popcountll(board->pieces[KNIGHTS]) > 10) {return 20;}
    if(__builtin_popcountll(board->pieces[BISHOPS]) > 10) {return 21;}
    if(__builtin_popcountll(board->pieces[ROOKS]) > 10) {return 22;}
    if(__builtin_popcountll(board->pieces[QUEENS]) > 10) {return 23;}
    if(__builtin_popcountll(board->pieces[KINGS]) != 2) {return 24;}

    if((U64_RANK_1 | U64_RANK_8) & board->pieces[PAWNS]) {return 25;}

    return 0;
}
