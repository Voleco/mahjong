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

template <typename State, typename Environment>
inline bool BeamSearch(const State &start, std::vector<State> &path, const Environment *env, int beam_size, int max_depth)
{
    struct search_node
    {
        search_node(State _s, int _g, int _h, int _par_idx)
            : s(_s), g(_g), h(_h), parent_idx(_par_idx) {}
        State s;
        int g;
        int h;
        int parent_idx;
    };

    path.clear();

    std::vector<std::vector<search_node>> openlist(max_depth);
    for (int i = 0; i < max_depth; i++)
        openlist[i].reserve(beam_size);

    auto lam_extract_path = [&](search_node &goal_node)
    {
        search_node &cur_node = goal_node;
        while (true)
        {
            path.push_back(cur_node.s);
            int par_depth = cur_node.g - 1;
            int par_idx = cur_node.parent_idx;
            if (par_depth < 0)
                break;
            cur_node = openlist[par_depth][par_idx];
        }

        std::reverse(path.begin(), path.end());
    };

    int init_hcost = env->HCost(start);
    openlist[0].push_back(search_node(start, 0, init_hcost, -1));
    int depth = 0;
    while (depth < max_depth)
    {
        std::vector<search_node> &cur_level = openlist[depth];
        std::vector<search_node> next_level_candidates;
        for (int i = 0; i < int(cur_level.size()); i++)
        {
            auto &cur_s = cur_level[i];
            if (env->is_Win(cur_s.s))
            {
                lam_extract_path(cur_s);
                return true;
            }
            std::vector<State> nbs;
            env->GetNeighbors(cur_s.s, nbs);
            for (auto child : nbs)
            {
                int hcost = env->HCost(child);
                next_level_candidates.emplace_back(
                    search_node(child, depth + 1, hcost, i));
            }
        }
        std::sort(next_level_candidates.begin(), next_level_candidates.end(), [](auto &left, auto &right)
                  { return (left.g + left.h) < (right.g + right.h); });
        // next_level_candidates.resize(beam_size);

        depth++;
        for (int x = 0; x < beam_size; x++)
            openlist[depth].push_back(next_level_candidates[x]);

        // openlist[depth] = std::move(next_level_candidates);
    }

    return false;
}
