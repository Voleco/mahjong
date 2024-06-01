
#include "cards.h"

std::string semi_meld_t::to_str()
{
    std::string res = "(" + std::to_string(s1) +
                      ", " + std::to_string(s2) + ")";
    return res;
}

std::string meld_t::to_str()
{
    std::string res = "(" + std::to_string(c1) +
                      ", " + std::to_string(c2) +
                      ", " + std::to_string(c3) + ")";
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

std::string hand_t::to_str()
{
    std::string res = "";
    for (int i = 0; i < int(cards.size()); i++)
        if (cards[i] > 0)
            res += "(" + std::to_string(i) + " x " +
                   std::to_string(cards[i]) + ") ";
    return res;
}