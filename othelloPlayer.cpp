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

Move ComputerPlayer::chooseMove(OthelloBoard *board)
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