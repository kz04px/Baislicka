#include <stdio.h>
#include "tests.h"
#include "chess/bitboards.h"

#define TEST_PASS(x) ((x) == 0) ? "Passed" : "Failed", x

int main()
{
    bitboards_init();
    printf("Tests:\n");
    printf(" 1) Move legality:  %s (%i)\n", TEST_PASS(test_move_legality()));
    printf(" 2) Eval mirroring: %s (%i)\n", TEST_PASS(test_eval_mirroring()));
    printf(" 3) Backward pawns: %s (%i)\n", TEST_PASS(test_backward_pawns()));
    printf(" 4) Passed pawns:   %s (%i)\n", TEST_PASS(test_passed_pawns()));
    printf(" 5) Make undo:      %s (%i)\n", TEST_PASS(test_make_undo()));
    printf("\n");
}
