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
    ComputerPlayer player2 = ComputerPlayer("DCE", OTHELLO_WHITE);
    OthelloBoard *copy;
    Move move;
    int passes = 0;

    while (passes < 2) // Both players must make a move if possible so more than two passes
    {
        move = player1.chooseMove(defaultBoard);
        if (move.first == -1)
        {
            // pass
            passes++;
        }
        else
        {
            copy = defaultBoard->makeMove(move.first - 1, move.second - 1, OTHELLO_BLACK);
            delete defaultBoard;
            defaultBoard = copy;
            defaultBoard->displayBoard();
            score = defaultBoard->getScores();
            std::cout << "Score   Black: " << score.first << " White: " << score.second << std::endl;
        }

        if (passes == 2)
        {
            break;
        }

        move = player2.chooseMove(defaultBoard);
        if (move.first == -1)
        {
            // pass
            passes++;
        }
        else
        {
            copy = defaultBoard->makeMove(move.first, move.second, OTHELLO_WHITE);
            std::cout << "Computer chooses:" << move.first + 1 << ',' << move.second + 1 << std::endl;
            delete defaultBoard;
            defaultBoard = copy;
            defaultBoard->displayBoard();
            score = defaultBoard->getScores();
            std::cout << "Score   Black: " << score.first << " White: " << score.second << std::endl;
        }
    }

    std::cout << "Game End\n";
}