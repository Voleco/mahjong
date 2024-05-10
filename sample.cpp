#include <iostream>

#include "game_server.h"
#include "hand_evaluator.h"

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
        if (he.is_Win(test_hands[i]))
        {
            std::cout << "hand " << i << " is win\n";
        }
    }
}