#include "src/othelloPlayer.h"

HumanPlayer::HumanPlayer(std::string playerName, int playerColor)
{
    name = playerName;
    color = playerColor;
}

// Asks the human player for a move on the board and if legal makes the move. If not asks again.
Move HumanPlayer::chooseMove(OthelloBoard *board)
{
    // First check if there are any legal moves
    std::vector<Move> moves = board->legalMoves(color);

    // Print out the contents of the moves vector
    std::cout << "Legal Moves: ";
    for (const auto &move : moves)
    {
        std::cout << "(" << move.first + 1 << ", " << move.second + 1 << ") ";
    }
    std::cout << std::endl;

    // Check if there are any legal moves
    if (moves.empty())
    {
        // Handle the case where there are no legal moves
        return std::make_pair(-1, -1); // or some other indication that no move is available
    }

    // If there are proceed
    Move userMove;
    int moveRow;
    int moveCol;
    bool validInput = false;

    // Keep asking until valid input
    while (!validInput)
    {
        std::string userInput;

        std::cout << "Enter your next move (row, column): ";
        std::cin >> userInput;

        std::istringstream ss(userInput);
        char comma;

        if (ss >> moveRow >> comma >> moveCol && comma == ',')
        {
            if (moveRow >= 1 && moveRow <= OTHELLO_BOARD_SIZE && moveCol >= 1 && moveCol <= OTHELLO_BOARD_SIZE)
            {
                // Move is within board bounds
                userMove = std::make_pair(moveRow - 1, moveCol - 1);
                auto it = std::find(moves.begin(), moves.end(), userMove);
                if (it != moves.end())
                {
                    // The move exists within the possible moves
                    validInput = true;
                }
                else
                {
                    std::cout << "Invalid move. Please enter a legal move." << std::endl;
                    // Clear the input buffer to handle invalid input
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else
            {
                std::cout << "Invalid input. Row and column must be integers between 1 and 8." << std::endl;
                // Clear the input buffer to handle invalid input
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        else
        {
            std::cout << "Invalid input. Please enter two integers separated by a comma." << std::endl;
            // Clear the input buffer to handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return std::make_pair(moveRow, moveCol);
}

ComputerPlayer::ComputerPlayer(std::string playerName, int playerColor)
{
    name = playerName;
    color = playerColor;
}

int ComputerPlayer::getColor()
{
    return color;
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

// Worker Function for parallelization
void workerFunction(int threadID, Move move, int start, int end, OthelloBoard *board, ComputerPlayer *origPlayer, ComputerPlayer *simPlayer, std::vector<int> &results)
{
    OthelloBoard *copy;

    // Each thread has a pre determined amount of work
    for (int i = start; i <= end; i++)
    {
        // Make the move in question and start the simulation
        copy = board->makeMove(move.first, move.second, origPlayer->getColor());
        results[threadID] += simulateToEnd(copy, origPlayer, simPlayer);
    }
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

    std::vector<Move> moves = board->legalMoves(color);

    // Check if there are any legal moves
    if (moves.empty())
    {
        // Handle the case where there are no legal moves
        return std::make_pair(-1, -1); // or some other indication that no move is available
    }

    // Create simulation opponenet
    ComputerPlayer *simPlayer = new ComputerPlayer("BOT", color * -1);

    int movesPerThread = nSimulations / nThreads;
    std::vector<int> moveResults = std::vector<int>(moves.size(), 0);

    for (int index = 0; index < moves.size(); index++)
    {
        // Create a vector to store thread objects
        std::vector<std::thread> threads;
        std::vector<int> results = std::vector<int>(nThreads, 0);

        // Spawn threads
        for (int i = 0; i < nThreads; ++i)
        {
            int start = i * movesPerThread + 1;
            int end = (i == nThreads - 1) ? nSimulations : (i + 1) * movesPerThread;

            threads.emplace_back(workerFunction, i, moves[index], start, end, board, this, simPlayer, std::ref(results));
        }

        // Wait for all threads to finish
        for (auto &thread : threads)
        {
            thread.join();
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
    std::vector<Move> moves = board->legalMoves(color);

    // Check if there are any legal moves
    if (moves.empty())
    {
        // Handle the case where there are no legal moves
        return std::make_pair(-1, -1);
    }

    // Generate a random index within the range of valid moves
    int randomIndex = rand() % moves.size();

    // Retrieve the randomly chosen move
    Move randomMove = moves[randomIndex];

    return randomMove;
}