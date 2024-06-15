#pragma once

#include <string>

#include <SFML/Graphics/Text.hpp>

#include "elements.hpp"



using gui_components::card_tile;

class Resource_Factory
{
public:
    bool LoadResources(const std::string &theme);

    const sf::Font &GetNumFont() const { return fonts[0]; }
    const sf::Font &GetSymbolFont() const { return fonts[1]; }
    const sf::Font &GetCNFont() const { return fonts[2]; }

    card_tile MakeTile(card_t c) const { return all_cards[c]; }

    std::vector<gui_components::card_tile> all_cards;

    // gui_components::game_card_factory card_fac;
    std::vector<sf::Font> fonts;
};