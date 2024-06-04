
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

#include "../game/game_server.h"
#include "../game/hand_evaluator.h"
#include "../game/deck.hpp"
#include "../strategy/policy.hpp"

void test1();

void test2();

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

            auto res = he.full_decomp_hand(cur_hand);
            std::cout << "cur hand:\n";
            std::cout << cur_hand.to_str() << "\n";
            std::cout << "hcost: " << he.HCost(cur_hand) << "\n";
            std::cout << "possible decompse: \n";
            for (int i = 0; i < int(res.size()); i++)
            {
                std::cout << "decompose: " << i + 1 << "\n";
                std::cout << res[i].to_str() << "\n";
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

    int test_count = 1000000;

    // int test_count = 10;

    int count_win = 0;
    std::vector<std::vector<card_t>> hand_win;
    std::vector<std::vector<card_t>> hand__1;

    Deck deck;

    auto time1 = std::chrono::high_resolution_clock::now();

    std::vector<int> distribution(10, 0);
    for (int i = 0; i < test_count; i++)
    {
        deck.Shuffle_Cards();
        std::vector<card_t> cur_raw_hand = deck.Deal_Multi_Cards(dc_mode::copy, 14);

        // std::cout << "i= " << i << ", ";
        // for (auto item : cur_raw_hand)
        //     std::cout << int(item) << " ";
        // std::cout << "\n";

        hand_t cur_hand(cur_raw_hand);

        if (he.is_Win(cur_hand) == true)
        {
            hand_win.push_back(cur_raw_hand);
            count_win++;
        }

        int cur_h = he.HCost(cur_hand);
        distribution[cur_h + 1]++;

        if (cur_h == -1)
            hand__1.push_back(cur_raw_hand);

        // auto res = he.full_decomp_hand(cur_hand);
        // auto res = he.decomp_hand<meld_t>(cur_hand);
    }

    auto time2 = std::chrono::high_resolution_clock::now();

    auto time_usage = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
    std::cout << "time usage: " << time_usage / 1000.0 << " s\n";

    std::cout << "distribution:\n";
    for (int i = 0; i < distribution.size(); i++)
        std::cout << i - 1 << "_to draw: \t" << distribution[i] << "\n";

    std::cout << count_win << " win out of " << test_count << "\n";

    std::cout << "hand win cnt:" << hand_win.size() << "\n";
    for (auto hand : hand_win)
    {
        std::sort(hand.begin(), hand.end());
        for (auto item : hand)
            std::cout << int(item) << " ";
        std::cout << "\n";
    }

    std::cout << "hand h(-1) cnt:" << hand__1.size() << " \n";
    for (auto hand : hand__1)
    {
        std::sort(hand.begin(), hand.end());
        for (auto item : hand)
            std::cout << int(item) << " ";
        std::cout << "\n";
    }
}

void test2()
{
    Deck deck;
    int test_count = 10;

    Policy ply;

    for (int i = 0; i < test_count; i++)
    {
        deck.Shuffle_Cards();
        std::vector<card_t> cur_raw_hand = deck.Deal_Multi_Cards(dc_mode::copy, 14);

        // std::cout << "i= " << i << ", ";
        // for (auto item : cur_raw_hand)
        //     std::cout << int(item) << " ";
        // std::cout << "\n";

        hand_t cur_hand(cur_raw_hand);
        auto ip_cards = ply.Get_Improving_Cards(cur_hand);
    }
}