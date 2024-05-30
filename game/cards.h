#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

using card_t = uint8_t;

/*
万子牌 = 1~9
筒子牌 = 11~19
索子牌 = 21~29

东 南 西 北 = 52 55 58 61
白 发 中 =  64 67 70
春、夏、秋、冬	= 73 76 79 82	　
梅、兰、竹、菊	= 85 88 91 94	　
*/

using cardcnt = uint8_t;
const card_t MAX_CARD_VALUE = 30;

struct hand_t
{
    std::vector<cardcnt> cards;
    int hand_cnt;
};

inline void print_hand(const hand_t &hand)
{
    for (int i = 0; i < int(hand.cards.size()); i++)
        if (hand.cards[i] > 0)
            std::cout << "(" << i << ", " << int(hand.cards[i]) << ") ";
    std::cout << "\n";
}

inline hand_t init_hand()
{
    hand_t res;
    res.hand_cnt = 0;
    res.cards = std::vector<cardcnt>(MAX_CARD_VALUE, 0);
    return res;
}

inline hand_t cards_to_hand(const std::vector<card_t> &cards)
{
    hand_t cur_hand = init_hand();
    cur_hand.hand_cnt = cards.size();
    for (auto &item : cards)
        cur_hand.cards[item]++;
    return cur_hand;
}

// const card_t CARD_MAX_VALUE = 100;

// struct grouped_card_t
// {
//     grouped_card_t(card_t _name, uint8_t _cnt) : cardname(_name), cardcnt(_cnt) {}
//     card_t cardname;
//     uint8_t cardcnt;
// };

/*game type: 108 card game, 144, 152*/
