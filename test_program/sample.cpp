
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

#include "../game/game_server.h"
#include "../game/hand_evaluator.h"

void test1();

int main()
{
    test1();
    return 0;
}

void test1()
{
    Hand_Evaluator he;

    {
        std::vector<std::vector<card_t>> test_hands = {
            {1, 1, 1, 2, 2},
            {1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 9, 9},
            {1, 1, 2, 2, 6, 6, 21, 21, 25, 25, 28, 28, 24, 24}};

        for (int i = 0; i < int(test_hands.size()); i++)
        {
            hand_t cur_hand(test_hands[i]);

            std::cout << "cur hand:\n";
            std::cout << cur_hand.to_str() << "\n";
            if (he.is_Win(cur_hand))
            {
                std::cout << "hand " << i << " is win\n";
            }
        }
    }

    {
        std::vector<std::vector<card_t>> test_hands = {
            {1, 1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9, 9, 9},
            {1, 1, 2, 2, 6, 6, 21, 22, 23, 24, 28, 28, 29, 29},
            {1, 1, 2, 2, 6, 6, 6, 7, 8, 9, 25, 25, 27, 29}};

        for (int i = 0; i < int(test_hands.size()); i++)
        {

            hand_t cur_hand(test_hands[i]);
            auto res = he.decomp_hand<meld_t>(cur_hand);

            std::cout << "cur hand:\n";
            std::cout << cur_hand.to_str() << "\n";
            std::cout << "possible decompse: \n";
            for (int x = 0; x < int(res.size()); x++)
            {
                std::cout << "decompose: " << x + 1 << "\n";

                std::cout << "meld: ";
                for (auto md : res[x].combos)
                    std::cout << md.to_str() << " ";
                std::cout << "\nleft: ";
                std::cout << res[x].remain_hand.to_str() << "\n";

                std::cout << "deep decompose:\n";
                auto dep_res = he.decomp_hand<semi_meld_t>(res[x].remain_hand);
                for (int y = 0; y < int(dep_res.size()); y++)
                {
                    std::cout << "semi-meld: ";
                    for (auto md : dep_res[y].combos)
                        std::cout << md.to_str() << " ";
                    std::cout << "\nleft: ";
                    std::cout << dep_res[y].remain_hand.to_str() << "\n";
                }

                std::cout << "\n";
            }
        }
    }

    std::vector<hand_t> test_cases;
    std::vector<card_t> all_cards;
    for (int x = 0; x < 3; x++)
    {
        for (int i = 0; i < 9; i++)
        {
            card_t cur_card = 10 * x + i + 1;
            for (int j = 0; j < 4; j++)
                all_cards.push_back(cur_card);
        }
    }

    // int test_count = 1000000;

    int test_count = 0;

    int count_win = 0;
    std::vector<std::vector<card_t>> hand_win;

    std::random_device rd;
    std::mt19937 rng(rd());

    auto time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < test_count; i++)
    {
        std::shuffle(all_cards.begin(), all_cards.end(), rng);
        std::vector<card_t> cur_raw_hand = std::vector<card_t>(all_cards.begin(), all_cards.begin() + 14);

        // std::cout << "i= " << i << ", ";
        // for (auto item : cur_hand)
        //     std::cout << int(item) << " ";
        // std::cout << "\n";
        hand_t cur_hand(cur_raw_hand);
        // auto res = he.decomp_hand(cur_hand);

        if (he.is_Win(cur_hand) == true)
        {
            hand_win.push_back(cur_raw_hand);
            count_win++;
        }
    }

    auto time2 = std::chrono::high_resolution_clock::now();

    auto time_usage = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
    std::cout << "time usage: " << time_usage / 1000.0 << " s\n";

    std::cout << count_win << " win out of " << test_count << "\n";

    // for (auto hand : hand_win)
    // {
    //     std::sort(hand.begin(), hand.end());
    //     for (auto item : hand)
    //         std::cout << int(item) << " ";
    //     std::cout << "\n";
    // }
}