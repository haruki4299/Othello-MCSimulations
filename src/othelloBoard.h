// othelloBoard.h
#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#include <iostream>
#include <vector>
#include <memory>

// Basic Constants
namespace constants
{
    const int OTHELLO_BOARD_SIZE = 8;
    const int OTHELLO_WHITE = 1;  // Represents white on the board
    const int OTHELLO_BLACK = -1; // Represents black on the board
    const int OTHELLO_EMPTY = 0;  // Represents an empty square on the board
}

typedef std::pair<int, int> Move;  // var for move
typedef std::pair<int, int> Score; // (black score, white score)

class OthelloBoard
{
public:
    OthelloBoard();
    OthelloBoard(int board[constants::OTHELLO_BOARD_SIZE][constants::OTHELLO_BOARD_SIZE]);
    void displayBoard();
    int getElement(int row, int col);
    void setElement(int row, int col, int piece);
    OthelloBoard *makeMove(int row, int col, int piece);
    std::vector<Move> legalMoves(int player);
    Score getScores();
    void playGame();

private:
    int array[constants::OTHELLO_BOARD_SIZE][constants::OTHELLO_BOARD_SIZE]; // Array representing an 8x8 othello board
    bool checkAndFlip(int row, int col, int piece);
};

#endif // OTHELLO_BOARD_H