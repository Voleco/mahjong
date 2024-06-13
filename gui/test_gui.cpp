#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../game/deck.hpp"
#include "elements.hpp"

void render_top_cards(sf::RenderWindow *win_ptr,
                      const gui_components::game_card_factory &card_fac,
                      const std::vector<sf::Font> &fonts,
                      const Deck &cur_deck);

void render_bot_hand(sf::RenderWindow *win_ptr,
                     const gui_components::game_card_factory &card_fac,
                     const gui_components::Card_Slot &selected_cards);

int main()
{
    gui_components::game_card_factory gui_cards;
    if (!gui_cards.load("../../resouces/images/theme1/"))
        return -1;

    sf::Font font1;
    if (!font1.loadFromFile("../../resouces/fonts/Sans_Serif.ttf"))
        return -1;
    sf::Font font2;
    if (!font2.loadFromFile("../../resouces/fonts/CrimsonText_SemiBold.ttf"))
        return -1;

    std::vector<sf::Font> fonts = {font1, font2};

    sf::RenderWindow window(sf::VideoMode(1000, 800), "ImGui Util Demo");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;

    Deck deck;

    gui_components::Card_Slot selected;

    deck.Shuffle_Cards();
    auto hand = deck.Deal_Multi_Cards(dc_mode::take, 14);
    for (int i = 0; i < 14; i++)
        selected.Replace(i, hand[i]);
    // selected.Replace(0, 1);
    // selected.Replace(1, 4);
    // selected.Replace(2, 16);
    // selected.Replace(3, 16);
    // selected.Replace(5, 21);
    // selected.Replace(8, 25);

    ImGui::SetNextWindowSize(ImVec2(400, 200));
    ImGui::SetNextWindowPos(ImVec2(100, 400)); // Set the position of the new window

    // let's define a view
    sf::View view(sf::FloatRect(0, 0, 1000, 800));
    // activate it
    window.setView(view);

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
            // catch the resize events
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
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

        /* view approach start*/

        sf::Font font;
        if (!font.loadFromFile("../../resouces/fonts/CrimsonText_SemiBold.ttf"))
        {
            std::cout << "error font\n";
        }

        // draw something to that view
        render_top_cards(&window, gui_cards, fonts, deck);
        render_bot_hand(&window, gui_cards, selected);

        // want to do visibility checks? retrieve the view
        sf::View currentView = window.getView();

        /*end*/

        // render_top_cards(&window, gui_cards);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}

void render_top_cards(sf::RenderWindow *win_ptr,
                      const gui_components::game_card_factory &card_fac,
                      const std::vector<sf::Font> &fonts,
                      const Deck &cur_deck)
{
    sf::Vector2u win_size = win_ptr->getSize();

    int dx = std::min(int(win_size.x) / 10, 200);
    int init_x = std::max(50, (int((win_size.x) - 9 * dx) / 2));
    int dy = 90;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            card_t c = i * 10 + j;
            gui_components::card_tile cur_tile = card_fac.make_tile(c);
            float posx = (j - 1) * dx + init_x;
            float posy = i * dy + 10;

            // cur_tile.setScale(sf::Vector2f(1, 1));
            cur_tile.setPosition(posx, posy);

            // "x" +
            int cnt = cur_deck.Get_CardCnt(c);
            sf::Text cross;
            cross.setPosition(posx + 70, posy + 30);
            cross.setFont(fonts[0]);
            cross.setString("x");
            cross.setCharacterSize(30);
            cross.setFillColor(sf::Color::Black);

            win_ptr->draw(cross);

            sf::Text text_cnt;
            text_cnt.setPosition(posx + 90, posy);
            text_cnt.setFont(fonts[1]);
            text_cnt.setString(std::to_string(cnt));
            text_cnt.setCharacterSize(70);
            text_cnt.setFillColor(sf::Color::Black);

            win_ptr->draw(text_cnt);

            win_ptr->draw(cur_tile);
        }
    }
}

void render_bot_hand(sf::RenderWindow *win_ptr,
                     const gui_components::game_card_factory &card_fac,
                     const gui_components::Card_Slot &selected_cards)
{
    sf::Vector2u win_size = win_ptr->getSize();

    int dx = std::min(int(win_size.x) / 14, 100);
    int init_x = std::max(50, (int((win_size.x) - 15 * dx) / 2));
    int y = int(win_size.y) - 150;

    for (int i = 0; i < int(selected_cards.partial_hand.size()); i++)
    {
        card_t c = selected_cards.partial_hand[i];
        if (c == 0)
            continue;
        gui_components::card_tile cur_tile = card_fac.make_tile(c);
        float posx = i * dx + init_x;
        float posy = y;
        // std::cout << "i: " << i << "\n";
        // std::cout << "posx: " << posx << ", " << "posy: " << posy << "\n";
        cur_tile.setPosition(posx, posy);
        win_ptr->draw(cur_tile);
    }
}