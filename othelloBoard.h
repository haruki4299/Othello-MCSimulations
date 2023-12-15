// othelloBoard.h
#include <iostream>
#include <vector>

const int OTHELLO_BOARD_SIZE = 8;
const int OTHELLO_WHITE = 1;
const int OTHELLO_BLACK = -1;
const int OTHELLO_EMPTY = 0;

typedef std::pair<int, int> Move;

class OthelloBoard
{
public:
    OthelloBoard();
    OthelloBoard(int board[OTHELLO_BOARD_SIZE][OTHELLO_BOARD_SIZE]);
    void displayBoard();
    OthelloBoard *makeMove(int row, int col, int piece);
    std::vector<int> getScores();
    void playGame();

private:
    int array[OTHELLO_BOARD_SIZE][OTHELLO_BOARD_SIZE];
    std::vector<Move> legalMoves(int color);
};