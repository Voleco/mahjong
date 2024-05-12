#pragma once

#include "cards.h"

class Game_Server
{
public:
    Game_Server();
    ~Game_Server();
    virtual void Create_Deck() = 0;
    virtual void Shuffle_Deck() = 0;
    virtual void Deal_Cards() = 0;
};

class Training_Game_Server : public Game_Server
{
public:
    Training_Game_Server();
    ~Training_Game_Server();
    void Create_Deck() override;
    void Shuffle_Deck() override;
    void Deal_Cards() override;

private:
};