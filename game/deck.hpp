#pragma once

#include <random>
#include <algorithm>

#include "cards.h"

/*deal card mode*/
enum class dc_mode
{
    take = 0,
    copy = 1
};

class Deck
{
public:
    Deck()
    {
        Reset();
        std::random_device rd;
        rng = std::mt19937(rd());
    }
    ~Deck() {}

    void Reset();
    void Shuffle_Cards();
    card_t Deal_Card(dc_mode mode);
    std::vector<card_t> Deal_Multi_Cards(dc_mode mode, int n);
    void Take_Card(card_t c);

    std::vector<card_t> Get_Possible_Cards() const;

private:
    std::mt19937 rng;
    std::vector<card_t> remaining_cards;
    std::vector<card_t> taken_cards;
    std::vector<cardcnt> stacked_remaining_cards; /*堆在一起的剩余牌*/
};
