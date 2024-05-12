#pragma once

#include <vector>

/*dir == 1 to the right
dir == -1 to the left*/
template <typename T>
int get_next_index(const std::vector<T> &seq, int cur_idx)
{
    for (int i = cur_idx + 1; i < int(seq.size()); i++)
        if (seq[i] != seq[cur_idx])
            return i;

    return seq.size();
}

template <typename T>
int get_prev_index(const std::vector<T> &seq, int cur_idx)
{
    for (int i = cur_idx - 1; i >= 0; i--)
        if (seq[i] != seq[cur_idx])
            return i;

    return -1;
}