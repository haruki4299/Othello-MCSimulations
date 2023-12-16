// othelloPlayer.h
#ifndef OTHELLO_PLAYER_H
#define OTHELLO_PLAYER_H

#include <string>
#include <iostream>
#include <sstream>

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
    void chooseMove(OthelloBoard *board);

private:
    std::string name;
    int color;
    int simulate(OthelloBoard *board, int turn);
};

#endif // OTHELLO_PLAYER_H