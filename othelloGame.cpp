#include "src/othelloPlayer.h"

using namespace std;
using namespace othelloboard;
using namespace othelloplayer;
using namespace constants;

template <typename Player1Type, typename Player2Type>
    requires Player<Player1Type> && Player<Player2Type>
void playGame(Player1Type *player1, Player2Type *player2)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    Score score;

    unique_ptr<OthelloBoard> board = make_unique<OthelloBoard>();
    board->displayBoard();
    score = board->getScores();
    cout << format("Score  Black: {}  White: {}\n", score.first, score.second);

    Move move;
    int passes = 0;

    // Start the game
    while (passes < 2) // Both players must make a move if possible so more than two passes
    {
        // First Player's Turn

        move = player1->chooseMove(board.get());
        if (move.first == -1)
        {
            // No moves possible PASS
            passes++;
        }
        else
        {
            // Make the move
            board = board->makeMove(move.first, move.second, player1->getColor());
            cout << format("Player1 chooses: {}, {}\n", move.first, move.second);
            board->displayBoard();
            score = board->getScores();
            cout << format("Score  Black: {}  White: {}\n", score.first, score.second);
        }

        if (passes == 2)
        {
            break;
        }

        // Second Player's Turn
        move = player2->chooseMove(board.get());
        if (move.first == -1)
        {
            // No moves possible PASS
            passes++;
        }
        else
        {
            // Make the move
            board = board->makeMove(move.first, move.second, player2->getColor());
            cout << format("Player2 chooses: {}, {}\n", move.first, move.second);
            board->displayBoard();
            score = board->getScores();
            cout << format("Score  Black: {}  White: {}\n", score.first, score.second);
        }
    }

    cout << format("Game End\n");
}

int main(int argc, char **argv)
{
    // Reading in command line arguments
    // Default Values Below
    string mode = "hvc"; // hvc (default), cvh, cvc
    int nThreadsC1 = 1;
    int nSimulationsC1 = 1000;
    int nThreadsC2 = 1;
    int nSimulationsC2 = 1000;

    // READ USER INPUT
    if (argc >= 4)
    {
        mode = argv[1];
        nThreadsC1 = atoi(argv[2]);
        nSimulationsC1 = atoi(argv[3]);

        if (nThreadsC1 <= 0 || nSimulationsC1 <= 0)
        {
            cerr << "Error: Invalid number of threads or simulations for Player 1." << endl;
            return 1;
        }
    }
    else
    {
        cerr << "Usage: " << argv[0] << " mode nThreadsC1 nSimulationsC1 [nThreadsC2 nSimulationsC2]" << endl;
        return 1;
    }

    if (argc >= 4)
    {
        nThreadsC2 = atoi(argv[4]);
        nSimulationsC2 = atoi(argv[5]);

        if (nThreadsC2 <= 0 || nSimulationsC2 <= 0)
        {
            cerr << "Error: Invalid number of threads or simulations for Player 2." << endl;
            return 1;
        }
    }

    srand(time(nullptr));

    if (mode == "hvh")
    {
        Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
        Player auto player2 = HumanPlayer("Player2", OTHELLO_WHITE);
        playGame(&player1, &player2);
    }
    else if (mode == "cvc")
    {
        Player auto player1 = ComputerPlayer("Player1", OTHELLO_BLACK, nThreadsC1, nSimulationsC1);
        Player auto player2 = ComputerPlayer("Player2", OTHELLO_WHITE, nThreadsC2, nSimulationsC2);
        playGame(&player1, &player2);
    }
    else if (mode == "cvh")
    {
        Player auto player1 = ComputerPlayer("Player1", OTHELLO_BLACK, nThreadsC1, nSimulationsC1);
        Player auto player2 = HumanPlayer("Player2", OTHELLO_WHITE);
        playGame(&player1, &player2);
    }
    else
    { // "hvc"
        Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
        Player auto player2 = ComputerPlayer("Player2", OTHELLO_WHITE, nThreadsC1, nSimulationsC1);
        playGame(&player1, &player2);
    }
}