#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "elements.hpp"

int main()
{
    // gui_components::card_tile tile;
    // tile.set_cardValue(1);
    // // tile.setPrefix("../../resouces/images/theme1/");
    // if (!tile.load("../../resouces/images/theme1/"))
    //     return -1;

    gui_components::game_cards gui_cards;
    if (!gui_cards.load("../../resouces/images/theme1/"))
        return -1;

    // // Assign it to a sprite
    // sf::Sprite sprite;
    // sprite.setTexture(texture);

    // sf::Image image;
    // image.loadFromFile("../resources/images/theme1/1.png");
    // sf::Texture texture;
    // texture.loadFromImage(image, area);

    sf::RenderWindow window(sf::VideoMode(800, 600), "ImGui Util Demo");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        return -1;

    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        // window.draw(shape);
        // window.draw(sprite);
        // window.draw(tile);
        gui_components::card_tile c1 = gui_cards.get_tile(1);
        gui_components::card_tile c2 = gui_cards.get_tile(2);
        c1.setPosition(50, 70);
        c2.setPosition(100, 70);

        window.draw(c1);
        window.draw(c2);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
