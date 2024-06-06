
#include "cards.h"

std::string get_cardName(card_t c)
{
    std::string res = "";
    int rank = c % 10;
    int type = c / 10;

    res += std::to_string(rank);
    if (type == 0)
        res += "万";
    else if (type == 1)
        res += "筒";
    else
        res += "索";
    return res;
}

std::string semi_meld_t::to_str() const
{
    std::string res = "(" + get_cardName(s1) +
                      ", " + get_cardName(s2) + ")";
    return res;
}

std::string meld_t::to_str() const
{
    std::string res = "(" + get_cardName(c1) +
                      ", " + get_cardName(c2) +
                      ", " + get_cardName(c3) + ")";
    return res;
}

hand_t::hand_t()
{
    init();
}

hand_t::hand_t(const std::vector<card_t> &_cards)
{
    init();
    hand_cnt = _cards.size();
    for (auto &item : _cards)
        cards[item]++;
}

void hand_t::init()
{
    hand_cnt = 0;
    cards = std::vector<cardcnt>(MAX_CARD_VALUE, 0);
}

std::string hand_t::to_str() const
{
    std::string res = "";
    for (int i = 0; i < int(cards.size()); i++)
        if (cards[i] > 0)
            res += "(" + get_cardName(i) + " x " +
                   std::to_string(cards[i]) + ") ";
    return res;
}

std::vector<card_t> hand_t::to_card() const
{
    std::vector<card_t> res;
    for (int i = 0; i < int(cards.size()); i++)
        for (int j = 0; j < cards[i]; j++)
            res.push_back(card_t(i));
    return res;
}

std::string full_DH::to_str() const
{
    std::string res = "melds: [";

    for (auto md : melds)
        res += md.to_str() + " ";
    res += "]\n";
    res += "semi-melds: [";
    for (auto md : semi_melds)
        res += md.to_str() + " ";
    res += "]\n";
    res += "single cards: [";
    for (auto c : single_cards)
        res += get_cardName(c) + " ";
    res += "]";
    return res;
}