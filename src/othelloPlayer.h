// othelloPlayer.h
#ifndef OTHELLO_PLAYER_H
#define OTHELLO_PLAYER_H

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <format>
#include <thread>
#include <concepts>

#include "othelloBoard.h"

namespace othelloplayer
{
    // Player Concept
    template <typename T>
    concept Player = requires(T p, othelloboard::OthelloBoard *board) {
        {
            p.chooseMove(board)
        } -> std::convertible_to<othelloboard::Move>;
        {
            p.getColor()
        } -> std::convertible_to<int>;
    };

    class HumanPlayer
    {
    public:
        // Constructors
        HumanPlayer(std::string playerName, int playerColor)
        {
            name = playerName;
            color = playerColor;
        }
        HumanPlayer(const HumanPlayer &) = delete; // Delete copy constructor
        HumanPlayer(HumanPlayer &&) = delete;      // Delete move constructor

        othelloboard::Move chooseMove(othelloboard::OthelloBoard *board);
        int getColor() { return color; }

    private:
        // Player information
        std::string name;
        int color;
    };

    class ComputerPlayer
    {
    public:
        // Constructors
        ComputerPlayer(std::string playerName, int playerColor, int num_theads, int num_simulations)
        {
            name = playerName;
            color = playerColor;
            nThreads = num_theads;
            nSimulations = num_simulations;
        }
        ComputerPlayer(const ComputerPlayer &) = delete; // Delete copy constructor
        ComputerPlayer(ComputerPlayer &&) = delete;      // Delete move constructor

        othelloboard::Move chooseMove(othelloboard::OthelloBoard *board);
        int getColor() { return color; }
        othelloboard::Move selectRandomMove(othelloboard::OthelloBoard *board);
        int simulateToEnd(othelloboard::OthelloBoard *board);

    private:
        // Player information
        std::string name;
        int color;

        // For Parallelism
        int nThreads;
        int nSimulations;

        // othelloboard::Move selectRandomMove(othelloboard::OthelloBoard *board);
    };

    // Asks the human player for a move on the board and if legal makes the move. If not asks again.
    othelloboard::Move HumanPlayer::chooseMove(othelloboard::OthelloBoard *board)
    {
        board->displayBoard();
        // First check if there are any legal moves
        std::vector<othelloboard::Move> moves = board->legalMoves(color);

        // Print out the contents of the moves vector
        std::cout << std::format("Legal Moves: ");
        for (const auto &move : moves)
        {
            std::cout << std::format("({}, {}) ", move.first, move.second);
        }
        std::cout << std::endl;

        // Check if there are any legal moves
        if (moves.empty())
        {
            // Handle the case where there are no legal moves
            return std::make_pair(-1, -1); // or some other indication that no move is available
        }

        // If there are proceed
        othelloboard::Move userMove;
        int moveRow;
        int moveCol;
        bool validInput = false;

        // Keep asking until valid input
        while (!validInput)
        {
            std::string userInput;

            std::cout << std::format("Enter your next move (row, column): ");
            std::cin >> userInput;

            std::istringstream ss(userInput);
            char comma;

            if (ss >> moveRow >> comma >> moveCol && comma == ',')
            {
                if (moveRow >= 1 && moveRow <= constants::OTHELLO_BOARD_SIZE && moveCol >= 1 && moveCol <= constants::OTHELLO_BOARD_SIZE)
                {
                    // Move is within board bounds
                    userMove = std::make_pair(moveRow, moveCol);
                    auto it = std::find(moves.begin(), moves.end(), userMove);
                    if (it != moves.end())
                    {
                        // The move exists within the possible moves
                        validInput = true;
                    }
                    else
                    {
                        std::cout << std::format("Invalid move. Please enter a legal move.\n");
                        // Clear the input buffer to handle invalid input
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    std::cout << std::format("Invalid input. Row and column must be integers between 1 and 8.\n");
                    // Clear the input buffer to handle invalid input
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else
            {
                std::cout << std::format("Invalid input. Please enter two integers separated by a comma.\n");
                // Clear the input buffer to handle invalid input
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        return std::make_pair(moveRow, moveCol);
    }

    // Choose move function for the computer player.
    // if nSimulations is 0, the computer player makes a random move from the possible
    // Else, the computer player does n Simulations on each possible move. The move with
    // the best results from these simulations is chosen as the best possible move.
    othelloboard::Move ComputerPlayer::chooseMove(othelloboard::OthelloBoard *board)
    {
        // If the number of simulations is 0 then just do a random move selection
        if (nSimulations == 0)
        {
            return selectRandomMove(board);
        }

        std::vector<othelloboard::Move> moves = board->legalMoves(color);

        // Check if there are any legal moves
        if (moves.empty())
        {
            // Handle the case where there are no legal moves
            return std::make_pair(-1, -1); // or some other indication that no move is available
        }
        else if (moves.size() == 1)
        {
            // If there is only one move possible just return that move
            return moves[0];
        }

        // Store Results for each possible next move
        std::vector<int> moveResults = std::vector<int>(moves.size(), 0);
        int count = 0;

        // For each next possible move, simulate the results from that position
        for (int index = 0; index < moves.size(); index++)
        {
            othelloboard::Move move = moves[index];
            std::vector<int> results = std::vector<int>(nThreads, 0); // Create vector for each thread to store results
            std::unique_ptr<othelloboard::OthelloBoard> copy = board->makeMove(move.first, move.second, this->getColor());

            std::vector<std::thread> threads; // Container for threads

            nThreads = 4;

            for (int threadId = 0; threadId < nThreads; threadId++)
            {
                threads.emplace_back([&results, &copy, threadId, this]()
                                     {
                                        int start = threadId * (nSimulations / nThreads);
                                        int end = (threadId == nThreads) ? nSimulations: (threadId + 1) * (nSimulations / nThreads);
                                        for (int i = 0; i < nSimulations/nThreads; i++) {
                                            results[threadId] += simulateToEnd(copy.get());
                                        } });
            }

            // Join threads
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

        othelloboard::Move move = moves[maxIndex];
        return move;
    }

    // Computer Player selects a random move from possible moves
    othelloboard::Move ComputerPlayer::selectRandomMove(othelloboard::OthelloBoard *board)
    {
        std::vector<othelloboard::Move> moves = board->legalMoves(color);

        // Check if there are any legal moves
        if (moves.empty())
        {
            // Handle the case where there are no legal moves
            return std::make_pair(-1, -1);
        }

        // Generate a random index within the range of valid moves
        int randomIndex = rand() % moves.size();

        // Retrieve the randomly chosen move
        othelloboard::Move randomMove = moves[randomIndex];

        return randomMove;
    }

    // Simulate the game from the board. The simulation always starts from the sim player.
    // OrigPlayer represents the CPU which is trying to make the current move.
    int ComputerPlayer::simulateToEnd(othelloboard::OthelloBoard *board)
    {
        Player auto simPlayer = ComputerPlayer("BOT", this->getColor() * -1, 1, 1);
        std::unique_ptr<othelloboard::OthelloBoard> copy = std::make_unique<othelloboard::OthelloBoard>(*board);
        othelloboard::Move move;
        othelloboard::Score score;
        int passes = 0;

        // Simulate a game with all random moves till the end
        while (passes < 2)
        {
            move = simPlayer.selectRandomMove(copy.get());
            if (move.first == -1)
            {
                // pass
                passes++;
            }
            else
            {
                copy = copy->makeMove(move.first, move.second, simPlayer.getColor());
            }
            if (passes == 2)
            {
                break;
            }

            move = this->selectRandomMove(copy.get());
            if (move.first == -1)
            {
                // pass
                passes++;
            }
            else
            {
                copy = copy->makeMove(move.first, move.second, this->getColor());
            }
        }

        // Find the results of the game
        score = board->getScores();
        int res;
        if (score.first > score.second)
        {
            // Black Wins
            res = -1;
        }
        else if (score.first < score.second)
        {
            // White Wins
            res = 1;
        }
        else
        {
            // Draw
            res = 0;
        }
        return res;
    }

}

#endif // OTHELLO_PLAYER_H