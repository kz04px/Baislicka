#include <assert.h>
#include <stddef.h>
#include "pv.h"

int legal_pv(s_board *board, const s_pv *pv)
{
    assert(board != NULL);
    assert(pv != NULL);
    assert(pv->num_moves >= 0);
    assert(pv->num_moves < MAX_DEPTH);

    int r = 1;
    int n = 0;

    while(n < pv->num_moves)
    {
        if(is_legal_move(board, &pv->moves[n]) == 0)
        {
            r = 0;
            break;
        }

        move_make(board, &pv->moves[n]);
        n++;
    }

    while(n > 0)
    {
        move_undo(board, &pv->moves[n-1]);
        n--;
    }

    return r;
}
