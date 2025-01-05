
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <cstring>

#include "../game/game_server.h"
#include "../game/hand_evaluator.h"
#include "../game/deck.hpp"
#include "../strategy/policy.hpp"

void test1(int test_count);

void test2(int enough_count, int checking_H);

void test3(int enough_count, int checking_H);

int main(int argc, char **argv)
{
    if (argc > 1 && std::strcmp(argv[1], "-h") == 0)
    {
        int test_count = 0;
        if (argc > 2)
            test_count = std::stoi(argv[2]);
        test1(test_count);
    }
    else if (argc > 1 && strcmp(argv[1], "-a") == 0)
    {
        int enough_count = 10;
        if (argc > 2)
            enough_count = std::stoi(argv[2]);
        int checking_H = 0;
        if (argc > 3)
            checking_H = std::stoi(argv[3]);
        test2(enough_count, checking_H);
    }
    else if (argc > 1 && strcmp(argv[1], "-s") == 0)
    {
        int enough_count = 10;
        if (argc > 2)
            enough_count = std::stoi(argv[2]);
        int checking_H = 0;
        if (argc > 3)
            checking_H = std::stoi(argv[3]);
        test3(enough_count, checking_H);
    }
    else
    {
        std::cout << "usage: ...\n";
    }
    return 0;
}

void test1(int test_count)
{
    Hand_Evaluator he;

    // {
    //     std::vector<std::vector<card_t>> test_hands = {
    //         {1, 1, 1, 2, 2},
    //         {1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 9, 9},
    //         {1, 1, 2, 2, 6, 6, 21, 21, 25, 25, 28, 28, 24, 24}};

    //     for (int i = 0; i < int(test_hands.size()); i++)
    //     {
    //         hand_t cur_hand(test_hands[i]);

    //         // std::cout << "cur hand:\n";
    //         std::cout << cur_hand.to_str() << "\n";
    //         if (he.is_Win(cur_hand))
    //         {
    //             std::cout << "hand " << i << " is win\n";
    //         }
    //     }
    // }

    // {
    //     std::vector<std::vector<card_t>> test_hands = {
    //         {5, 6, 8, 8, 11, 12, 13, 15, 15, 27, 28, 29, 29, 29},
    //         {1, 1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9, 9, 9},
    //         {1, 1, 2, 2, 6, 6, 21, 22, 23, 24, 28, 28, 29, 29},
    //         {1, 1, 2, 2, 6, 6, 6, 7, 8, 9, 25, 25, 27, 29}};

    //     for (int i = 0; i < int(test_hands.size()); i++)
    //     {

    //         hand_t cur_hand(test_hands[i]);

    //         auto res = he.full_decomp_hand(cur_hand);
    //         std::cout << "cur hand:\n";
    //         std::cout << cur_hand.to_str() << "\n";
    //         std::cout << "hcost: " << he.HCost(cur_hand) << "\n";
    //         std::cout << "possible decompse: \n";
    //         for (int i = 0; i < int(res.size()); i++)
    //         {
    //             std::cout << "decompose: " << i + 1 << "\n";
    //             std::cout << res[i].to_str() << "\n";
    //         }
    //     }
    // }

    std::vector<hand_t> test_cases;

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

void test2(int enough_count, int checking_H)
{
    Deck deck;
    int test_count = 1000000;

    Hand_Evaluator he;
    Policy ply;

    int enough = 0;
    for (int i = 0; i < test_count; i++)
    {
        if (enough >= enough_count)
            break;
        deck.Shuffle_Cards();
        std::vector<card_t> cur_raw_hand = deck.Deal_Multi_Cards(dc_mode::copy, 14);

        // std::cout << "i= " << i << ", ";
        // for (auto item : cur_raw_hand)
        //     std::cout << int(item) << " ";
        // std::cout << "\n";

        hand_t cur_hand(cur_raw_hand);

        int cur_h = he.HCost(cur_hand);

        if (cur_h == checking_H)
        {
            std::cout << "cur_h: " << cur_h << " cur hand:\n";
            std::cout << cur_hand.to_str() << "\n\n";
            ResDeck resd(cur_hand);
            auto ip_cards = ply.Get_Improving_Cards(cur_hand, resd);
            std::cout << "choice count: " << ip_cards.size() << ", good choices:\n";
            for (auto item : ip_cards)
                std::cout << item.to_str() << "\n";
            std::cout << "\n";
            enough++;
        }
    }
}



void test3(int enough_count, int checking_H)
{
    Deck deck;
    int test_count = 1000000; // 总共尝试次数

    Hand_Evaluator he;
    Policy ply;

    int enough = 0;
    for (int i = 0; i < test_count; i++)
    {
        if (enough >= enough_count)
            break;

        // 1) 洗牌 & 抽 14 张牌
        deck.Shuffle_Cards();
        std::vector<card_t> cur_raw_hand = deck.Deal_Multi_Cards(dc_mode::copy, 14);
        hand_t cur_hand(cur_raw_hand);
        int cur_h = he.HCost(cur_hand);
        if (cur_h != checking_H)
            continue; // 我们只关心 HCost == checking_H 的手牌

        // 如果到这里，说明当前手牌HCost满足
        std::cout << "------------------------------------------------------------------------\n";
        std::cout << "[test3] Found a hand with HCost = " << checking_H << "\n";
        std::cout << "Hand cards: " << cur_hand.to_str() << "\n\n";

        // 2) 构建剩余牌堆
        ResDeck resd(cur_hand);

        // 3) 获取当前手牌的“能让HCost继续下降”的所有Choice
        auto ip_cards = ply.Get_Improving_Cards(cur_hand, resd);

        // 如果没有可提升的牌，直接打印提示即可
        if (ip_cards.empty())
        {
            std::cout << "No improving cards. (Cannot reduce HCost below " << checking_H << ")\n";
        }
        else
        {
            std::cout << "Choice count: " << ip_cards.size() << "\n";

            // 4) 遍历每个 choice (kick_card) 以及对应可进的 improving_cards
            for (auto &choice : ip_cards)
            {
                card_t kc = choice.kick_card;
                std::cout << "  >> Kick card: " << get_cardName(kc) << "\n";
                if (choice.improving_cards.empty())
                {
                    std::cout << "     No improving_cards?\n";
                    continue;
                }

                // 遍历具体可进的牌
                for (auto &mc : choice.improving_cards)
                {
                    card_t in = mc.rank;
                    cardcnt cnt = mc.cnt;   // 剩余张数(可能是 deck里的余量)

                    if (cnt <= 0)
                        continue;

                    // --- 打印这一步的信息 ---
                    std::cout << "     Replace with card: " << get_cardName(in)
                              << " (cnt=" << cnt << " in deck) ... ";

                    // --- 准备做一次“in-place”动作，但要先保存(或回溯) ---
                    //     这里为了让示例简单，不影响当前 cur_hand & resd，
                    //     我们做一个局部拷贝，然后操作并评分
                    hand_t tmp_hand = cur_hand;
                    ResDeck  tmp_resd = resd;

                    // 打出 kc，换进 in
                    tmp_hand.cards[kc]--;
                    tmp_hand.cards[in]++;

                    // 将这一张 in 从牌堆里移除1张
                    tmp_resd.Remove_Card(in, 1);

                    // --- 现在调用 Get_Score_DFS 对新手牌进行评分 ---
                    int dep_limit = 5;
                    uint64_t new_score = ply.Get_Score_DFS(tmp_hand, tmp_resd, he, dep_limit);

                    std::cout << " new_score=" << new_score;

                    // 也可以看看新的 HCost
                    int new_h = he.HCost(tmp_hand);
                    std::cout << " (HCost=" << new_h << ")\n";
                }

                std::cout << "\n";
            }
        }

        std::cout << "------------------------------------------------------------------------\n\n";

        enough++;
    }

    std::cout << "[test3] Finished after checking " << test_count << " random deals. \n"
              << "Found " << enough << " hands with HCost = " << checking_H << ".\n";
}