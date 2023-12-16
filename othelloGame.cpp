#include "othelloBoard.h"
#include "othelloPlayer.h"

int main()
{
    srand(time(0));

    OthelloBoard *defaultBoard = new OthelloBoard();
    defaultBoard->displayBoard();
    Move score = defaultBoard->getScores();
    std::cout << "Score   Black: " << score.first << " White: " << score.second << std::endl;

    HumanPlayer player1 = HumanPlayer("ABC", OTHELLO_BLACK);
    Move move = player1.chooseMove(defaultBoard);

    OthelloBoard *copy1 = defaultBoard->makeMove(move.first - 1, move.second - 1, OTHELLO_BLACK);
    delete defaultBoard;
    defaultBoard = copy1;
    defaultBoard->displayBoard();

    ComputerPlayer player2 = ComputerPlayer("DCE", OTHELLO_WHITE);
    move = player2.chooseMove(defaultBoard);
    copy1 = defaultBoard->makeMove(move.first, move.second, OTHELLO_WHITE);
    std::cout << "Computer chooses:" << move.first + 1 << ',' << move.second + 1 << std::endl;
    delete defaultBoard;
    defaultBoard = copy1;
    defaultBoard->displayBoard();
}