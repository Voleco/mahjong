#include "hand_evaluator.h"


bool Hand_Evaluator::is_leaf(const decomposed_hand<meld_t> &s) const
{
    auto &s_hand = s.remain_hand;
    for (int i = 0; i < (s_hand.cards.size()); i++)
    {
        if (can_straight(s_hand, i) == true ||
            can_triple(s_hand, i) == true)
            return false;
    }
    return true;
}

bool Hand_Evaluator::is_leaf(const decomposed_hand<semi_meld_t> &s) const
{
    auto &s_hand = s.remain_hand;
    for (int i = 0; i < (s_hand.cards.size()); i++)
    {
        if (can_semi_straight(s_hand, i) > 0 ||
            can_semi_triple(s_hand, i) == true)
            return false;
    }
    return true;
}

/*tpye == 1 triple; type == 2 straight*/
decomposed_hand<meld_t> Hand_Evaluator::_extract_combo(
    const decomposed_hand<meld_t> &s, int pos, int type) const
{
    decomposed_hand<meld_t> tmp = s;

    if (type == 1)
    {
        tmp.combos.push_back(
            meld_t({card_t(pos), card_t(pos), card_t(pos)}));
        tmp.remain_hand.cards[pos] -= 3;
    }
    else if (type == 2)
    {
        tmp.combos.push_back(
            meld_t({card_t(pos), card_t(pos + 1), card_t(pos + 2)}));
        tmp.remain_hand.cards[pos]--;
        tmp.remain_hand.cards[pos + 1]--;
        tmp.remain_hand.cards[pos + 2]--;
    }

    tmp.remain_hand.hand_cnt -= 3;
    return tmp;
}

void Hand_Evaluator::_get_nbs(const decomposed_hand<meld_t> &s,
                              std::vector<decomposed_hand<meld_t>> &nbs) const
{
    for (int i = 0; i < (s.remain_hand.cards.size()); i++)
    {
        if (s.remain_hand.cards[i] == 4)
        {
            nbs.push_back(_extract_combo(s, i, 1));
            return;
        }
        else if (s.remain_hand.cards[i] == 3)
        {
            nbs.push_back(_extract_combo(s, i, 1));
            if (can_straight(s.remain_hand, i))
                nbs.push_back(_extract_combo(s, i, 2));
            return;
        }
        else
        {
            auto item_unique = [&](int idx)
            {
                return s.remain_hand.cards[idx] == 1;
            };
            if (can_straight(s.remain_hand, i))
            {
                nbs.push_back(_extract_combo(s, i, 2));
                if (can_straight(s.remain_hand, i + 1) &&
                    (item_unique(i + 1) || item_unique(i + 2)))
                    nbs.push_back(_extract_combo(s, i + 1, 2));
                if (can_straight(s.remain_hand, i + 2) &&
                    item_unique(i + 2))
                    nbs.push_back(_extract_combo(s, i + 2, 2));
                if (s.remain_hand.cards[i + 1] == 3)
                    nbs.push_back(_extract_combo(s, i + 1, 1));
                if (s.remain_hand.cards[i + 2] == 3)
                    nbs.push_back(_extract_combo(s, i + 2, 1));
                return;
            }
        }
    }
}

decomposed_hand<semi_meld_t> Hand_Evaluator::_extract_combo(
    const decomposed_hand<semi_meld_t> &s, int pos, int type) const
{
    decomposed_hand<semi_meld_t> tmp = s;

    if (type == 1)
    {
        tmp.combos.push_back(
            semi_meld_t({card_t(pos), card_t(pos)}));
        tmp.remain_hand.cards[pos] -= 2;
    }
    else if (type == 2)
    {
        tmp.combos.push_back(
            semi_meld_t({card_t(pos), card_t(pos + 1)}));
        tmp.remain_hand.cards[pos]--;
        tmp.remain_hand.cards[pos + 1]--;
    }
    else if (type == 3)
    {
        tmp.combos.push_back(
            semi_meld_t({card_t(pos), card_t(pos + 2)}));
        tmp.remain_hand.cards[pos]--;
        tmp.remain_hand.cards[pos + 2]--;
    }

    tmp.remain_hand.hand_cnt -= 2;
    return tmp;
}

void Hand_Evaluator::_get_nbs(const decomposed_hand<semi_meld_t> &s,
                              std::vector<decomposed_hand<semi_meld_t>> &nbs) const
{
    for (int i = 0; i < (s.remain_hand.cards.size()); i++)
    {
        if (s.remain_hand.cards[i] == 2)
        {
            nbs.push_back(_extract_combo(s, i, 1));
            int type = can_semi_straight(s.remain_hand, i);
            if (type > 0)
                nbs.push_back(_extract_combo(s, i, type + 1));
            return;
        }
        else
        {
            int type = can_semi_straight(s.remain_hand, i);
            if (type > 0)
            {
                nbs.push_back(_extract_combo(s, i, type + 1));
                int next_type = can_semi_straight(s.remain_hand, i + type);
                if (next_type > 0 && s.remain_hand.cards[i + type] == 1)
                    nbs.push_back(_extract_combo(s, i + type, next_type + 1));

                if (s.remain_hand.cards[i + type] == 2)
                    nbs.push_back(_extract_combo(s, i + type, 1));

                return;
            }
        }
    }
}

bool Hand_Evaluator::all_melds(const hand_t &cards) const
{
    if (cards.hand_cnt == 0)
        return true;
    struct state
    {
        state(const hand_t &hand_, uint8_t combo2go_) : hand(hand_), combo2go(combo2go_) {}
        hand_t hand;
        uint8_t combo2go;
    };

    std::vector<state> openlist;
    openlist.push_back(state(cards, cards.hand_cnt / 3));
    while (openlist.size() > 0)
    {
        auto cur_state = openlist.back();
        // std::cout << "cur_state: " << int(cur_state.combo2go) << "\n";
        // print_hand(cur_state.hand);
        openlist.pop_back();
        if (cur_state.combo2go == 0)
            return true;

        int idx = first_non0<cardcnt>(cur_state.hand.cards);
        auto nbs = extract_meld(cur_state.hand, idx);
        // std::cout << "idx: " << idx << " nbs:\n ";
        // for (auto &item : nbs)
        //     print_hand(item);

        for (auto &item : nbs)
            openlist.push_back(state(item, cur_state.combo2go - 1));
    }

    return false;
}

bool Hand_Evaluator::can_straight(const hand_t &hand, int i) const
{
    if (i / 10 > 2 || i % 10 > 7)
        return false;

    if (hand.cards[i] > 0 && hand.cards[i + 1] > 0 && hand.cards[i + 2] > 0)
        return true;

    return false;
}

bool Hand_Evaluator::can_triple(const hand_t &hand, int i) const
{
    if (i >= hand.cards.size())
        return false;
    if (hand.cards[i] >= 3)
        return true;
    return false;
}

int Hand_Evaluator::can_semi_straight(const hand_t &hand, int i) const
{
    if (i / 10 > 2 || i % 10 > 8)
        return 0;

    if (i % 10 == 8)
    {
        if (hand.cards[i] > 0 && hand.cards[i + 1] > 0)
            return 1;
    }
    else
    {
        if (hand.cards[i] > 0 && hand.cards[i + 1] > 0)
            return 1;
        if (hand.cards[i] > 0 && hand.cards[i + 2] > 0)
            return 2;
    }
    return 0;
}

bool Hand_Evaluator::can_semi_triple(const hand_t &hand, int i) const
{
    if (i >= hand.cards.size())
        return false;
    if (hand.cards[i] >= 2)
        return true;
    return false;
}

std::vector<hand_t> Hand_Evaluator::extract_meld(const hand_t &cards, int index) const
{
    std::vector<hand_t> result;

    if (can_straight(cards, index))
    {
        result.push_back(cards);
        result.back().hand_cnt -= 3;
        result.back().cards[index]--;
        result.back().cards[index + 1]--;
        result.back().cards[index + 2]--;
    }

    if (can_triple(cards, index))
    {
        result.push_back(cards);
        result.back().hand_cnt -= 3;
        result.back().cards[index] -= 3;
    }

    return result;
}

bool Hand_Evaluator::is_Win(const hand_t &hand) const
{
    /*sp case 1: 7 pairs*/
    if (hand.hand_cnt == 14)
    {
        bool all_even = true;
        for (auto item : hand.cards)
            if (item % 2 != 0)
            {
                all_even = false;
                break;
            }

        if (all_even)
            return true;
    }

    /*TODO sp case 2: 13yao*/

    /*regular case*/
    if (hand.hand_cnt != 14 && hand.hand_cnt != 11 && hand.hand_cnt != 8 &&
        hand.hand_cnt != 5 && hand.hand_cnt != 2)
        return false;

    // atama_idxs
    std::vector<int> atama_candidates;
    for (int i = 0; i < MAX_CARD_VALUE; i++)
        if (hand.cards[i] >= 2)
            atama_candidates.push_back(i);

    // int
    for (auto ac : atama_candidates)
    {
        // std::cout << "checking atama: " << int(card_group[idx].cardname) << "\n";
        hand_t candi_melds = hand;
        candi_melds.hand_cnt -= 2;
        candi_melds.cards[ac] -= 2;

        if (all_melds(candi_melds) == true)
        {
            return true;
        }
    }

    return false;
}

std::vector<full_DH> Hand_Evaluator::full_decomp_hand(const hand_t &hand) const
{
    std::vector<full_DH> dcp;

    auto part1 = decomp_hand<meld_t>(hand);

    for (int i = 0; i < int(part1.size()); i++)
    {
        auto part2 = decomp_hand<semi_meld_t>(part1[i].remain_hand);
        if (part2.size() == 0)
        {
            full_DH tmp;
            tmp.melds = part1[i].combos;
            tmp.single_cards = part1[i].remain_hand.to_card();
            tmp.has_atama = 0;
            dcp.push_back(tmp);
        }
        for (int j = 0; j < int(part2.size()); j++)
        {
            auto ct_atama = [](const std::vector<semi_meld_t> &sms)
            {
                for (auto item : sms)
                    if (item.s1 == item.s2)
                        return 1;
                return 0;
            };

            full_DH tmp;
            tmp.melds = part1[i].combos;
            tmp.semi_melds = part2[j].combos;
            tmp.single_cards = part2[j].remain_hand.to_card();
            tmp.has_atama = ct_atama(tmp.semi_melds);
            dcp.push_back(tmp);
        }
    }
    auto cmp_fdh = [](const full_DH &l, const full_DH &r)
    {
        /*has atama is better*/
        if (l.single_cards.size() == l.single_cards.size())
            return l.has_atama > r.has_atama;
        return l.single_cards.size() < r.single_cards.size();
    };
    std::sort(dcp.begin(), dcp.end(), cmp_fdh);

    return dcp;
}

int Hand_Evaluator::HCost(const hand_t &hand) const
{
    /*h = 9 — 2*m — d + c — q
     m是面子数, d是搭子数, c是超载数, q是雀头
     c为超载数，当m+d≤5时，c=0，当m+d＞5时，c=m+d—5；
     q为雀头函数，m+d≤4时，q＝1，m+d＞4时，有雀头（对子）时，q=1，没有雀头时，q=0
     */
    // if (is_Win(hand))
    //     return 0;

    std::vector<full_DH> dcp = full_decomp_hand(hand);
    full_DH &best_dch = dcp.front();
    int m = best_dch.melds.size();
    int d = best_dch.semi_melds.size();
    int c = std::max(m + d - 5, 0);

    int q;
    if (m + d <= 4)
        q = 1;
    else
        q = best_dch.has_atama;

    int h = 9 - 2 * m - d + c - q;

    return h;
}

void Hand_Evaluator::GetNeighbors(const hand_t &hand, std::vector<hand_t> &nbs) const
{
}