#ifndef EVAL_PHASE_H
#define EVAL_PHASE_H

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

#endif
