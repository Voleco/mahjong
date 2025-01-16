#include "policy.hpp"

template <typename Func>
void try_kick_in(hand_t &hand, ResDeck &deck, card_t out_card,
                 const std::vector<multi_card> &cards_in,
                 Func dfs_or_scoring_fn)
{
    // 如果手里没有 out_card，则直接返回（理论上不会发生）
    if (hand.cards[out_card] == 0)
        return;

    // 先减少手牌中的 out_card
    hand.cards[out_card]--;

    // 遍历可进的牌
    for (auto &mc : cards_in)
    {
        card_t in_card = mc.rank;
        uint64_t count_in = mc.cnt;
        if (count_in == 0)
            continue;

        // 实际可以进的数量可能比 mc.cnt 更少（受牌堆限制）
        uint64_t actual_cnt_in = std::min<uint64_t>(count_in, deck.Get_CardCnt(in_card));
        if (actual_cnt_in == 0)
            continue;

        // 从牌堆取 1 张，手牌加 1 张
        deck.Remove_Card(in_card, 1);
        hand.cards[in_card]++;

        // 在这里执行外部传递进来的搜索逻辑(DFS等)，
        // 并将 actual_cnt_in 作为加权系数之一（若需要）。
        dfs_or_scoring_fn(in_card, actual_cnt_in);

        // 回退
        hand.cards[in_card]--;
        deck.Add_Card(in_card, 1);
    }

    // 撤销踢掉 out_card
    hand.cards[out_card]++;
}

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

uint64_t Policy::dfs_improving(
    hand_t &hand,
    ResDeck &deck,
    const Hand_Evaluator &he,
    int depth,
    int dep_limit,
    int initH // 初始时的HCost
) const
{
    // 1) 计算当前手牌的评估值
    int curH = he.HCost(hand);

    // 2) curH == -1 代表和牌, 有效叶子
    if (curH == -1)
        return 1;

    // 3) 如果达到搜索深度上限，看是否满足特定条件
    if (depth == dep_limit)
        // 只有在 curH == initH - dep_limit 时视为达到目标
        return (curH == initH - dep_limit) ? 1 : 0;

    // 4) 获取当前状态下“能改善手牌”的所有踢牌 + 进牌组合
    std::vector<Choice> choices = Get_Improving_Cards(hand, deck);
    if (choices.empty())
        return 0;

    // 5) 对每个 Choice 进行展开
    uint64_t ways = 0;
    for (auto &ch : choices)
    {
        card_t out_card = ch.kick_card;
        // 使用通用函数 try_kick_in 来做“踢-进-撤销”
        try_kick_in(hand, deck, out_card, ch.improving_cards,
                    [&](card_t in_card, uint64_t actual_cnt_in)
                    {
                        // 这里是自定义的 DFS 逻辑
                        // (in_card 是本次进的牌, actual_cnt_in 是可进的数量)
                        // 调用 dfs_improving 进入下一层
                        uint64_t child_ways = dfs_improving(hand, deck, he, depth + 1, dep_limit, initH);
                        // 根据可进的数量进行加权
                        ways += actual_cnt_in * child_ways;
                    });
    }

    return ways;
}

// 计算对手牌中每一张可打出的牌，其对应的和牌(Hcost == -1)叶子节点累计数
std::unordered_map<card_t, uint64_t> Policy::Get_Score_DFS(
    const hand_t &start_hand,
    ResDeck start_deck,
    const Hand_Evaluator &he,
    int dep_limit) const
{
    std::unordered_map<card_t, uint64_t> result;
    int initH = he.HCost(start_hand);

    // 1) 获取所有“能改善手牌”的踢牌选择
    std::vector<Choice> choices = Get_Improving_Cards(start_hand, start_deck);
    if (choices.empty())
        return result;

    // 2) 对每个 choice，做一次 DFS 并把累加分支数放入 result[kick_card]
    for (auto &ch : choices)
    {
        card_t out_card = ch.kick_card;
        hand_t temp_hand = start_hand;

        // 在 temp_hand 中先踢掉 out_card
        if (temp_hand.cards[out_card] == 0)
            continue;

        uint64_t total_ways_for_this_out = 0;

        // 利用 try_kick_in
        try_kick_in(temp_hand, start_deck, out_card, ch.improving_cards,
                    [&](card_t in_card, uint64_t actual_cnt_in)
                    {
                        // 我们在进入 DFS 前，可以先调用一次 dfs_improving
                        // 这里 depth=1，因为踢+进已经算了一步
                        uint64_t ways = dfs_improving(temp_hand, start_deck, he, 1, dep_limit, initH);
                        total_ways_for_this_out += actual_cnt_in * ways;
                    });

        // 将结果写入
        result[out_card] = total_ways_for_this_out;
    }

    return result;
}

std::vector<Choice> Policy::Get_Improving_Cards(const hand_t &hand, const ResDeck &resdeck) const
{
    std::vector<Choice> res;

    Hand_Evaluator he;

    std::vector<sub> actions;
    get_actions(hand, resdeck, actions);

    std::vector<sub> good_actions;
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