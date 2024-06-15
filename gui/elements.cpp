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
}
