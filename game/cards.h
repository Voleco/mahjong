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

struct meld_t
{
    std::string to_str();
    card_t c1;
    card_t c2;
    card_t c3;
};

struct semi_meld_t
{
    std::string to_str();
    card_t s1;
    card_t s2;
};

struct hand_t
{
    hand_t();
    hand_t(const std::vector<card_t> &_cards);
    void init();
    std::string to_str();

    std::vector<cardcnt> cards;
    int hand_cnt;
};

template <typename T>
struct decomposed_hand
{
    std::string to_str();

    std::vector<T> combos;
    hand_t remain_hand;
};

template <typename T>
std::string decomposed_hand<T>::to_str()
{
    std::string res = "combos:\n";
    for (auto item : combos)
        res += item.to_str() + " ";
    res += "\nremaining cards:\n";
    res += remain_hand.to_str() + "\n";
    return res;
}

// std::string meld_to_str(const meld_t &md);
// std::string semi_meld_to_str(const semi_meld_t &md);
// void print_hand(const hand_t &hand);

/*game type: 108 card game, 144, 152*/
