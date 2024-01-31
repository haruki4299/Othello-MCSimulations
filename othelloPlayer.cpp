#include "src/othelloPlayer.h"

using namespace std;
using namespace constants;

// Asks the human player for a move on the board and if legal makes the move. If not asks again.
Move HumanPlayer::chooseMove(OthelloBoard *board)
{
    // First check if there are any legal moves
    vector<Move> moves = board->legalMoves(color);

    // Print out the contents of the moves vector
    cout << format("Legal Moves: ");
    for (const auto &move : moves)
    {
        cout << format("({}, {}) ", move.first + 1, move.second + 1);
    }
    cout << endl;

    // Check if there are any legal moves
    if (moves.empty())
    {
        // Handle the case where there are no legal moves
        return make_pair(-1, -1); // or some other indication that no move is available
    }

    // If there are proceed
    Move userMove;
    int moveRow;
    int moveCol;
    bool validInput = false;

    // Keep asking until valid input
    while (!validInput)
    {
        string userInput;

        cout << format("Enter your next move (row, column): ");
        cin >> userInput;

        istringstream ss(userInput);
        char comma;

        if (ss >> moveRow >> comma >> moveCol && comma == ',')
        {
            if (moveRow >= 1 && moveRow <= OTHELLO_BOARD_SIZE && moveCol >= 1 && moveCol <= OTHELLO_BOARD_SIZE)
            {
                // Move is within board bounds
                userMove = make_pair(moveRow - 1, moveCol - 1);
                auto it = find(moves.begin(), moves.end(), userMove);
                if (it != moves.end())
                {
                    // The move exists within the possible moves
                    validInput = true;
                }
                else
                {
                    cout << format("Invalid move. Please enter a legal move.\n");
                    // Clear the input buffer to handle invalid input
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else
            {
                cout << format("Invalid input. Row and column must be integers between 1 and 8.\n");
                // Clear the input buffer to handle invalid input
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        else
        {
            cout << format("Invalid input. Please enter two integers separated by a comma.\n");
            // Clear the input buffer to handle invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return make_pair(moveRow, moveCol);
}

// Simulate the game from the board. The simulation always starts from the sim player.
// OrigPlayer represents the CPU which is trying to make the current move.
int simulateToEnd(OthelloBoard *board, ComputerPlayer *origPlayer, ComputerPlayer *simPlayer)
{
    OthelloBoard *copy;
    Move move;
    Score score;
    int passes = 0;

    // Simulate a game with all random moves till the end
    while (passes < 2)
    {
        move = simPlayer->selectRandomMove(board);
        if (move.first == -1)
        {
            // pass
            passes++;
        }
        else
        {
            copy = board->makeMove(move.first, move.second, simPlayer->getColor());
            if (board != NULL)
                delete board;
            board = copy;
        }
        if (passes == 2)
        {
            break;
        }

        move = origPlayer->selectRandomMove(board);
        if (move.first == -1)
        {
            // pass
            passes++;
        }
        else
        {
            copy = board->makeMove(move.first, move.second, origPlayer->getColor());
            if (board != NULL)
                delete board;
            board = copy;
        }
    }

    // Find the results of the game
    score = board->getScores();
    int res;
    if (score.first > score.second)
    {
        res = -1;
    }
    else if (score.first < score.second)
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    return res;
}

// Choose move function for the computer player.
// if nSimulations is 0, the computer player makes a random move from the possible
// Else, the computer player does n Simulations on each possible move. The move with
// the best results from these simulations is chosen as the best possible move.
Move ComputerPlayer::chooseMove(OthelloBoard *board, int nThreads, int nSimulations)
{
    // If the number of simulations is 0 then just do a random move selection
    if (nSimulations == 0)
    {
        return selectRandomMove(board);
    }

    vector<Move> moves = board->legalMoves(color);

    // Check if there are any legal moves
    if (moves.empty())
    {
        // Handle the case where there are no legal moves
        return make_pair(-1, -1); // or some other indication that no move is available
    }

    // Create simulation opponenet
    ComputerPlayer *simPlayer = new ComputerPlayer("BOT", color * -1);

    vector<int> moveResults = vector<int>(moves.size(), 0);

    for (int index = 0; index < moves.size(); index++)
    {
        Move move = moves[index];
        vector<int> results = vector<int>(nThreads, 0);
        OthelloBoard *copy;
        int threadID, i;

#pragma omp parallel for default(none) shared(move, results, nSimulations, board, simPlayer) private(threadID, i, copy) num_threads(nThreads) schedule(static)
        for (i = 0; i < nSimulations; ++i)
        {
            threadID = omp_get_thread_num();
            copy = board->makeMove(move.first, move.second, this->getColor());
            results[threadID] += simulateToEnd(copy, this, simPlayer);
        }

        for (int i = 0; i < nThreads; i++)
        {
            moveResults[index] += results[i];
        }
        moveResults[index] *= color;
    }

    delete simPlayer;

    // Find the best move
    int maxIndex = 0;
    int maxVal = moveResults[0];
    for (int i = 1; i < moves.size(); i++)
    {
        if (maxVal < moveResults[i])
        {
            maxIndex = i;
            maxVal = moveResults[i];
        }
    }

    Move move = moves[maxIndex];
    return move;
}

// Computer Player selects a random move from possible moves
Move ComputerPlayer::selectRandomMove(OthelloBoard *board)
{
    vector<Move> moves = board->legalMoves(color);

    // Check if there are any legal moves
    if (moves.empty())
    {
        // Handle the case where there are no legal moves
        return make_pair(-1, -1);
    }

    // Generate a random index within the range of valid moves
    int randomIndex = rand() % moves.size();

    // Retrieve the randomly chosen move
    Move randomMove = moves[randomIndex];

    return randomMove;
}