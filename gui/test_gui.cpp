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

void render_top_cards(sf::RenderWindow *win_ptr,
                      const gui_components::game_card_factory &card_fac);

int main()
{
    gui_components::game_card_factory gui_cards;
    if (!gui_cards.load("../../resouces/images/theme1/"))
        return -1;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "ImGui Util Demo");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;

    ImGui::SetNextWindowSize(ImVec2(400, 200));
    ImGui::SetNextWindowPos(ImVec2(100, 400)); // Set the position of the new window

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            // if (event.type == sf::Event::MouseButtonPressed)
            // {

            // }

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        sf::Vector2u window_size = window.getSize();
        // std::cout << "winzise: " << window_size.x << ", " << window_size.y << "\n";

        bool show_memu = true;

        ImGui::Begin("Memu", &show_memu);
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        sf::Color bg_color(13, 152, 186);
        window.clear(bg_color);

        render_top_cards(&window, gui_cards);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}

void render_top_cards(sf::RenderWindow *win_ptr,
                      const gui_components::game_card_factory &card_fac)
{
    sf::Vector2u win_size = win_ptr->getSize();

    // int dx = win_size.x / 10;
    int dx = 100;
    int dy = 90;

    // std::cout << "winzise: " << win_size.x << ", " << win_size.y << "\n";
    // std::cout << "dx: " << dx << ", " << "dy: " << dy << "\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            card_t c = i * 10 + j;
            gui_components::card_tile cur_tile = card_fac.make_tile(c);
            float posx = (j - 1) * dx + dx / 2;
            float posy = i * dy + 10;
            // std::cout << "i: " << i << ", " << "j: " << j << "\n";
            // std::cout << "posx: " << posx << ", " << "posy: " << posy << "\n";
            cur_tile.setPosition(posx, posy);
            win_ptr->draw(cur_tile);
        }
    }
    // gui_components::card_tile c1 = gui_cards.get_tile(1);
    // gui_components::card_tile c2 = gui_cards.get_tile(2);
    // c1.setPosition(50, 70);
    // c2.setPosition(100, 70);
    //     window.draw(c1);
    // window.draw(c2);
}
