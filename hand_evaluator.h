#pragma once

#include <vector>
#include <algorithm>

#include "cards.h"
#include "util_helpers.h"

using hand_t = std::vector<card_t>;
using subhand_t = std::vector<card_t>;
using compact_hand_t = std::vector<grouped_card_t>;

class Hand_Evaluator
{
public:
    Hand_Evaluator() {}
    ~Hand_Evaluator() {}
    bool is_Win(const hand_t &hand);

private:
    bool has_4combo(const compact_hand_t &cards_12);
    std::vector<compact_hand_t> extract_combo(compact_hand_t &N_cards);

    /* if fisrt non 0 cnt card can form staright, return pos*/
    size_t first_straght(const compact_hand_t &hand);

    /* if fisrt non 0 cnt card can form triple, return pos*/
    size_t fisrt_triple(const compact_hand_t &hand);
};