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

void apply_action(hand_t &hand, sub act);
void undo_action(hand_t &hand, sub act);

struct Choice
{
    std::string to_str() const;
    card_t kick_card;                        /*打出的牌*/
    std::vector<multi_card> improving_cards; /*进张*/
};

class Policy
{
public:
    Policy() {}
    ~Policy() {}

    /* get cards, kicking which could improve hand */
    std::vector<Choice> Get_Improving_Cards(const hand_t &hand) const;

private:
};