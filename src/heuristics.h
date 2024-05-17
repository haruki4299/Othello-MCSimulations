#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "othelloBoard.h"
#include <concepts>

namespace heuristics
{
    // Base class for heuristic functions
    // A greater positive value indicates better for White and vice versa
    class HeuristicFunction
    {
    public:
        virtual int operator()(othelloboard::OthelloBoard *board) const = 0;
        virtual ~HeuristicFunction() = default;
    };

    struct HeuristicEasy : public HeuristicFunction
    {
        int operator()(othelloboard::OthelloBoard *board) const
        {
            int sum = 0;
            // Simply Count the number of tiles in the board for Black and White
            othelloboard::Score score = board->getScores();
            sum = score.second - score.first; // Black is minus and White is plus

            // board->displayBoard();
            // std::cout << sum << std::endl;

            return sum;
        }
    };

    struct HeuristicMedium : public HeuristicFunction
    {
        int operator()(othelloboard::OthelloBoard *board) const
        {
            int sum = 0;
            // Simply Count the number of tiles in the board for Black and White
            othelloboard::Score score = board->getScores();
            sum = score.second - score.first; // Black is minus and White is plus

            // Corners
            sum += 15 * (board->getSquare(1, 1) + board->getSquare(1, 8) + board->getSquare(8, 1) + board->getSquare(8, 8));

            // Danger area => Do not want
            sum += -3 * (board->getSquare(2, 2) + board->getSquare(2, 7) + board->getSquare(7, 2) + board->getSquare(7, 7));
            sum += -3 * (board->getSquare(1, 2) + board->getSquare(2, 1) + board->getSquare(1, 7) + board->getSquare(7, 1));
            sum += -3 * (board->getSquare(2, 8) + board->getSquare(8, 2) + board->getSquare(7, 8) + board->getSquare(8, 7));

            return sum;
        }
    };

    struct HeuristicHard : public HeuristicFunction
    {
        int operator()(othelloboard::OthelloBoard *board) const
        {
            int sum = 0;
            // Simply Count the number of tiles in the board for Black and White
            othelloboard::Score score = board->getScores();
            sum = score.second - score.first; // Black is minus and White is plus

            // // Enhance the importance of the number of chips at the end of the game
            // if (score.second + score.first >= constants::OTHELLO_BOARD_SIZE * constants::OTHELLO_BOARD_SIZE - 4)
            // {
            //     sum = (score.second - score.first) * 100;
            // }

            // Minimize the number of opponent's moves
            sum += board->legalMoves(constants::OTHELLO_WHITE).size() * 2;
            sum -= board->legalMoves(constants::OTHELLO_BLACK).size() * 2;

            // Corners
            sum += 15 * (board->getSquare(1, 1) + board->getSquare(1, 8) + board->getSquare(8, 1) + board->getSquare(8, 8));

            // Danger area => Do not want
            sum += -3 * (board->getSquare(2, 2) + board->getSquare(2, 7) + board->getSquare(7, 2) + board->getSquare(7, 7));
            sum += -3 * (board->getSquare(1, 2) + board->getSquare(2, 1) + board->getSquare(1, 7) + board->getSquare(7, 1));
            sum += -3 * (board->getSquare(2, 8) + board->getSquare(8, 2) + board->getSquare(7, 8) + board->getSquare(8, 7));

            return sum;
        }
    };
};

#endif