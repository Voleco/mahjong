#pragma once

#include "../game/cards.h"
#include "../game/deck.hpp"
#include "../game/hand_evaluator.h"

struct multi_card
{
    card_t rank;
    cardcnt cnt;
};

struct sub
{
    card_t out;
    card_t in;
};

inline void get_actions(const hand_t &hand, const ResDeck &resdeck, std::vector<sub> &actions)
{
    actions.clear();

    std::vector<card_t> cur_kinds;
    for (int i = 0; i < int(hand.cards.size()); i++)
        if (hand.cards[i] > 0)
            cur_kinds.push_back(card_t(i));

    std::vector<card_t> possible_ins = resdeck.Get_Possible_Cards();

    for (auto c : cur_kinds)
        for (auto i : possible_ins)
            if (c != i)
                actions.push_back(sub({c, i}));
}

// struct treenode
// {
//     int depth;
//     int parent_idx;

//     Choice c;
// };

struct Choice
{
    std::string to_str() const;
    card_t kick_card;                        /*打出的牌*/
    std::vector<multi_card> improving_cards; /*进张*/
};

enum class score_mode
{
    no_bias = 0,
    prefer_pong = 1,

    other = 9
};

class Policy
{
public:
    Policy() {}
    ~Policy() {}

    // std::vector<card_t>
    /* get cards, kicking which could improve hand */
    std::vector<Choice> Get_Improving_Cards(const hand_t &hand, const ResDeck &resdeck) const;

    std::unordered_map<card_t, uint64_t> Get_Score_DFS(
        const hand_t &start_hand,
        ResDeck start_deck,
        const Hand_Evaluator &he,
        int dep_limit) const;
    /**/
    std::vector<Choice> Get_Good_Paths(const hand_t &hand) const;

private:
    uint64_t dfs_improving(
        hand_t &hand,
        ResDeck &deck,
        const Hand_Evaluator &he,
        int depth,
        int dep_limit,
        int initH // 初始时的HCost
    ) const;
};