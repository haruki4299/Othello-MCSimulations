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

class HumanPlayer
{
public:
    HumanPlayer(std::string playerName, int playerColor);
    Move chooseMove(OthelloBoard *board);

private:
    std::string name;
    int color;
};

class ComputerPlayer
{
public:
    ComputerPlayer(std::string name, int color);
    Move chooseMove(OthelloBoard *board);
    Move selectRandomMove(OthelloBoard *board);
    int getColor();

private:
    std::string name;
    int color;
};

#endif // OTHELLO_PLAYER_H