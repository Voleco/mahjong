#include "policy.hpp"

std::string Choice::to_str() const
{
    std::string res = "";
    res += "kick " + get_cardName(kick_card) + " and get: {";
    for (auto cb : improving_cards)
        res += "(" + get_cardName(cb.rank) + " x " + std::to_string(cb.cnt) + ") ";
    res += "}";
    return res;
}

void apply_action(hand_t &hand, sub act)
{
    hand.cards[act.out]--;
    hand.cards[act.in]++;
}

void undo_action(hand_t &hand, sub act)
{
    hand.cards[act.out]++;
    hand.cards[act.in]--;
}

struct state
{
    hand_t hand;   // 当前手牌
    ResDeck deck;  // 剩余牌堆
    uint64_t ways; // 累加计数(或概率权重）
    int depth;     // 当前搜索深度
};

// ------ 递归DFS的辅助函数 ------
uint64_t Policy::dfs_impl(hand_t &hand, ResDeck &deck,
                          const Hand_Evaluator &he,
                          int depth, int dep_limit,
                          uint64_t ways) const
{
    // 1) 计算HCost
    int cur_h = he.HCost(hand);

    // 2) 判断是否到达叶子：HCost=-1 或深度达上限
    if (cur_h == -1 || depth == dep_limit)
    {
        return ways;
    }

    // 3) 查看能否进一步改善
    //    通常Get_Improving_Cards只返回“能让HCost更低”的改变，否则返回空
    std::vector<Choice> choices = Get_Improving_Cards(hand, deck);
    if (choices.empty())
    {
        // 无法减少HCost，也视为叶子
        return ways;
    }

    uint64_t result = 0;

    // 4) 为每个Choice遍历
    for (auto &choice : choices)
    {
        // choice.kick_card是打出去的牌
        // choice.improving_cards中列举了“可进的牌”及其剩余数量
        for (auto &mc : choice.improving_cards)
        {
            if (mc.cnt == 0)
            {
                continue; // 没剩余就跳过
            }

            // 构造换牌操作
            sub s{choice.kick_card, mc.rank};
            // 改动 hand
            apply_action(hand, s);

            // 从 deck 中移除1张进的牌
            deck.Remove_Card(mc.rank, 1);

            // ways 乘以 mc.cnt (如果你想将其视为“有 mc.cnt 条分支”)
            // 若你只想算路径数而不加权，也可 ways2 = ways * 1
            uint64_t ways2 = ways * mc.cnt;

            // 递归
            result += dfs_impl(hand, deck, he, depth + 1, dep_limit, ways2);

            // 回溯
            deck.Add_Card(mc.rank, 1);
            undo_action(hand, s);
        }
    }

    return result;
}

// 计算对手牌中每一张可打出的牌，其对应的和牌(Hcost == -1)叶子节点累计数
std::unordered_map<card_t, uint64_t> Policy::Get_Score_DFS(
    const hand_t &start_hand,
    ResDeck start_deck,
    const Hand_Evaluator &he,
    int dep_limit) const
{
    // 结果：其中 key=打出的牌，value=该牌对应的和牌分支数
    std::unordered_map<card_t, uint64_t> result;

    // 收集手牌中所有非零数量的牌
    // 这样只对手里实际有的牌做处理
    std::vector<card_t> unique_cards;
    for (int i = 0; i < (int)start_hand.cards.size(); i++)
    {
        if (start_hand.cards[i] > 0)
        {
            unique_cards.push_back(card_t(i));
        }
    }

    // 对于手牌里每一张牌，假设打出它，然后执行 DFS 计算
    for (auto c : unique_cards)
    {
        hand_t temp_hand = start_hand;

        // 先打出 c
        temp_hand.cards[c]--;
        if (temp_hand.cards[c] < 0)
        {
            // 如果减完为负数，说明此牌无效，恢复后跳过
            temp_hand.cards[c]++;
            continue;
        }

        // ways 从1开始
        uint64_t ways = 1;
        // 搜索并得到当前打出 c 之后的“和牌”叶子节点数
        uint64_t sum_leaves = dfs_impl(temp_hand, start_deck, he, 0, dep_limit, ways);

        // 恢复手牌
        temp_hand.cards[c]++;

        // 记录到结果
        result[c] = sum_leaves;
    }

    return result;
}

std::vector<Choice> Policy::Get_Improving_Cards(const hand_t &hand, const ResDeck &resdeck) const
{
    std::vector<Choice> res;

    Hand_Evaluator he;

    std::vector<sub> actions;
    get_actions(hand, resdeck, actions);

    std::vector<sub>
        good_actions;
    hand_t cur_hand = hand;
    int init_h = he.HCost(cur_hand);
    for (auto act : actions)
    {
        apply_action(cur_hand, act);
        int cur_h = he.HCost(cur_hand);
        if (cur_h < init_h)
            good_actions.push_back(act);
        undo_action(cur_hand, act);
    }

    // std::cout << "good actions:\n";
    // for (auto a : good_actions)
    //     std::cout << "kick " << std::to_string(a.out) << " and get "
    //               << std::to_string(a.in) << "\n";

    for (auto a : good_actions)
    {
        if (res.size() == 0 || res.back().kick_card != a.out)
        {
            Choice neo_choice;
            neo_choice.kick_card = a.out;
            neo_choice.improving_cards.push_back(multi_card{a.in, resdeck.Get_CardCnt(a.in)});
            res.push_back(neo_choice);
        }
        else
        {
            res.back().improving_cards.push_back(multi_card{a.in, resdeck.Get_CardCnt(a.in)});
        }
    }

    // std::cout << "good choices:\n";

    return res;
}