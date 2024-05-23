
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
    std::vector<hand_t> test_hands = {{1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 9, 9},
                                      {1, 1, 2, 2, 6, 6, 21, 21, 25, 25, 28, 28, 24, 24}};

    Hand_Evaluator he;
    for (int i = 0; i < int(test_hands.size()); i++)
    {
        compact_hand_t cur_hand = to_compact_hand(test_hands[i]);
        if (he.is_Win(cur_hand))
        {
            std::cout << "hand " << i << " is win\n";
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

    int count_win = 0;
    std::vector<hand_t> hand_win;

    std::random_device rd;
    std::mt19937 rng(rd());

    auto time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < test_count; i++)
    {
        std::shuffle(all_cards.begin(), all_cards.end(), rng);
        hand_t cur_raw_hand = hand_t(all_cards.begin(), all_cards.begin() + 14);

        // std::cout << "i= " << i << ", ";
        // for (auto item : cur_hand)
        //     std::cout << int(item) << " ";
        // std::cout << "\n";
        compact_hand_t cur_hand = to_compact_hand(cur_raw_hand);
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