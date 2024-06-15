#include "resource_factory.hpp"

#include <cassert>

bool load_cards_textures(std::vector<card_tile> &all_cards, const std::string &card_image_folder)
{
    for (int x = 0; x < 3; x++)
    {
        for (int i = 0; i < 9; i++)
        {
            card_t cur_card = 10 * x + i + 1;
            all_cards[cur_card].set_cardValue(cur_card);
            if (!all_cards[cur_card].load(card_image_folder))
                return false;
        }
    }
    return true;
}

bool Resource_Factory::LoadResources(const std::string &theme)
{
    all_cards = std::vector<card_tile>(MAX_CARD_VALUE, card_tile());
    fonts.clear();

    std::string folder = "../../resouces";

    std::string text_folder = "/images/theme1/";
    std::string num_font = "/fonts/Sans_Serif.ttf";
    std::string symbol_font = "/fonts/CrimsonText_SemiBold.ttf";
    std::string CN_font = "/fonts/NotoSansSC_Regular.ttf";
    if (theme == "default")
    {
    }

    if (!load_cards_textures(all_cards, folder + text_folder))
        return false;

    sf::Font font1;
    bool va1 = font1.loadFromFile(folder + num_font);
    if (!va1)
        return false;
    sf::Font font2;
    if (!font2.loadFromFile(folder + symbol_font))
        return false;
    sf::Font font3;
    if (!font3.loadFromFile(folder + CN_font))
        return false;
    fonts = {font1, font2, font3};
    assert(fonts.size() == 3);

    return true;
}
