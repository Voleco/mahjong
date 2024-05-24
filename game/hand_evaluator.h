#pragma once

#include <vector>
#include <algorithm>

#include "cards.h"
#include "../utils/util_helpers.h"

using hand_t = std::vector<cardcnt>;

hand_t cards_to_hand(const std::vector<card_t> &hand);

void print_hand(const hand_t& hand);

class Hand_Evaluator
{
public:
    Hand_Evaluator() {}
    ~Hand_Evaluator() {}
    bool is_Win(const hand_t &hand) const;
    int HCost(const hand_t &hand) const;
    void GetNeighbors(const hand_t &hand, std::vector<hand_t> &nbs) const;

private:
    bool has_4melds(const hand_t &cards_12) const;

    std::vector<hand_t> extract_meld(const hand_t &cards, int index) const;

    /*fisrt non 0 cnt card */
    size_t first_non0(const hand_t &hand) const;

    bool can_straight(const hand_t &hand, int index) const;
    bool can_triple(const hand_t &hand, int index) const;
};

/// @brief steps to win. 0: win, 1: draw, 2: one to draw, 3: two to draw...
/// @param hand 14 cards
/// @return steps
int Step2Win(const hand_t &hand, const Hand_Evaluator *he_ptr);
