// othelloPlayer.h
#ifndef OTHELLO_PLAYER_H
#define OTHELLO_PLAYER_H

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <thread>

#include "othelloBoard.h"

class Player
{
public:
    Player(std::string playerName, int playerColor)
    {
        name = playerName;
        color = playerColor;
    }
    int getColor() { return color; }
    std::string getName() { return name; }

private:
    std::string name;
    int color;
};

class HumanPlayer
{
public:
    HumanPlayer(std::string playerName, int playerColor)
    {
        name = playerName;
        color = playerColor;
    }
    Move chooseMove(OthelloBoard *board);

private:
    std::string name;
    int color;
};

class ComputerPlayer
{
public:
    ComputerPlayer(std::string playerName, int playerColor)
    {
        name = playerName;
        color = playerColor;
    }
    Move chooseMove(OthelloBoard *board, int nThreads, int nSimulations);
    Move selectRandomMove(OthelloBoard *board);
    int getColor() { return color; }

private:
    std::string name;
    int color;
};

#endif // OTHELLO_PLAYER_H