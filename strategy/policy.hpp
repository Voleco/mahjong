#pragma once

#include "../game/cards.h"
#include "../game/deck.hpp"
#include "../game/hand_evaluator.h"

struct multi_card
{
    card_t rank;
    cardcnt cnt;
};

struct Choice
{
    card_t kick_card;                        /*打出的牌*/
    std::vector<multi_card> improving_cards; /*进张*/
};

class Policy
{
public:
    Policy() {}
    ~Policy() {}

    std::vector<Choice> Get_Improving_Cards(const hand_t &hand) const;

private:
};