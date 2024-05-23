#include "hand_evaluator.h"

compact_hand_t to_compact_hand(const hand_t &hand)
{
    std::vector<cardcnt> init_hand(MAX_CARD_VALUE, 0);
    for (auto &item : hand)
        init_hand[item]++;
    return init_hand;
}

int Step2Win(const compact_hand_t &hand, const Hand_Evaluator *he_ptr)
{
    std::vector<compact_hand_t> path;
    bool find_sol = BeamSearch<compact_hand_t, Hand_Evaluator>(
        hand, path, he_ptr, 10, 5);

    return 0;
}

bool Hand_Evaluator::has_4combo(const compact_hand_t &cards_12) const
{
    struct state
    {
        state(const compact_hand_t &hand_, uint8_t combo2go_) : hand(hand_), combo2go(combo2go_) {}
        compact_hand_t hand;
        uint8_t combo2go;
    };

    std::vector<state> openlist;
    openlist.push_back(state(cards_12, 4));
    while (openlist.size() > 0)
    {
        auto cur_state = openlist.back();
        // std::cout << "cur_state: " << int(cur_state.combo2go) << "\n";
        // for (auto s : cur_state.hand)
        //     std::cout << int(s.cardname) << ", " << int(s.cardcnt) << "\n";
        openlist.pop_back();
        if (cur_state.combo2go == 0)
            return true;

        auto nbs = extract_combo(cur_state.hand);
        // int idx = first_non0(cur_state.hand);
        // // std::cout << "idx: " << idx;
        // auto nbs = extract_meld(cur_state.hand, idx);

        for (auto &item : nbs)
            openlist.push_back(state(item, cur_state.combo2go - 1));
    }

    return false;
}

bool Hand_Evaluator::can_straight(const compact_hand_t &hand, int i) const
{
    if (i >= hand.size() - 2)
        return false;

    if (hand[i + 1] > 0 &&
        hand[i + 2] > 0)
        return true;

    return false;
}

bool Hand_Evaluator::can_triple(const compact_hand_t &hand, int i) const
{
    if (i >= hand.size())
        return false;
    if (hand[i] >= 3)
        return true;
    return false;
}

size_t Hand_Evaluator::first_straght(const compact_hand_t &hand) const
{
    size_t i = 0;
    for (i = 0; i < hand.size(); i++)
        if (hand[i] > 0)
            break;
    /*category not enough*/
    if (can_straight(hand, i))
        return i;

    return hand.size();
}

size_t Hand_Evaluator::fisrt_triple(const compact_hand_t &hand) const
{
    size_t i = 0;
    for (i = 0; i < hand.size(); i++)
        if (hand[i] > 0)
            break;

    if (can_triple(hand, i))
        return i;

    /*card not enough*/
    return hand.size();
}

/* if fisrt non 0 cnt card can form staright, return pos*/
size_t Hand_Evaluator::first_non0(const compact_hand_t &hand) const
{
    size_t i = 0;
    for (i = 0; i < hand.size(); i++)
        if (hand[i] > 0)
            break;
    return hand.size();
}

std::vector<compact_hand_t> Hand_Evaluator::extract_meld(const compact_hand_t &cards, int index) const
{
    std::vector<compact_hand_t> result;

    if (can_straight(cards, index))
    {
        result.push_back(cards);
        result.back()[index]--;
        result.back()[index + 1]--;
        result.back()[index + 2]--;
    }

    if (can_triple(cards, index))
    {
        result.push_back(cards);
        result.back()[index] -= 3;
    }

    return result;
}

std::vector<compact_hand_t> Hand_Evaluator::extract_combo(const compact_hand_t &N_cards) const
{
    std::vector<compact_hand_t> result;
    int pos1 = first_straght(N_cards);
    if (pos1 < N_cards.size())
    {
        result.push_back(N_cards);
        result.back()[pos1]--;
        result.back()[pos1 + 1]--;
        result.back()[pos1 + 2]--;
    }

    int pos2 = fisrt_triple(N_cards);
    if (pos2 < N_cards.size())
    {
        result.push_back(N_cards);
        result.back()[pos2] -= 3;
    }

    return result;
}

bool Hand_Evaluator::is_Win(const compact_hand_t &hand) const
{
    // hand_t cur_hand = hand;
    // std::sort(cur_hand.begin(), cur_hand.end());

    /*already sort*/

    /*sp case 1: 7 pairs*/
    bool all_even = true;
    for (auto item : hand)
        if (item % 2 != 0)
        {
            all_even = false;
            break;
        }

    if (all_even)
        return true;

    /*TODO sp case 2: 13yao*/

    /*regular case*/

    // atama_idxs
    std::vector<int> atama_candidates;
    for (int i = 0; i < MAX_CARD_VALUE; i++)
        if (hand[i] >= 2)
            atama_candidates.push_back(i);

    for (auto ac : atama_candidates)
    {
        // std::cout << "checking atama: " << int(card_group[idx].cardname) << "\n";
        std::vector<cardcnt> candi_12cards = hand;
        candi_12cards[ac] -= 2;

        if (has_4combo(candi_12cards) == true)
        {
            return true;
        }
    }

    return false;
}

int Hand_Evaluator::HCost(const compact_hand_t &hand) const
{
    /*h = 9 — 2*m — d + c — q
     m是面子数, d是搭子数, c是超载数, q是雀头
     c为超载数，当m+d≤5时，c=0，当m+d＞5时，c=m+d—5；
     q为雀头函数，m+d≤4时，q＝1，m+d＞4时，有雀头（对子）时，q=1，没有雀头时，q=0
     */

    return 0;
}

void Hand_Evaluator::GetNeighbors(const compact_hand_t &hand, std::vector<compact_hand_t> &nbs) const
{
}