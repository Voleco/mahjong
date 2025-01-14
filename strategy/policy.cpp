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

    // std::cout << "cur hand: " << hand.to_str() << " ,curH: " << curH << ", depth: " << depth << "\n";

    // 2) 判断是否达成“目标状态”
    //    （示例：curH == -1 代表和牌；或 curH <= initH - dep_limit 代表某种阈值达成）
    if (curH == -1)
    {
        // 找到“有效叶子”
        return 1ULL;
    }

    // 3) 如果达到搜索深度上限，还没达成目标，则不计分
    // 如果已经走到dep_limit
    if (depth == dep_limit)
    {
        // 只有在curH==initH - dep_limit时视为恰好完成dep_limit次改善
        if (curH == initH - dep_limit)
            return 1ULL;
        else
            return 0ULL;
    }

    // 4) 获取当前状态下“能改善手牌”的所有踢牌 + 进牌组合
    std::vector<Choice> choices = Get_Improving_Cards(hand, deck);
    if (choices.empty())
    {
        // 如果没有能改善的动作，则无法继续搜
        return 0ULL;
    }

    // 5) 对每个 Choice 进行展开
    uint64_t ways = 0ULL;
    for (auto &ch : choices)
    {
        // ch.kick_card 是要踢出的那张牌
        // ch.improving_cards 是“进的牌及其张数”
        card_t out_card = ch.kick_card;

        // 如果手牌里根本没有 out_card，跳过(理论上不会发生)
        if (hand.cards[out_card] == 0)
        {
            continue;
        }

        // 先踢掉 out_card
        hand.cards[out_card]--;

        // 然后对每种 improving_card 做尝试
        for (auto &mc : ch.improving_cards)
        {
            card_t in_card = mc.rank;
            uint64_t count_in = mc.cnt;
            if (count_in == 0)
            {
                // 没牌可进，也没贡献
                continue;
            }
            // 如果牌堆里对应数量更少，也需要根据实际可用数量来做处理
            // 这里简单示范
            uint64_t actual_cnt_in = std::min<uint64_t>(count_in, deck.Get_CardCnt(in_card));
            if (actual_cnt_in == 0)
            {
                continue;
            }

            // hand 加一张 in_card
            hand.cards[in_card]++;
            deck.Remove_Card(in_card, 1); // 从牌堆取 1 张

            // 递归搜索下一层（深度+1）
            uint64_t child_ways = dfs_improving(hand, deck, he, depth + 1, dep_limit, initH);
            // 加权累加
            ways += actual_cnt_in * child_ways;

            // 撤销操作
            deck.Add_Card(in_card, 1);
            hand.cards[in_card]--;
        }

        // 撤销踢掉 out_card
        hand.cards[out_card]++;
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
    // map: key=打出的牌, value=search加权分支数
    std::unordered_map<card_t, uint64_t> result;

    // 打印起手信息
    // std::cout << "start hand\n";
    // std::cout << start_hand.to_str() << "\n";
    int initH = he.HCost(start_hand);
    // std::cout << "start hcost = " << initH << "\n";

    // 1) 获取所有“能改善手牌”的踢牌选择
    std::vector<Choice> choices = Get_Improving_Cards(start_hand, start_deck);
    if (choices.empty())
    {
        return result;
    }

    // 2) 对每个 choice，做一次 DFS 并把累加分支数放入 result[kick_card]
    //    其中 choice.kick_card 就是“踢出的那张牌”
    //    choice.improving_cards 是可进的牌及其数量
    for (auto &ch : choices)
    {
        card_t kcard = ch.kick_card;

        // 把手牌复制一份，好做apply/undo
        hand_t temp_hand = start_hand;

        // 先踢出 kcard
        if (temp_hand.cards[kcard] == 0)
        {
            // 理论上不会发生，因为 Get_Improving_Cards 都是基于真实手牌
            continue;
        }
        temp_hand.cards[kcard]--;

        // 计算针对这张 kick_card 的总ways
        uint64_t total_ways_for_this_kcard = 0ULL;

        // 遍历可进的牌
        for (auto &mc : ch.improving_cards)
        {
            card_t card_in = mc.rank;
            uint64_t count_in = mc.cnt;
            if (count_in == 0)
            {
                // 理论上也不太会出现，但以防万一
                continue;
            }

            // 临时处理：手牌加一张进牌
            temp_hand.cards[card_in]++;
            // 牌堆中也要减少 1 张 card_in
            start_deck.Remove_Card(card_in, 1);

            // std::cout << "打掉 " << get_cardName(kcard) << " get " << get_cardName(card_in) << "\n";
            // 开始DFS，从 depth=1 (因为踢+进已经相当于走了一步)
            uint64_t ways = dfs_improving(temp_hand, start_deck, he, 1, dep_limit, initH);

            // std::cout << "total score: " << count_in * ways << "\n";
            // 根据 count_in 进行加权累加
            total_ways_for_this_kcard += count_in * ways;

            //   << " score: " << count_in * ways << "\n";
            // 撤销这张进牌
            start_deck.Add_Card(card_in, 1);
            temp_hand.cards[card_in]--;
        }

        // 把这张踢牌对应的统计值存到 result
        result[kcard] = total_ways_for_this_kcard;
    }

    // 3) 返回汇总结果
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