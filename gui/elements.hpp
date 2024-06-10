
#include <string>
#include <map>

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../game/cards.h"
#include "../game/deck.hpp"

namespace gui_components
{

    class card_tile : public sf::RectangleShape
    {
    public:
        void set_cardValue(card_t c) { card_value = c; }
        bool load(const std::string &prefix);
        std::string getName();
        // void setPrefix(const std::string &s) { prefix = s; }

    private:
        card_t card_value;
        sf::Texture texture;
        // std::string prefix;
    };

    class game_cards
    {
    public:
        game_cards();
        bool load(const std::string &prefix);
        card_tile get_tile(card_t c) { return card_tile(all_cards[c]); }

    private:
        std::vector<card_tile> all_cards;
    };

}

// class MyEntity : public sf::Drawable, public sf::Transformable
// {
// public:

//     // add functions to play with the entity's geometry / colors / texturing...

// private:

//     virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
//     {
//         // apply the entity's transform -- combine it with the one that was passed by the caller
//         states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

//         // apply the texture
//         states.texture = &m_texture;

//         // you may also override states.shader or states.blendMode if you want

//         // draw the vertex array
//         target.draw(m_vertices, states);
//     }

//     sf::VertexArray m_vertices;
//     sf::Texture m_texture;
// };