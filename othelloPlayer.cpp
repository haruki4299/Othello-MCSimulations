#include "src/othelloPlayer.h"

using namespace std;
using namespace othelloboard;
using namespace othelloplayer;
using namespace constants;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    unique_ptr<OthelloBoard> board = make_unique<OthelloBoard>();
    board->displayBoard();

    Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
    Player auto player2 = ComputerPlayer("Player2", OTHELLO_WHITE, 1, 1000);

    Move move;

    move = player1.chooseMove(board.get());
    board = board->makeMove(move.first, move.second, player1.getColor());
    board->displayBoard();

    move = player2.chooseMove(board.get());
    board = board->makeMove(move.first, move.second, player2.getColor());
    board->displayBoard();
}