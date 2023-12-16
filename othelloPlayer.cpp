#include "othelloPlayer.h"

HumanPlayer::HumanPlayer(std::string playerName, int playerColor)
{
    name = playerName;
    color = playerColor;
}

Move HumanPlayer::chooseMove(OthelloBoard *board)
{
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
                validInput = true;
            }
            else
            {
                std::cout << "Invalid input. Row and column must be integers between 1 and 8." << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid input. Please enter two integers separated by a comma." << std::endl;
            // Clear the input buffer to handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "You entered: " << moveRow << ", " << moveCol << std::endl;
    }

    return std::make_pair(moveRow, moveCol);
}
