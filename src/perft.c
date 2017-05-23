#include "defs.h"
#include "search.h"
#include "attack.h"
#include "movegen.h"
#include "bitboards.h"
#include "move.h"
#include "perft.h"
#include "display.h"
#include <string.h>
#include <assert.h>

uint64_t perft_search(s_board *board, int d)
{
  assert(board != NULL);
  assert(d >= 0);
  assert(board->turn == WHITE || board->turn == BLACK);

  if(d == 0)
  {
    return 1;
  }

  s_move moves[MAX_MOVES];
  int num_moves = find_moves_all(board, &moves[0], board->turn);

  // Set old permissions
  s_irreversible permissions;
  store_irreversible(&permissions, board);

  uint64_t nodes = 0ULL;

  int m;
  for(m = 0; m < num_moves; ++m)
  {
    move_make(board, &moves[m]);

    if(square_attacked(board, board->pieces[KINGS]&board->colour[!board->turn], board->turn))
    {
      // Restore old permissions
      restore_irreversible(&permissions, board);

      move_undo(board, &moves[m]);
      continue;
    }

    nodes += perft_search(board, d-1);

    // Restore old permissions
    restore_irreversible(&permissions, board);

    move_undo(board, &moves[m]);
  }

  return nodes;
}
