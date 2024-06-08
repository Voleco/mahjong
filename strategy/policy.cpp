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

int Policy::Get_Score(const hand_t &hand, card_t kicked, score_mode mode, int depth) const
{
    int score;
    // Hand_Evaluator he;
    // // Deck deck;
    // std::vector<sub> good_actions;
    // hand_t cur_hand = hand;
    // int init_h = he.HCost(cur_hand);
    // for (auto act : actions)
    // {
    //     apply_action(cur_hand, act);
    //     int cur_h = he.HCost(cur_hand);
    //     if (cur_h < init_h)
    //         good_actions.push_back(act);
    //     undo_action(cur_hand, act);
    // }

    // // std::cout << "good actions:\n";
    // // for (auto a : good_actions)
    // //     std::cout << "kick " << std::to_string(a.out) << " and get "
    // //               << std::to_string(a.in) << "\n";

    // for (auto a : good_actions)
    // {
    //     if (res.size() == 0 || res.back().kick_card != a.out)
    //     {
    //         Choice neo_choice;
    //         neo_choice.kick_card = a.out;
    //         neo_choice.improving_cards.push_back(multi_card{a.in, resd.Get_CardCnt(a.in)});
    //         res.push_back(neo_choice);
    //     }
    //     else
    //     {
    //         res.back().improving_cards.push_back(multi_card{a.in, resd.Get_CardCnt(a.in)});
    //     }
    // }

    // // std::cout << "good choices:\n";

    return score;
}

std::vector<Choice> Policy::Get_Improving_Cards(const hand_t &hand, const ResDeck &resdeck) const
{
    std::vector<Choice> res;

    Hand_Evaluator he;

    std::vector<sub> actions;
    get_actions(hand, resdeck, actions);
    
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
            neo_choice.improving_cards.push_back(multi_card{a.in, resdeck.Get_CardCnt(a.in)});
            res.push_back(neo_choice);
        }
        else
        {
            res.back().improving_cards.push_back(multi_card{a.in, resdeck.Get_CardCnt(a.in)});
        }
    }

    // std::cout << "good choices:\n";

    return res;
}