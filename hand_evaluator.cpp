#include "hand_evaluator.h"

bool Hand_Evaluator::has_4combo(const compact_hand_t &cards_12)
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
        for (auto &item : nbs)
            openlist.push_back(state(item, cur_state.combo2go - 1));
    }

    return false;
}

size_t Hand_Evaluator::first_straght(const compact_hand_t &hand)
{
    size_t i = 0;
    for (i = 0; i < hand.size(); i++)
        if (hand[i].cardcnt > 0)
            break;
    /*category not enough*/
    if (i >= hand.size() - 2)
        return hand.size();

    if (hand[i + 1].cardname == hand[i].cardname + 1 &&
        hand[i + 1].cardcnt > 0 &&
        hand[i + 2].cardname == hand[i + 1].cardname + 1 &&
        hand[i + 2].cardcnt > 0)
        return i;

    return hand.size();
}

size_t Hand_Evaluator::fisrt_triple(const compact_hand_t &hand)
{
    size_t i = 0;
    for (i = 0; i < hand.size(); i++)
        if (hand[i].cardcnt > 0)
            break;

    if (hand[i].cardcnt >= 3)
        return i;

    /*card not enough*/
    return hand.size();
}

std::vector<compact_hand_t> Hand_Evaluator::extract_combo(compact_hand_t &N_cards)
{
    std::vector<compact_hand_t> result;
    int pos1 = first_straght(N_cards);
    if (pos1 < N_cards.size())
    {
        result.push_back(N_cards);
        result.back()[pos1].cardcnt--;
        result.back()[pos1 + 1].cardcnt--;
        result.back()[pos1 + 2].cardcnt--;
    }

    int pos2 = fisrt_triple(N_cards);
    if (pos2 < N_cards.size())
    {
        result.push_back(N_cards);
        result.back()[pos2].cardcnt -= 3;
    }

    return result;
}

bool Hand_Evaluator::is_Win(const hand_t &hand)
{
    // hand_t cur_hand = hand;
    // std::sort(cur_hand.begin(), cur_hand.end());
    std::vector<uint8_t> cards_cnt(CARD_MAX_VALUE, 0);
    for (auto &item : hand)
        cards_cnt[item]++;

    std::vector<grouped_card_t> card_group;
    for (unsigned int i = 0; i < CARD_MAX_VALUE; i++)
        if (cards_cnt[i] > 0)
            card_group.push_back(grouped_card_t(card_t(i), cards_cnt[i]));
    /*already sort*/

    /*sp case 1: 7 pairs*/
    bool all_even = true;
    for (auto item : card_group)
        if (item.cardcnt % 2 != 0)
        {
            all_even = false;
            break;
        }

    if (all_even)
        return true;

    /*TODO sp case 2: 13yao*/

    /*regular case*/

    // atama_idxs
    std::vector<int> atama_idxs;
    for (int i = 0; i < int(card_group.size()); i++)
        if (card_group[i].cardcnt >= 2)
            atama_idxs.push_back(i);

    for (auto idx : atama_idxs)
    {
        // std::cout << "checking atama: " << int(card_group[idx].cardname) << "\n";
        std::vector<grouped_card_t> candi_12cards = card_group;
        candi_12cards[idx].cardcnt -= 2;

        if (has_4combo(candi_12cards) == true)
        {
            return true;
        }
    }

    return false;
}