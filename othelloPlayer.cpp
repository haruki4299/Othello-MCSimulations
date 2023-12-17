#include "othelloPlayer.h"

HumanPlayer::HumanPlayer(std::string playerName, int playerColor)
{
    name = playerName;
    color = playerColor;
}

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
    while (!validInput)
    {
        std::string userInput;

        std::cout << "Enter your next move (row, column): ";
        std::cin >> userInput;

        std::istringstream ss(userInput);
        char comma;

        if (ss >> moveRow >> comma >> moveCol && comma == ',')
        {
            if (moveRow >= 1 && moveRow <= 8 && moveCol >= 1 && moveCol <= 8)
            {
                userMove = std::make_pair(moveRow - 1, moveCol - 1);
                auto it = std::find(moves.begin(), moves.end(), userMove);
                if (it != moves.end())
                {
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

int simulateToEnd(OthelloBoard *board, ComputerPlayer *origPlayer, ComputerPlayer *simPlayer)
{
    OthelloBoard *copy;
    Move move;
    Score score;
    int passes = 0;

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
            copy = board->makeMove(move.first, move.second, OTHELLO_BLACK);
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
            copy = board->makeMove(move.first, move.second, OTHELLO_WHITE);
            if (board != NULL)
                delete board;
            board = copy;
        }
    }
    score = board->getScores();
    // std::cout << "Score   Black: " << score.first << " White: " << score.second << std::endl;
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

void workerFunction(int threadID, Move move, int start, int end, OthelloBoard *board, ComputerPlayer *origPlayer, ComputerPlayer *simPlayer, std::vector<int> &results)
{
    OthelloBoard *copy;
    int res;
    for (int i = start; i <= end; i++)
    {
        copy = board->makeMove(move.first, move.second, origPlayer->getColor());
        results[threadID] += simulateToEnd(copy, origPlayer, simPlayer);
    }
}

Move ComputerPlayer::chooseMove(OthelloBoard *board)
{
    int nThreads = 2;
    int nSimulations = 100;

    std::vector<Move> moves = board->legalMoves(color);

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
        std::cout << "Move Results" << index << " Value: " << moveResults[index] << std::endl;
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
    std::cout << "maxIndex: " << maxIndex << std::endl;

    Move move = moves[maxIndex];
    return move;
}

Move ComputerPlayer::selectRandomMove(OthelloBoard *board)
{
    std::vector<Move> moves = board->legalMoves(color);

    // Check if there are any legal moves
    if (moves.empty())
    {
        // Handle the case where there are no legal moves
        return std::make_pair(-1, -1); // or some other indication that no move is available
    }

    // Generate a random index within the range of valid moves
    int randomIndex = rand() % moves.size();

    // Retrieve the randomly chosen move
    Move randomMove = moves[randomIndex];

    return randomMove;
}