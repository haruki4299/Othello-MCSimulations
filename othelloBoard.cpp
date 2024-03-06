#include "src/othelloBoard.h"

using namespace std;
using namespace othelloboard;
using namespace constants;

int main()
{
    shared_ptr<OthelloBoard> board = make_shared<OthelloBoard>();
    board->displayBoard();
    board = board->makeMove(3, 4, OTHELLO_BLACK);

    board->displayBoard();
    shared_ptr<OthelloBoard> copy = make_shared<OthelloBoard>(*board);
    cout << "Copy Board\n";
    copy->displayBoard();
    board = board->makeMove(5, 3, OTHELLO_WHITE);
    cout << "Normal Board\n";
    board->displayBoard();
    board = move(copy);
    cout << "Normal Board\n";
    board->displayBoard();
    cout << "Copy Board\n";
    copy->displayBoard();
    std::vector<Move> moves = board->legalMoves(OTHELLO_WHITE);
    for (Move move : moves)
    {
        cout << move.first << " " << move.second << endl;
    }
}