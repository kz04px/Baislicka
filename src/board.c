#include <string.h>
#include <assert.h>
#include "defs.h"
#include "hashtable.h"
#include "board.h"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

const char temp[256] = START_FEN;

void reset_hh_bf(s_board *board)
{
#ifdef QUIET_SORT_HISTORY_HEURISTIC
    for(int x = 0; x < 64; ++x)
    {
        int y;
        for(y = 0; y < 64; ++y)
        {
            board->hh_score[x][y] = 0;
            board->bf_score[x][y] = 1;
        }
    }
#endif
}

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
        assert(0 <= rank && rank <= 7);
        board->ep = rank*8 + file;
        n++;
    }
    n += 2;

    board->num_halfmoves = atoi(&fen[n]);

#ifdef HASHTABLE
        board->key = create_key_board(board);
#endif

    // History
    board->key_history[0] = board->key;
    board->history_size = 1;

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
        if(a->key_history[i] != b->key_history[i]) {return 0;}
    }

    return 1;
}

int store_irreversible(s_irreversible *info, const s_board *board)
{
    assert(board);
    assert(info);

#ifdef HASHTABLE
    info->key           = board->key;
#endif
    info->num_halfmoves = board->num_halfmoves;
    info->ep            = board->ep;
    info->castling      = board->castling;

    return 0;
}

int restore_irreversible(const s_irreversible *info, s_board *board)
{
    assert(board);
    assert(info);

#ifdef HASHTABLE
    board->key           = info->key;
#endif
    board->num_halfmoves = info->num_halfmoves;
    board->ep            = info->ep;
    board->castling      = info->castling;

    return 0;
}
