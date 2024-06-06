#include "policy.hpp"

std::string Choice::to_str() const
{
    std::string res = "";
    res += "kick " + get_cardName(kick_card) + " and get: {";
    for (auto cb : improving_cards)
        res += "(" + get_cardName(cb.rank) + " x " + std::to_string(cb.cnt) + ") ";
    res += "}";
    return res;
}

void apply_action(hand_t &hand, sub act)
{
    hand.cards[act.out]--;
    hand.cards[act.in]++;
}

void undo_action(hand_t &hand, sub act)
{
    hand.cards[act.out]++;
    hand.cards[act.in]--;
}

std::vector<Choice> Policy::Get_Improving_Cards(const hand_t &hand) const
{
    std::vector<Choice> res;

    Hand_Evaluator he;
    // Deck deck;
    std::vector<card_t> cur_kinds;
    for (int i = 0; i < int(hand.cards.size()); i++)
        if (hand.cards[i] > 0)
            cur_kinds.push_back(card_t(i));

    ResDeck resd(hand);
    std::vector<card_t> possible_ins = resd.Get_Possible_Cards();

    std::vector<sub> actions;
    for (auto c : cur_kinds)
        for (auto i : possible_ins)
            if (c != i)
                actions.push_back(sub({c, i}));

    std::vector<sub> good_actions;
    hand_t cur_hand = hand;
    int init_h = he.HCost(cur_hand);
    for (auto act : actions)
    {
        apply_action(cur_hand, act);
        int cur_h = he.HCost(cur_hand);
        if (cur_h < init_h)
            good_actions.push_back(act);
        undo_action(cur_hand, act);
    }

    // std::cout << "good actions:\n";
    // for (auto a : good_actions)
    //     std::cout << "kick " << std::to_string(a.out) << " and get "
    //               << std::to_string(a.in) << "\n";

    for (auto a : good_actions)
    {
        if (res.size() == 0 || res.back().kick_card != a.out)
        {
            Choice neo_choice;
            neo_choice.kick_card = a.out;
            neo_choice.improving_cards.push_back(multi_card{a.in, resd.Get_CardCnt(a.in)});
            res.push_back(neo_choice);
        }
        else
        {
            res.back().improving_cards.push_back(multi_card{a.in, resd.Get_CardCnt(a.in)});
        }
    }

    // std::cout << "good choices:\n";

    return res;
}