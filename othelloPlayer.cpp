#include "src/othelloPlayer.h"
#include "src/heuristics.h"

using namespace std;
using namespace othelloboard;
using namespace othelloplayer;
using namespace constants;
using namespace heuristics;

int numFunc()
{
    return 30;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    unique_ptr<OthelloBoard> board = make_unique<OthelloBoard>();
    board->displayBoard();

    HeuristicEasy he;

    Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
    Player auto player2 = ComputerPlayerMC("Player2", OTHELLO_WHITE, 1, 1000);
    Player auto player3 = ComputerPlayerMM<HeuristicEasy>("Player3", OTHELLO_WHITE, 4, he);
    Player auto player4 = ComputerPlayerMM<HeuristicEasy>("Player3", OTHELLO_BLACK, 4, he);

    Move move;

    // move = player1.chooseMove(board.get());
    // board = board->makeMove(move.first, move.second, player1.getColor());
    // board->displayBoard();

    // move = player2.chooseMove(board.get());
    // board = board->makeMove(move.first, move.second, player2.getColor());
    // board->displayBoard();

    move = player3.chooseMove(board.get());
    board = board->makeMove(move.first, move.second, player3.getColor());
    board->displayBoard();

    move = player4.chooseMove(board.get());
    board = board->makeMove(move.first, move.second, player4.getColor());
    board->displayBoard();
}