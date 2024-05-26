#pragma once

#include <vector>
#include <algorithm>

#include "cards.h"
#include "../utils/util_helpers.h"

// using hand_t = std::vector<cardcnt>;

hand_t cards_to_hand(const std::vector<card_t> &hand);

void print_hand(const hand_t &hand);

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

private:
    /// @brief decomposite hand into melds
    /// @param hand can be 14, 11, 8, 5, 2 cards
    /// @param hand_cnt num cards in hand
    /// @param meld_cnt num melds to get
    /// @return vector of (melds, single cards)
    std::vector<std::pair<hand_t, hand_t>> decomp_hand(const hand_t &hand, int hand_cnt, int meld_cnt) const;

    bool all_melds(const hand_t &cards) const;

    std::vector<hand_t> extract_meld(const hand_t &cards, int index) const;

    bool can_straight(const hand_t &hand, int index) const;
    bool can_triple(const hand_t &hand, int index) const;
};

/// @brief steps to win. 0: win, 1: draw, 2: one to draw, 3: two to draw...
/// @param hand 14 cards
/// @return steps
int Step2Win(const hand_t &hand, const Hand_Evaluator *he_ptr);
