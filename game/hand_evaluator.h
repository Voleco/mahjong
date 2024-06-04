#pragma once

#include <vector>
#include <algorithm>

#include "cards.h"
#include "deck.hpp"
#include "../utils/util_helpers.h"


/*fisrt non 0 cnt card */
template <typename T>
size_t first_non0(const std::vector<T> &vec)
{
    for (size_t i = 0; i < vec.size(); i++)
        if (vec[i] > 0)
            return i;
    return vec.size();
}

class Hand_Evaluator
{
public:
    Hand_Evaluator() {}
    ~Hand_Evaluator() {}

    bool is_Win(const hand_t &hand) const;
    int HCost(const hand_t &hand) const;

    void GetNeighbors(const hand_t &hand, std::vector<hand_t> &nbs) const;

    /// @brief decomposite hand into maximal number of combos (melds/semi-melds)
    /// @param hand can be any num of cards
    /// @return vector of <combos, remaning cards>
    template <typename T>
    std::vector<decomposed_hand<T>> decomp_hand(const hand_t &hand) const;

    std::vector<full_DH> full_decomp_hand(const hand_t &hand) const;

private:
    bool all_melds(const hand_t &cards) const;

    std::vector<hand_t> extract_meld(const hand_t &cards, int index) const;

    bool can_straight(const hand_t &hand, int index) const;
    bool can_triple(const hand_t &hand, int index) const;

    bool is_leaf(const decomposed_hand<meld_t> &s) const;
    decomposed_hand<meld_t> _extract_combo(const decomposed_hand<meld_t> &s, int pos, int type) const;
    void _get_nbs(const decomposed_hand<meld_t> &s, std::vector<decomposed_hand<meld_t>> &nbs) const;

    /*return 0: cannot, 1: adj e.g.{6,7}, 2: gap e.g.{3,5}*/
    int can_semi_straight(const hand_t &hand, int index) const;
    bool can_semi_triple(const hand_t &hand, int index) const;

    bool is_leaf(const decomposed_hand<semi_meld_t> &s) const;
    decomposed_hand<semi_meld_t> _extract_combo(const decomposed_hand<semi_meld_t> &s, int pos, int type) const;
    void _get_nbs(const decomposed_hand<semi_meld_t> &s, std::vector<decomposed_hand<semi_meld_t>> &nbs) const;
};

template <typename T>
std::vector<decomposed_hand<T>>
Hand_Evaluator::decomp_hand(const hand_t &hand) const
{
    std::vector<decomposed_hand<T>> res;

    using search_state = decomposed_hand<T>;

    std::vector<search_state> leafnodes;
    std::vector<search_state> openlist;
    search_state start;
    start.remain_hand = hand;
    openlist.push_back(start);
    while (openlist.size() > 0)
    {
        search_state cur_state = openlist.front();
        // std::cout << "cur_state:\n";
        // std::cout << cur_state.to_str() << "\n";
        openlist.erase(openlist.begin());

        if (is_leaf(cur_state))
        {
            // std::cout << "is leaf\n";
            leafnodes.push_back(cur_state);
            continue;
        }
        std::vector<search_state> nbs;
        _get_nbs(cur_state, nbs);
        // std::cout << "nbs:\n";
        for (auto &item : nbs)
        {
            // std::cout << item.to_str() << "\n";
            openlist.push_back(item);
        }
    }

    std::sort(leafnodes.begin(), leafnodes.end(), [](auto l, auto r)
              { return l.remain_hand.hand_cnt < r.remain_hand.hand_cnt; });

    res = leafnodes;
    while (res.size() > 1 && res.back().remain_hand.hand_cnt > res.front().remain_hand.hand_cnt)
        res.pop_back();

    return res;
}
