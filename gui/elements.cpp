#include "elements.hpp"

namespace gui_components
{
    bool card_tile::load(const std::string &prefix)
    {
        std::string fname = std::to_string(card_value) + ".png";
        if (!texture.loadFromFile(prefix + fname))
            return false;

        setSize(sf::Vector2f(70.f, 94.f));
        setTexture(&texture);
        return true;
    }

    std::string card_tile::getName()
    {
        std::string res = "tile_";
        res += get_cardName(card_value);
        return res;
    }

    bool Card_Slot::Add(card_t c)
    {
        for (int i = 0; i < int(partial_hand.size()); i++)
            if (partial_hand[i] == 0)
            {
                partial_hand[i] = c;
                valid_cnt++;
                return true;
            }
        return false;
    }

    void Card_Slot::Sort()
    {
        int left = 0;
        int right = int(partial_hand.size()) - 1;
        while (true)
        {
            while (left < int(partial_hand.size()) && partial_hand[left] != 0)
                left++;
            while (right >= 0 && partial_hand[right] == 0)
                right--;
            if (left >= right)
                break;
            std::swap(partial_hand[left], partial_hand[right]);
        }
        std::sort(partial_hand.begin(), partial_hand.begin() + valid_cnt);
    }
}
