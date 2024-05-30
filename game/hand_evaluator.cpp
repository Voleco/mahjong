#include "hand_evaluator.h"

int Step2Win(const hand_t &hand, const Hand_Evaluator *he_ptr)
{
    // std::vector<hand_t> path;
    // bool find_sol = BeamSearch<hand_t, Hand_Evaluator>(
    //     hand, path, he_ptr, 10, 5);

    return 0;
}

std::vector<std::pair<hand_t, hand_t>> Hand_Evaluator::decomp_hand(
    const hand_t &hand) const
{
    std::vector<std::pair<hand_t, hand_t>> res;

    struct search_state
    {
        hand_t extracted_cards;
        hand_t hand;
    };

    auto lam_isleaf = [&](const search_state &s)
    {
        for (int i = 0; i < (s.hand.cards.size()); i++)
        {
            if (can_straight(s.hand, i) == true ||
                can_triple(s.hand, i) == true)
                return false;
        }
        return true;
    };

    /*tpye == 1 triple; type == 2 straight*/
    auto lam_extract = [](const search_state &s, int pos, int type)
    {
        search_state tmp = s;

        if (type == 1)
        {
            tmp.extracted_cards.cards[pos] += 3;
            tmp.hand.cards[pos] -= 3;
        }
        else if (type == 2)
        {
            tmp.extracted_cards.cards[pos]++;
            tmp.extracted_cards.cards[pos + 1]++;
            tmp.extracted_cards.cards[pos + 2]++;
            tmp.hand.cards[pos]--;
            tmp.hand.cards[pos + 1]--;
            tmp.hand.cards[pos + 2]--;
        }

        tmp.hand.hand_cnt -= 3;
        return tmp;
    };

    auto lam_getnbs = [&](const search_state &s, std::vector<search_state> &nbs)
    {
        for (int i = 0; i < (s.hand.cards.size()); i++)
        {
            if (s.hand.cards[i] == 4)
            {
                nbs.push_back(lam_extract(s, i, 1));
                return;
            }
            else if (s.hand.cards[i] == 3)
            {
                nbs.push_back(lam_extract(s, i, 1));
                if (can_straight(s.hand, i))
                    nbs.push_back(lam_extract(s, i, 2));
                return;
            }
            else
            {
                if (can_straight(s.hand, i))
                {
                    nbs.push_back(lam_extract(s, i, 2));
                    if (can_straight(s.hand, i + 1) && s.hand.cards[i + 1] == 1)
                        nbs.push_back(lam_extract(s, i + 1, 2));
                    if (can_straight(s.hand, i + 2) && s.hand.cards[i + 2] == 1)
                        nbs.push_back(lam_extract(s, i + 2, 2));
                    return;
                }
            }
        }
    };

    std::vector<search_state> leafnodes;

    std::vector<search_state> openlist;
    search_state start;
    start.extracted_cards = init_hand();
    start.hand = hand;
    openlist.push_back(start);
    while (openlist.size() > 0)
    {
        search_state cur_state = openlist.front();
        // std::cout << "cur_state:\n";
        // print_hand(cur_state.hand);
        openlist.erase(openlist.begin());

        if (lam_isleaf(cur_state))
        {
            // std::cout << "is leaf\n";
            leafnodes.push_back(cur_state);
            continue;
        }
        std::vector<search_state> nbs;
        lam_getnbs(cur_state, nbs);
        // std::cout << "nbs:\n";
        for (auto &item : nbs)
        {
            // print_hand(item.hand);
            openlist.push_back(item);
        }
    }

    std::sort(leafnodes.begin(), leafnodes.end(), [](auto l, auto r)
              { return l.hand.hand_cnt < r.hand.hand_cnt; });

    for (auto item : leafnodes)
        res.push_back(std::make_pair(item.extracted_cards, item.hand));
    while (res.size() > 1 && res.back().second.hand_cnt > res.front().second.hand_cnt)
        res.pop_back();

    return res;
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

int Hand_Evaluator::HCost(const hand_t &hand) const
{
    /*h = 9 — 2*m — d + c — q
     m是面子数, d是搭子数, c是超载数, q是雀头
     c为超载数，当m+d≤5时，c=0，当m+d＞5时，c=m+d—5；
     q为雀头函数，m+d≤4时，q＝1，m+d＞4时，有雀头（对子）时，q=1，没有雀头时，q=0
     */

    return 0;
}

void Hand_Evaluator::GetNeighbors(const hand_t &hand, std::vector<hand_t> &nbs) const
{
}