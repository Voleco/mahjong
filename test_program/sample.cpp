
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

void test3(int enough_count, int checking_H, int depth_limit);

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
        int depth_limit = 2;
        if (argc > 4)
            depth_limit = std::stoi(argv[4]);
        test3(enough_count, checking_H, depth_limit);
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

static std::string quoteString(const std::string &str)
{
    // 简易版本：在字符串外围加双引号
    return "\"" + str + "\"";
}

void test3(int enough_count, int checking_H, int depth_limit)
{
    Deck deck;
    int test_count = 1000000; // 总共尝试次数

    Hand_Evaluator he;
    Policy ply;

    int found_hands = 0;  // 记录满足条件 (HCost == checking_H) 的手牌数
    int sample_index = 0; // 每遇到一副符合条件的手牌，就给它一个序号

    // 先打印一个简单的说明，或者你也可以只输出纯数据
    // 这里我们只打印“#”开头的注释，表明列含义
    // 注意：如果要纯 CSV，可将此注释行去掉
    std::cout << "Sample hands of HCost = " << checking_H << "\n";

    std::cout << "# sample_index,hand (14 cards),[Then multiple lines: card_name,depth1_score,depth2_score,...]\n";

    for (int i = 0; i < test_count; i++)
    {
        if (found_hands >= enough_count)
            break;

        // 1) 洗牌 & 抽 14 张牌
        deck.Shuffle_Cards();
        std::vector<card_t> cur_raw_hand = deck.Deal_Multi_Cards(dc_mode::copy, 14);
        hand_t cur_hand(cur_raw_hand);

        // 只关心 HCost == checking_H 的手牌
        int cur_h = he.HCost(cur_hand);
        if (cur_h != checking_H)
            continue;

        found_hands++;

        // （可选）打印一些分隔符，或先打印该手牌的序号和手牌本身
        // 样式示例：  sample_index,hand
        // 这样后续每张牌的一行“表格”就可直接跟在后面
        std::cout << "[sample start]: " << sample_index << "\n";

        std::cout << sample_index << ","
                  << quoteString(cur_hand.to_str()) << "\n";

        // 2) 构建剩余牌堆
        ResDeck resd(cur_hand);

        // 3) 先收集各深度的分数：card_scores[c][d-1] = 分数
        std::unordered_map<card_t, std::vector<uint64_t>> card_scores;
        card_scores.reserve(40); // 视情况而定

        for (int d = 1; d <= depth_limit; d++)
        {
            // 为保证每次搜索不干扰，复制一份 ResDeck
            ResDeck tmpResd = resd;
            auto scores = ply.Get_Score_DFS(cur_hand, tmpResd, he, d);

            for (auto &kv : scores)
            {
                card_t c = kv.first;
                uint64_t sc = kv.second;
                // 初始化 vector
                if (card_scores[c].size() < (size_t)depth_limit)
                {
                    card_scores[c].resize(depth_limit, 0ULL);
                }
                card_scores[c][d - 1] = sc;
            }
        }

        // 4) 现在按照 "card_scores" 打印表里的一行
        //    行格式形如：
        //    card_name, depth1_score, depth2_score, ...
        // 注意：这里不再打印序号和手牌，以避免重复；你也可以保留
        for (auto &kv : card_scores)
        {
            card_t c = kv.first;
            std::vector<uint64_t> &scores_vec = kv.second;

            // card_name
            std::cout << get_cardName(c);

            // 打印 depth=1..depth_limit 的分数，用逗号分隔
            for (auto sc : scores_vec)
            {
                std::cout << "," << sc;
            }
            // 每张牌打印一行
            std::cout << "\n";
        }

        // 打印完当前手牌各项后，换行 (或加个空行) 以分隔下一副手牌
        std::cout << "[sample end]: " << sample_index << "\n\n";
        sample_index++;
    }

    // 最后补充一个总结信息
    std::cout << "[test3] Finished after checking " << test_count
              << " random deals. \nFound " << found_hands
              << " hands with HCost = " << checking_H << ".\n";
}