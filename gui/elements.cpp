#include "elements.hpp"

namespace gui_components
{
    bool card_tile::load(const std::string &prefix)
    {
        std::string fname = std::to_string(card_value) + ".png";
        if (!texture.loadFromFile(prefix + fname))
            return false;

        setSize(sf::Vector2f(35.f, 47.f));
        setTexture(&texture);
        return true;
    }

    std::string card_tile::getName()
    {
        std::string res = "tile_";
        res += get_cardName(card_value);
        return res;
    }

    game_cards::game_cards()
    {
        all_cards = std::vector<card_tile>(MAX_CARD_VALUE, card_tile());
    }

    bool game_cards::load(const std::string &prefix)
    {
        int cnt = 0;
        for (int x = 0; x < 3; x++)
        {
            for (int i = 0; i < 9; i++)
            {
                cnt++;
                if (cnt > 3)
                    return true;
                card_t cur_card = 10 * x + i + 1;
                all_cards[cur_card].set_cardValue(cur_card);
                if (!all_cards[cur_card].load(prefix))
                    return false;
            }
        }
        return true;
    }

}
