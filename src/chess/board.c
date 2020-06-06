#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "defs.h"
#include "board.h"
#include "zobrist.h"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

const char temp[256] = START_FEN;

int set_fen(s_board *board, const char *fen)
{
    assert(board != NULL);
    assert(fen != NULL);

    if(strncmp(fen, "startpos", 8) == 0)
    {
        fen = temp;
    }

    for(int i = 0; i < 6; ++i)
    {
        board->pieces[i] = 0;
    }
    board->colour[WHITE] = 0;
    board->colour[BLACK] = 0;
    board->ep = 0;
    board->castling = 0;
    board->history_size = 0;

    /*
    for(int i = 0; i < 12; ++i)
    {
        board->num_pieces[i] = 0;
        for(int j = 0; j < 10; ++j)
        {
            board->locations[i][j] = 0;
        }
    }
    */

    int n = 0, x = A8;
    while(fen[n] != ' ')
    {
        switch(fen[n])
        {
            case '/':
            {
                x -= 17;
                break;
            }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            {
                x += fen[n] - '1'; // FIXME: fen[n] = '0';
                break;
            }
            case 'p':
            {
                board->pieces[PAWNS] ^= (uint64_t)1<<x;
                board->colour[BLACK] ^= (uint64_t)1<<x;
                break;
            }
            case 'n':
            {
                board->pieces[KNIGHTS] ^= (uint64_t)1<<x;
                board->colour[BLACK] ^= (uint64_t)1<<x;
                break;
            }
            case 'b':
            {
                board->pieces[BISHOPS] ^= (uint64_t)1<<x;
                board->colour[BLACK] ^= (uint64_t)1<<x;
                break;
            }
            case 'r':
            {
                board->pieces[ROOKS] ^= (uint64_t)1<<x;
                board->colour[BLACK] ^= (uint64_t)1<<x;
                break;
            }
            case 'q':
            {
                board->pieces[QUEENS] ^= (uint64_t)1<<x;
                board->colour[BLACK] ^= (uint64_t)1<<x;
                break;
            }
            case 'k':
            {
                board->pieces[KINGS] ^= (uint64_t)1<<x;
                board->colour[BLACK] ^= (uint64_t)1<<x;
                break;
            }
            case 'P':
            {
                board->pieces[PAWNS] ^= (uint64_t)1<<x;
                board->colour[WHITE] ^= (uint64_t)1<<x;
                break;
            }
            case 'N':
            {
                board->pieces[KNIGHTS] ^= (uint64_t)1<<x;
                board->colour[WHITE] ^= (uint64_t)1<<x;
                break;
            }
            case 'B':
            {
                board->pieces[BISHOPS] ^= (uint64_t)1<<x;
                board->colour[WHITE] ^= (uint64_t)1<<x;
                break;
            }
            case 'R':
            {
                board->pieces[ROOKS] ^= (uint64_t)1<<x;
                board->colour[WHITE] ^= (uint64_t)1<<x;
                break;
            }
            case 'Q':
            {
                board->pieces[QUEENS] ^= (uint64_t)1<<x;
                board->colour[WHITE] ^= (uint64_t)1<<x;
                break;
            }
            case 'K':
            {
                board->pieces[KINGS] ^= (uint64_t)1<<x;
                board->colour[WHITE] ^= (uint64_t)1<<x;
                break;
            }
            default:
            {
                break;
            }
        }
        n++;
        x++;
    }
    n += 1;

    if(fen[n] == 'w')
    {
        board->turn = WHITE;
    }
    else if(fen[n] == 'b')
    {
        board->turn = BLACK;
    }
    else
    {
        return -1;
    }
    n += 2;

    while(fen[n] != ' ' && fen[n] != '\0' && fen[n] != '\n')
    {
        switch(fen[n])
        {
            case 'K':
            {
                board->castling |= wKSC;
                break;
            }
            case 'Q':
            {
                board->castling |= wQSC;
                break;
            }
            case 'k':
            {
                board->castling |= bKSC;
                break;
            }
            case 'q':
            {
                board->castling |= bQSC;
                break;
            }
            case '-':
            {
                break;
            }
            default:
            {
                return -2;
            }
        }
        n++;
    }
    n++;

    if(fen[n] != '-')
    {
        int file = fen[n] - 'a';
        int rank = fen[n+1] - '1';
        assert(0 <= file && file <= 7);
        assert(rank == 2 || rank == 5);
        board->ep = rank*8 + file;
        n++;
    }
    n += 2;

    board->num_halfmoves = atoi(&fen[n]);

    board->key = create_key_board(board);

    assert(board->ep == 0 || (1ULL<<board->ep) & (U64_RANK_3 | U64_RANK_6));

    return 0;
}

int board_equality(const s_board *a, const s_board *b)
{
    if(a->turn != b->turn) {return 0;}
    if(a->castling != b->castling) {return 0;}
    if(a->ep != b->ep) {return 0;}
    if(a->key != b->key) {return 0;}
    if(a->num_halfmoves != b->num_halfmoves) {return 0;}

    // Pieces
    for(int i = 0; i < 6; ++i)
    {
        if(a->pieces[i] != b->pieces[i]) {return 0;}
    }

    // Colours
    if(a->colour[WHITE] != b->colour[WHITE]) {return 0;}
    if(a->colour[BLACK] != b->colour[BLACK]) {return 0;}

    // History
    if(a->history_size != b->history_size) {return 0;}
    for(int i = 0; i < a->history_size; ++i)
    {
        if(a->history[i].key != b->history[i].key) {return 0;}
    }

    return 1;
}

int is_endgame(const s_board *board)
{
    assert(board != NULL);

    // "side to move has three or less non-pawn pieces including king"
    return __builtin_popcountll(board->colour[board->turn] & (board->pieces[KNIGHTS] | board->pieces[BISHOPS] | board->pieces[ROOKS] | board->pieces[QUEENS])) <= 2;
}

int is_fifty_move_draw(const s_board *board)
{
    assert(board != NULL);

    if(board->num_halfmoves >= 100)
    {
        return 1;
    }

    return 0;
}

int is_threefold(const s_board *board)
{
    assert(board != NULL);

    if(board->num_halfmoves < 8)
    {
        return 0;
    }

    int repeats = 0;
    int lim = board->num_halfmoves+1 < board->history_size ? board->num_halfmoves+1 : board->history_size;

    for(int i = 1; i <= lim; ++i)
    {
        assert(board->history_size-i >= 0);

        if(board->history[board->history_size-i].key == board->key)
        {
            repeats++;

            if(repeats >= 2)
            {
                return 1;
            }
        }
    }
    return 0;
}
