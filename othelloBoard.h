// othelloBoard.h
#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#include <iostream>
#include <vector>

const int OTHELLO_BOARD_SIZE = 8;
const int OTHELLO_WHITE = 1;
const int OTHELLO_BLACK = -1;
const int OTHELLO_EMPTY = 0;

typedef std::pair<int, int> Move;
typedef std::pair<int, int> Score;

class OthelloBoard
{
public:
    OthelloBoard();
    OthelloBoard(int board[OTHELLO_BOARD_SIZE][OTHELLO_BOARD_SIZE]);
    void displayBoard();
    int getElement(int row, int col);
    void setElement(int row, int col, int piece);
    OthelloBoard *makeMove(int row, int col, int piece);
    std::vector<Move> legalMoves(int player);
    Score getScores();
    void playGame();

private:
    int array[OTHELLO_BOARD_SIZE][OTHELLO_BOARD_SIZE];
    bool checkAndFlip(int row, int col, int piece);
};

#endif // OTHELLO_BOARD_H