#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

using card_t = uint8_t;

/*
万子牌 = 1~9
筒子牌 = 11~19
索子牌 = 21~29

东 南 西 北 = 31 32 33 34
白 发 中 =  35 36 37
春、夏、秋、冬	= 41 42 43 44	　
梅、兰、竹、菊	= 45 46 47 48	　
*/

using cardcnt = uint8_t;
const card_t MAX_CARD_VALUE = 30;

std::string get_cardName(card_t c);

struct meld_t
{
    std::string to_str() const;
    card_t c1;
    card_t c2;
    card_t c3;
};

struct semi_meld_t
{
    std::string to_str() const;
    card_t s1;
    card_t s2;
};

struct hand_t
{
    hand_t();
    hand_t(const std::vector<card_t> &_cards);
    void init();
    std::string to_str() const;
    std::vector<card_t> to_card() const;

    std::vector<cardcnt> cards;
    int hand_cnt;
};

template <typename T>
struct decomposed_hand
{
    std::string to_str() const;

    std::vector<T> combos;
    hand_t remain_hand;
};

template <typename T>
std::string decomposed_hand<T>::to_str() const
{
    std::string res = "combos:\n";
    for (auto item : combos)
        res += item.to_str() + " ";
    res += "\nremaining cards:\n";
    res += remain_hand.to_str() + "\n";
    return res;
}

/* full decomposed hand*/
struct full_DH
{
    std::string to_str() const;

    std::vector<meld_t> melds;
    std::vector<semi_meld_t> semi_melds;
    std::vector<card_t> single_cards;
    int has_atama; /*semi_melds contains atama? yes: 1, no: 0*/
};

/*game type: 108 card game, 144, 152*/
