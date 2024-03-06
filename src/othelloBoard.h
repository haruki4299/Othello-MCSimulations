// othelloBoard.h
#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#include <iostream>
#include <vector>
#include <memory>
#include <format>
#include <array>

// Basic Constants
namespace constants
{
    const int OTHELLO_BOARD_SIZE = 8;
    const int OTHELLO_WHITE = 1;  // Represents white on the board
    const int OTHELLO_BLACK = -1; // Represents black on the board
    const int OTHELLO_EMPTY = 0;  // Represents an empty square on the board
}

namespace othelloboard
{
    typedef std::pair<int, int> Move;  // var for move
    typedef std::pair<int, int> Score; // (black score, white score)

    class OthelloBoard
    {
    public:
        // Constructors
        OthelloBoard()
        {
            for (int i = 0; i < constants::OTHELLO_BOARD_SIZE; i++)
            {
                for (int j = 0; j < constants::OTHELLO_BOARD_SIZE; j++)
                {

                    array[i * constants::OTHELLO_BOARD_SIZE + j] = constants::OTHELLO_EMPTY;
                }
            }

            array[3 * constants::OTHELLO_BOARD_SIZE + 3] = constants::OTHELLO_WHITE;
            array[4 * constants::OTHELLO_BOARD_SIZE + 4] = constants::OTHELLO_WHITE;
            array[3 * constants::OTHELLO_BOARD_SIZE + 4] = constants::OTHELLO_BLACK;
            array[4 * constants::OTHELLO_BOARD_SIZE + 3] = constants::OTHELLO_BLACK;
        }

        // Copy Constructor
        OthelloBoard(const OthelloBoard &other)
        {
            for (int i = 0; i < constants::OTHELLO_BOARD_SIZE; ++i)
            {
                for (int j = 0; j < constants::OTHELLO_BOARD_SIZE; ++j)
                {
                    array[i * constants::OTHELLO_BOARD_SIZE + j] = other.array[i * constants::OTHELLO_BOARD_SIZE + j];
                }
            }
        }

        // Copy Assignment Operator
        OthelloBoard &operator=(const OthelloBoard &other)
        {
            if (this != &other)
            {
                for (int i = 0; i < constants::OTHELLO_BOARD_SIZE; ++i)
                {
                    for (int j = 0; j < constants::OTHELLO_BOARD_SIZE; ++j)
                    {
                        array[i * constants::OTHELLO_BOARD_SIZE + j] = other.array[i * constants::OTHELLO_BOARD_SIZE + j];
                    }
                }
            }
            return *this;
        }

        OthelloBoard(OthelloBoard &&other) = default;
        void operator=(OthelloBoard &&other) noexcept { std::swap(array, other.array); }

        ~OthelloBoard() = default;

        // Other Utility Functions
        void displayBoard();
        std::shared_ptr<OthelloBoard> makeMove(int row, int col, int piece);
        std::vector<Move> legalMoves(int player);
        Score getScores();

    private:
        std::array<int, constants::OTHELLO_BOARD_SIZE * constants::OTHELLO_BOARD_SIZE> array;
        bool checkAndFlip(int row, int col, int piece);
    };

    // Display the current board in text format
    void OthelloBoard::displayBoard()
    {
        if (this == nullptr)
        {
            // ptr could be null after invalid move
            return;
        }
        std::cout << "  ";
        for (int i = 1; i <= constants::OTHELLO_BOARD_SIZE; i++)
        {
            std::cout << std::format("{} ", i);
        }
        std::cout << std::endl;

        for (int i = 0; i < constants::OTHELLO_BOARD_SIZE; i++)
        {
            std::cout << std::format("{} ", i + 1);
            for (int j = 0; j < constants::OTHELLO_BOARD_SIZE; j++)
            {
                if (array[i * constants::OTHELLO_BOARD_SIZE + j] == constants::OTHELLO_WHITE)
                {
                    // White piece
                    std::cout << std::format("W ");
                }
                else if (array[i * constants::OTHELLO_BOARD_SIZE + j] == constants::OTHELLO_BLACK)
                {
                    // Black piece
                    std::cout << std::format("B ");
                }
                else
                {
                    // Empty square
                    std::cout << std::format("- ");
                }
            }
            std::cout << std::endl;
        }
    }

    // Get the current score on the board
    Score OthelloBoard::getScores()
    {
        int blackScore = 0;
        int whiteScore = 0;

        for (int i = 0; i < constants::OTHELLO_BOARD_SIZE; i++)
        {
            for (int j = 0; j < constants::OTHELLO_BOARD_SIZE; j++)
            {
                if (array[i * constants::OTHELLO_BOARD_SIZE + j] == constants::OTHELLO_WHITE)
                {
                    // Found white piece
                    whiteScore++;
                }
                else if (array[i * constants::OTHELLO_BOARD_SIZE + j] == constants::OTHELLO_BLACK)
                {
                    // Found black piece
                    blackScore++;
                }
            }
        }

        return std::make_pair(blackScore, whiteScore);
    }

    // Make the move on the othello board
    std::shared_ptr<OthelloBoard> OthelloBoard::makeMove(int row, int col, int piece)
    {
        // A move cannot be made if a piece is already there.
        if (array[(row - 1) * constants::OTHELLO_BOARD_SIZE + (col - 1)] != constants::OTHELLO_EMPTY)
            return nullptr;

        if (piece != constants::OTHELLO_BLACK && piece != constants::OTHELLO_WHITE)
            return nullptr;

        // Make a copy of the board
        std::shared_ptr<OthelloBoard> copyBoard = std::make_shared<OthelloBoard>(*this);
        // OthelloBoard *copyBoard = new OthelloBoard(array);
        copyBoard->array[(row - 1) * constants::OTHELLO_BOARD_SIZE + (col - 1)] = piece;

        // checkAndFlip flips pieces based on the move. If there are no flipped pieces, the move is not valid.
        bool flipped = copyBoard->checkAndFlip(row, col, piece);

        if (flipped)
        {
            // Valid Move
            return copyBoard;
        }

        return nullptr;
    }

    // Makes the move on the board and flips the appropriate pieces.
    // If pieces are flipped as a result it is a valid move.
    // If not the move is not valid.
    bool OthelloBoard::checkAndFlip(int row, int col, int piece)
    {
        bool flipped = false;

        // For each directional vector on the board (including diagonal)
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                    continue;

                int count = 0;
                std::pair<int, int> curSquare = std::make_pair(row - 1, col - 1);

                // See if a flip is possible in this direction
                // To flip we need a piece of the same color in the direction as the newly placed piece
                // and at least one opposition piece in between
                while (true)
                {
                    curSquare.first = curSquare.first + i;
                    curSquare.second = curSquare.second + j;

                    if (curSquare.first < 0 || curSquare.first >= constants::OTHELLO_BOARD_SIZE || curSquare.second < 0 || curSquare.second >= constants::OTHELLO_BOARD_SIZE)
                    {
                        // if out of bounds
                        count = 0;
                        break;
                    }
                    else if (array[curSquare.first * constants::OTHELLO_BOARD_SIZE + curSquare.second] == -1 * piece)
                    {
                        count += 1;
                    }
                    else if (array[curSquare.first * constants::OTHELLO_BOARD_SIZE + curSquare.second] == piece)
                    {
                        break;
                    }
                    else // if empty
                    {
                        count = 0;
                        break;
                    }
                }

                if (count > 0)
                {
                    flipped = true;
                }

                // Actually record the flips
                curSquare = std::make_pair(row - 1, col - 1);
                for (int k = 0; k < count; k++)
                {
                    curSquare.first = curSquare.first + i;
                    curSquare.second = curSquare.second + j;
                    array[curSquare.first * constants::OTHELLO_BOARD_SIZE + curSquare.second] = piece;
                }
            }
        }

        return flipped;
    }

    // Makes a vector list of all of the legal Moves on the board for the player
    std::vector<Move> OthelloBoard::legalMoves(int player)
    {
        std::vector<Move> moves;

        for (int i = 1; i <= constants::OTHELLO_BOARD_SIZE; i++)
        {
            for (int j = 1; j <= constants::OTHELLO_BOARD_SIZE; j++)
            {
                std::shared_ptr<OthelloBoard> result = this->makeMove(i, j, player);
                if (result != nullptr)
                {
                    Move move = std::make_pair(i, j);
                    moves.push_back(move);
                }
            }
        }

        return moves;
    }

}

#endif // OTHELLO_BOARD_H