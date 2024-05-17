#include "src/othelloPlayer.h"
#include "src/heuristics.h"

using namespace std;
using namespace othelloboard;
using namespace othelloplayer;
using namespace heuristics;
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
            passes = 0;
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
            passes = 0;
        }
    }

    cout << format("Game End\n");
}

int main(int argc, char **argv)
{
    // Reading in command line arguments
    // Default Values Below
    int mode = 0; // human vs computer MC : 0, computer MC vs human: 1, human vs human: 2, computer MC vs computer MC: 3,
    int nThreadsC1 = 1;
    int nSimulationsC1 = 1000;
    int nThreadsC2 = 1;
    int nSimulationsC2 = 1000;
    int heuristic1 = 0; // Easy: 0, Medium: 1, Hard: 2
    int heuristic2 = 0;
    int plies = 8; // How deep the minimax algorithm will look into

    if (argc >= 2)
    {
        mode = atoi(argv[1]);
    }

    if (mode >= 4)
    {
        // Using MiniMax (+ Potentially MC)
        // Read MiniMax CPU settings
        if (argc >= 3)
        {
            plies = atoi(argv[2]);

            if (plies < 0)
            {
                plies = 8;
            }
        }

        // Read MC CPU Settings
        if (argc >= 5)
        {
            nThreadsC1 = atoi(argv[3]);
            nSimulationsC1 = atoi(argv[4]);

            if (nThreadsC1 <= 0 || nSimulationsC1 < 0)
            {
                nThreadsC1 = 1;
                nSimulationsC1 = 1000;
            }
        }
    }
    else
    {
        // Only MC
        // Read First MC CPU settings
        if (argc >= 4)
        {
            nThreadsC1 = atoi(argv[2]);
            nSimulationsC1 = atoi(argv[3]);

            if (nThreadsC1 <= 0 || nSimulationsC1 < 0)
            {
                cerr << "Error: Invalid number of threads or simulations for Player 1." << endl;
                return 1;
            }
        }
        // Read Second MC CPU settings
        if (argc >= 6)
        {
            nThreadsC2 = atoi(argv[4]);
            nSimulationsC2 = atoi(argv[5]);

            if (nThreadsC2 <= 0 || nSimulationsC2 < 0)
            {
                cerr << "Error: Invalid number of threads or simulations for Player 2." << endl;
                return 1;
            }
        }
    }

    srand(time(nullptr));

    if (mode == 0)
    {
        cout << format("Human versus Human Mode\n");
        Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
        Player auto player2 = HumanPlayer("Player2", OTHELLO_WHITE);
        playGame(&player1, &player2);
    }
    else if (mode == 1)
    {
        cout << format("Computer (MC) versus Computer (MC) Mode\n");
        Player auto player1 = ComputerPlayerMC("Player1", OTHELLO_BLACK, nThreadsC1, nSimulationsC1);
        Player auto player2 = ComputerPlayerMC("Player2", OTHELLO_WHITE, nThreadsC2, nSimulationsC2);
        playGame(&player1, &player2);
    }
    else if (mode == 2)
    {
        cout << format("Computer versus Human Mode\n");
        Player auto player1 = ComputerPlayerMC("Player1", OTHELLO_BLACK, nThreadsC1, nSimulationsC1);
        Player auto player2 = HumanPlayer("Player2", OTHELLO_WHITE);
        playGame(&player1, &player2);
    }
    else if (mode == 3)
    { // "hvc"
        cout << format("Human versus Computer Mode\n");
        Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
        Player auto player2 = ComputerPlayerMC("Player2", OTHELLO_WHITE, nThreadsC1, nSimulationsC1);
        playGame(&player1, &player2);
    }
    else if (mode == 4)
    { // "hvc"
        cout << format("Human versus Computer (MiniMax-Easy) Mode\n");
        HeuristicEasy he;
        Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
        Player auto player2 = ComputerPlayerMM("Player2", OTHELLO_WHITE, plies, he);
        playGame(&player1, &player2);
    }
    else if (mode == 5)
    { // "hvc"
        cout << format("Human versus Computer (MiniMax-Medium) Mode\n");
        HeuristicMedium hm;
        Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
        Player auto player2 = ComputerPlayerMM("Player2", OTHELLO_WHITE, plies, hm);
        playGame(&player1, &player2);
    }
    else if (mode == 6)
    { // "hvc"
        cout << format("Human versus Computer (MiniMax-Hard) Mode\n");
        HeuristicHard hh;
        Player auto player1 = HumanPlayer("Player1", OTHELLO_BLACK);
        Player auto player2 = ComputerPlayerMM("Player2", OTHELLO_WHITE, plies, hh);
        playGame(&player1, &player2);
    }
    else if (mode == 7)
    { // "cvc"
        cout << format("Computer (Minimax-Hard) versus Computer (MC) Mode\n");
        HeuristicHard hh;
        Player auto player1 = ComputerPlayerMM("Player1", OTHELLO_BLACK, plies, hh);
        Player auto player2 = ComputerPlayerMC("Player2", OTHELLO_WHITE, nThreadsC1, nSimulationsC1);
        playGame(&player1, &player2);
    }
    else if (mode == 8)
    { // "cvc"
        cout << format("Computer (MC) versus  Computer (Minimax-Hard) Mode\n");
        HeuristicHard hh;
        Player auto player1 = ComputerPlayerMC("Player1", OTHELLO_BLACK, nThreadsC1, nSimulationsC1);
        Player auto player2 = ComputerPlayerMM("Player2", OTHELLO_WHITE, plies, hh);
        playGame(&player1, &player2);
    }
    else if (mode == 9)
    {
        cout << format("Computer (Minimax-Easy) versus  Computer (Minimax-Hard) Mode\n");
        HeuristicEasy he;
        HeuristicHard hh;
        Player auto player1 = ComputerPlayerMM("Player1", OTHELLO_BLACK, plies, he);
        Player auto player2 = ComputerPlayerMM("Player2", OTHELLO_WHITE, plies, hh);
        playGame(&player1, &player2);
    }
    else if (mode == 10)
    {
        cout << format("Computer (Minimax-Medium) versus  Computer (Minimax-Hard) Mode\n");
        HeuristicMedium hm;
        HeuristicHard hh;
        Player auto player1 = ComputerPlayerMM("Player1", OTHELLO_BLACK, plies, hm);
        Player auto player2 = ComputerPlayerMM("Player2", OTHELLO_WHITE, plies, hh);
        playGame(&player1, &player2);
    }
}