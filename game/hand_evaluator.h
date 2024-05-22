#pragma once

#include <vector>
#include <algorithm>

#include "cards.h"
#include "../utils/util_helpers.h"

using hand_t = std::vector<card_t>;
using subhand_t = std::vector<card_t>;
using compact_hand_t = std::vector<cardcnt>;

compact_hand_t to_compact_hand(const hand_t &hand);

class Hand_Evaluator
{
public:
    Hand_Evaluator() {}
    ~Hand_Evaluator() {}
    bool is_Win(const compact_hand_t &hand) const;
    int HCost(const compact_hand_t &hand) const;
    void GetNeighbors(const compact_hand_t &hand, std::vector<compact_hand_t> &nbs) const;

private:
    bool has_4combo(const compact_hand_t &cards_12) const;
    std::vector<compact_hand_t> extract_combo(const compact_hand_t &N_cards) const;

    std::vector<compact_hand_t> extract_meld(const compact_hand_t &cards, int index) const;

    /* if fisrt non 0 cnt card can form staright, return pos*/
    size_t first_non0(const compact_hand_t &hand) const;

    /* if fisrt non 0 cnt card can form staright, return pos*/
    size_t first_straght(const compact_hand_t &hand) const;

    /* if fisrt non 0 cnt card can form triple, return pos*/
    size_t fisrt_triple(const compact_hand_t &hand) const;

    bool can_straight(const compact_hand_t &hand, int index) const;
    bool can_triple(const compact_hand_t &hand, int index) const;
};

/// @brief steps to win. 0: win, 1: draw, 2: one to draw, 3: two to draw...
/// @param hand 14 cards
/// @return steps
int Step2Win(const compact_hand_t &hand, const Hand_Evaluator *he_ptr);
