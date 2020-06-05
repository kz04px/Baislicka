#include <stdint.h>
#include "tests.h"
#include "chess/bitboards.h"
#include "chess/board.h"
#include "chess/move.h"
#include "chess/perft.h"

typedef struct
{
    char fen[256];
    uint64_t nodes[6];
} s_test;

int test_perft()
{
    const s_test tests[] = {
        {"startpos", {1, 20, 400, 8902}},
        {"8/4k3/8/8/8/8/4K3/8 w - - 0 1", {1, 8}},
        {"7k/8/8/8/8/4B3/8/7K w - - 0 1", {1, 14}},
        {"7k/8/8/8/8/4N3/8/7K w - - 0 1", {1, 11}},
        {"4k3/4r3/8/8/8/3p4/4P3/4K3 w - - 0 1", {1, 6}},
        {"4k3/4r3/8/8/8/8/3p4/4K3 w - - 0 2", {1, 4}},
        {"4k3/8/8/8/8/8/1r4K1/5b2 w - - 0 2", {1, 5}},
        {"4k3/2b3q1/3P1P2/4K3/3P1P2/2b3q1/8/8 w - - 0 1", {1, 6}},
        {"4k3/b7/8/2Pp4/8/8/8/6K1 w - d6 0 2", {1, 5}},
        {"4k3/7b/8/4pP2/8/8/8/1K6 w - e6 0 2", {1, 5}},
        {"6k1/8/8/8/2pP4/8/B7/3K4 b - d3 0 2", {1, 5}},
        {"1k6/8/8/8/4Pp2/8/7B/4K3 b - e3 0 2", {1, 5}},
        {"4k3/b7/8/1pP5/8/8/8/6K1 w - b6 0 2", {1, 6}},
        {"4k3/7b/8/5Pp1/8/8/8/1K6 w - g6 0 2", {1, 6}},
        {"6k1/8/8/8/1Pp5/8/B7/4K3 b - b3 0 2", {1, 6}},
        {"1k6/8/8/8/5pP1/8/7B/4K3 b - g3 0 2", {1, 6}},
        {"4k3/K7/8/1pP5/8/8/8/6b1 w - b6 0 2", {1, 6}},
        {"4k3/7K/8/5Pp1/8/8/8/1b6 w - g6 0 2", {1, 6}},
        {"6B1/8/8/8/1Pp5/8/k7/4K3 b - b3 0 2", {1, 6}},
        {"1B6/8/8/8/5pP1/8/7k/4K3 b - g3 0 2", {1, 6}},
        {"4k3/8/8/K2pP2r/8/8/8/8 w - d6 0 1", {1, 6}},
        {"4k3/8/8/r2pP2K/8/8/8/8 w - d6 0 1", {1, 6}},
        {"8/8/8/8/1k1Pp2R/8/8/4K3 b - d3 0 1", {1, 8}},
        {"8/8/8/8/1R1Pp2k/8/8/4K3 b - d3 0 1", {1, 6}},
        {"k7/8/4r3/3pP3/8/8/8/4K3 w - d6 0 1", {1, 5}},
        {"k3K3/8/8/3pP3/8/8/8/4r3 w - d6 0 1", {1, 6}},
        {"8/8/8/8/1k1PpN1R/8/8/4K3 b - d3 0 1", {1, 9, 193, 1322}},
        {"8/8/8/8/1k1Ppn1R/8/8/4K3 b - d3 0 1", {1, 17, 220, 3001}},
        {"4k3/8/8/2PpP3/8/8/8/4K3 w - d6 0 1", {1, 9, 47, 376}},
        {"4k3/8/8/8/2pPp3/8/8/4K3 b - d3 0 1", {1, 9, 47, 376}},
        {"4k3/8/8/4pP2/3K4/8/8/8 w - e6 0 2", {1, 9}},
        {"8/8/8/4k3/5Pp1/8/8/3K4 b - f3 0 1", {1, 9}},
        {"4k3/8/K6r/3pP3/8/8/8/8 w - d6 0 1", {1, 6}},
    };

    s_board board;
    for (int t = 0; t < 33; ++t)
    {
        set_fen(&board, tests[t].fen);

        for (int n = 0; n < 6; ++n)
        {
            if (tests[t].nodes[n] == 0)
            {
                continue;
            }
            else if (perft_search(&board, n) != tests[t].nodes[n])
            {
                return -2;
            }
        }
    }

    return 0;
}
