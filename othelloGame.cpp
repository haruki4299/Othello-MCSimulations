#include "src/othelloBoard.h"
#include "src/othelloPlayer.h"

using namespace std;
using namespace constants;

// Mode for Human (Black) vs. Computer (White)
void hvc(int nThreads, int nSimulations)
{
    srand(time(0));

    // Get user name
    string name;
    cout << "Enter Name: ";
    cin >> name;

    // Set up initial board
    OthelloBoard *defaultBoard = new OthelloBoard();
    defaultBoard->displayBoard();
    Score score = defaultBoard->getScores();
    cout << "Score   Black: " << score.first << " White: " << score.second << endl;

    // Create both players
    HumanPlayer player1 = HumanPlayer(name, OTHELLO_BLACK);
    ComputerPlayer player2 = ComputerPlayer("CPU", OTHELLO_WHITE);
    OthelloBoard *copy;
    Move move;
    int passes = 0;

    // Start the game
    while (passes < 2) // Both players must make a move if possible so more than two passes
    {
        // First player's turn
        move = player1.chooseMove(defaultBoard);
        if (move.first == -1)
        {
            // no moves possible so pass
            passes++;
        }
        else
        {
            // Make the move
            copy = defaultBoard->makeMove(move.first - 1, move.second - 1, OTHELLO_BLACK);
            delete defaultBoard;
            defaultBoard = copy;
            defaultBoard->displayBoard();
            score = defaultBoard->getScores();
            cout << "Score   Black: " << score.first << " White: " << score.second << endl;
        }

        if (passes == 2)
        {
            // Two passes in a row indicate that the game can no longer be continued
            break;
        }

        // Second player's turn
        move = player2.chooseMove(defaultBoard, nThreads, nSimulations);
        if (move.first == -1)
        {
            // no moves possible so pass
            passes++;
        }
        else
        {
            // Make the move
            copy = defaultBoard->makeMove(move.first, move.second, OTHELLO_WHITE);
            cout << "Computer chooses:" << move.first + 1 << ',' << move.second + 1 << endl;
            delete defaultBoard;
            defaultBoard = copy;
            defaultBoard->displayBoard();
            score = defaultBoard->getScores();
            cout << "Score   Black: " << score.first << " White: " << score.second << endl;
        }
    }

    cout << "Game End\n";
}

// Mode for Computer (Black) vs. Human (White)
void cvh(int nThreads, int nSimulations)
{
    srand(time(0));

    // Get user name
    string name;
    cout << "Enter Name: ";
    cin >> name;

    // Set up initial board
    OthelloBoard *defaultBoard = new OthelloBoard();
    defaultBoard->displayBoard();
    Score score = defaultBoard->getScores();
    cout << "Score   Black: " << score.first << " White: " << score.second << endl;

    // Create both players
    ComputerPlayer player1 = ComputerPlayer("CPU", OTHELLO_BLACK);
    HumanPlayer player2 = HumanPlayer(name, OTHELLO_WHITE);
    OthelloBoard *copy;
    Move move;
    int passes = 0;

    // Start the game
    while (passes < 2) // Both players must make a move if possible so more than two passes
    {
        move = player1.chooseMove(defaultBoard, nThreads, nSimulations);
        if (move.first == -1)
        {
            // no moves possible so pass
            passes++;
        }
        else
        {
            // Make the move
            copy = defaultBoard->makeMove(move.first, move.second, OTHELLO_BLACK);
            cout << "Computer chooses:" << move.first + 1 << ',' << move.second + 1 << endl;
            delete defaultBoard;
            defaultBoard = copy;
            defaultBoard->displayBoard();
            score = defaultBoard->getScores();
            cout << "Score   Black: " << score.first << " White: " << score.second << endl;
        }

        if (passes == 2)
        {
            // Two passes in a row indicate that the game can no longer be continued
            break;
        }

        // Second player's turn
        move = player2.chooseMove(defaultBoard);
        if (move.first == -1)
        {
            // no moves possible so pass
            passes++;
        }
        else
        {
            // Make the move
            copy = defaultBoard->makeMove(move.first - 1, move.second - 1, OTHELLO_WHITE);
            delete defaultBoard;
            defaultBoard = copy;
            defaultBoard->displayBoard();
            score = defaultBoard->getScores();
            cout << "Score   Black: " << score.first << " White: " << score.second << endl;
        }
    }

    cout << "Game End\n";
}

// Mode for Computer1 (Black) vs. Computer2 (White)
void cvc(int nThreadsC1, int nSimulationsC1, int nThreadsC2, int nSimulationsC2)
{
    srand(time(0));

    // Set up initial board
    OthelloBoard *defaultBoard = new OthelloBoard();
    defaultBoard->displayBoard();
    Score score = defaultBoard->getScores();
    cout << "Score   Black: " << score.first << " White: " << score.second << endl;

    // Create both players
    ComputerPlayer player1 = ComputerPlayer("CPU1", OTHELLO_BLACK);
    ComputerPlayer player2 = ComputerPlayer("CPU2", OTHELLO_WHITE);
    OthelloBoard *copy;
    Move move;
    int passes = 0;

    // Start the game
    while (passes < 2) // Both players must make a move if possible so more than two passes
    {
        // First Player's Turn
        move = player1.chooseMove(defaultBoard, nThreadsC1, nSimulationsC1);
        if (move.first == -1)
        {
            // No moves possible PASS
            passes++;
        }
        else
        {
            // Make the move
            copy = defaultBoard->makeMove(move.first, move.second, OTHELLO_BLACK);
            cout << "Computer chooses:" << move.first + 1 << ',' << move.second + 1 << endl;
            delete defaultBoard;
            defaultBoard = copy;
            defaultBoard->displayBoard();
            score = defaultBoard->getScores();
            cout << "Score   Black: " << score.first << " White: " << score.second << endl;
        }

        if (passes == 2)
        {
            break;
        }

        // Second Player's Turn
        move = player2.chooseMove(defaultBoard, nThreadsC2, nSimulationsC2);
        if (move.first == -1)
        {
            // No moves possible PASS
            passes++;
        }
        else
        {
            // Make the move
            copy = defaultBoard->makeMove(move.first, move.second, OTHELLO_WHITE);
            cout << "Computer chooses:" << move.first + 1 << ',' << move.second + 1 << endl;
            delete defaultBoard;
            defaultBoard = copy;
            defaultBoard->displayBoard();
            score = defaultBoard->getScores();
            cout << "Score   Black: " << score.first << " White: " << score.second << endl;
        }
    }

    cout << "Game End\n";
}

int main(int argc, char **argv)
{
    // Reading in command line arguments
    // Default Values Below
    string mode = "hvc"; // hvc, cvh, cvc
    int nThreadsC1 = 1;
    int nSimulationsC1 = 100;
    int nThreadsC2 = 1;
    int nSimulationsC2 = 100;

    if (argc > 1)
    {
        mode = argv[1];
    }

    // args for first CPU player
    if (argc > 2)
    {
        string str1{argv[2]};
        nThreadsC1 = stoi(str1);
    }

    if (argc > 3)
    {
        string str2{argv[3]};
        nSimulationsC1 = stoi(str2);
    }

    if (nSimulationsC1 < nThreadsC1)
    {
        // nThreads should be less than the number of simulations
        nThreadsC1 = nSimulationsC1;
    }

    // args for second CPU player
    if (argc > 4)
    {
        string str3{argv[4]};
        nThreadsC2 = stoi(str3);
    }

    if (argc > 5)
    {
        string str4{argv[5]};
        nSimulationsC2 = stoi(str4);
    }

    if (nSimulationsC2 < nThreadsC2)
    {
        // nThreads should be less than the number of simulations
        nThreadsC2 = nSimulationsC2;
    }

    // Start the game
    if (mode == "hvc")
    {
        hvc(nThreadsC1, nSimulationsC1);
    }
    else if (mode == "cvh")
    {
        cvh(nThreadsC1, nSimulationsC1);
    }
    else
    {
        cvc(nThreadsC1, nSimulationsC1, nThreadsC2, nSimulationsC2);
    }
}