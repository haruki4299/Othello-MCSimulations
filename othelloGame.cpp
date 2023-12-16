#include "othelloBoard.h"
#include "othelloPlayer.h"

int main()
{
    OthelloBoard defaultBoard;
    defaultBoard.displayBoard();
    Move score = defaultBoard.getScores();
    std::cout << score.first << std::endl;

    HumanPlayer player1 = HumanPlayer("ABC", OTHELLO_BLACK);
    Move move = player1.chooseMove(&defaultBoard);
    std::cout << move.first << ',' << move.second << std::endl;

    OthelloBoard *copy1 = defaultBoard.makeMove(move.first - 1, move.second - 1, OTHELLO_BLACK);
    copy1->displayBoard();
}