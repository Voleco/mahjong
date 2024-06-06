#include "deck.hpp"

void Deck::Reset()
{
    remaining_cards.clear();
    taken_cards.clear();
    stacked_remaining_cards = std::vector<cardcnt>(MAX_CARD_VALUE, 0);
    for (int x = 0; x < 3; x++)
    {
        for (int i = 0; i < 9; i++)
        {
            card_t cur_card = 10 * x + i + 1;
            for (int j = 0; j < 4; j++)
                remaining_cards.push_back(cur_card);
            stacked_remaining_cards[cur_card] = 4;
        }
    }
}

std::vector<card_t> Deck::Get_Possible_Cards() const
{
    std::vector<card_t> res;
    for (int i = 0; i < int(stacked_remaining_cards.size()); i++)
        if (stacked_remaining_cards[i] > 0)
            res.push_back(card_t(i));
    return res;
}

void Deck::Shuffle_Cards()
{
    std::shuffle(remaining_cards.begin(), remaining_cards.end(), rng);
}

card_t Deck::Deal_Card(dc_mode mode)
{
    card_t res = remaining_cards.back();
    if (mode == dc_mode::take)
    {
        remaining_cards.pop_back();
        taken_cards.push_back(res);
        stacked_remaining_cards[res]--;
    }
    return res;
}

std::vector<card_t> Deck::Deal_Multi_Cards(dc_mode mode, int n)
{
    std::vector<card_t> res =
        std::vector<card_t>(remaining_cards.begin() + remaining_cards.size() - n, remaining_cards.end());
    if (mode == dc_mode::take)
    {
        remaining_cards.resize(remaining_cards.size() - n);
        taken_cards.insert(taken_cards.end(), res.begin(), res.end());
        for (auto c : res)
            stacked_remaining_cards[c]--;
    }
    return res;
}

void Deck::Take_Card(card_t c)
{
    for (int i = 0; i < int(remaining_cards.size()); i++)
        if (remaining_cards[i] == c)
        {
            std::swap(remaining_cards.back(), remaining_cards[i]);
            remaining_cards.pop_back();
            taken_cards.push_back(c);
            stacked_remaining_cards[c]--;
            return;
        }
}

ResDeck::ResDeck(const hand_t &hand)
{
    stacked_remaining_cards = std::vector<cardcnt>(MAX_CARD_VALUE, 0);
    for (int x = 0; x < 3; x++)
    {
        for (int i = 0; i < 9; i++)
        {
            card_t cur_card = 10 * x + i + 1;
            stacked_remaining_cards[cur_card] = 4 - hand.cards[cur_card];
        }
    }
}

void ResDeck::Take_Card(card_t c)
{
    stacked_remaining_cards[c]--;
}

std::vector<card_t> ResDeck::Get_Possible_Cards() const
{
    std::vector<card_t> res;
    for (int i = 0; i < int(stacked_remaining_cards.size()); i++)
        if (stacked_remaining_cards[i] > 0)
            res.push_back(card_t(i));
    return res;
}