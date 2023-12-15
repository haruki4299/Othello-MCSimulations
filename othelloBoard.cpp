#include "othelloBoard.h"

OthelloBoard::OthelloBoard()
{
    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++)
    {
        for (int j = 0; j < OTHELLO_BOARD_SIZE; j++)
        {
            array[i][j] = OTHELLO_EMPTY;
        }
    }

    array[3][3] = OTHELLO_WHITE;
    array[4][4] = OTHELLO_WHITE;
    array[3][4] = OTHELLO_BLACK;
    array[4][3] = OTHELLO_BLACK;
}

OthelloBoard::OthelloBoard(int board[OTHELLO_BOARD_SIZE][OTHELLO_BOARD_SIZE])
{
    for (int i = 0; i < OTHELLO_BOARD_SIZE; ++i)
    {
        for (int j = 0; j < OTHELLO_BOARD_SIZE; ++j)
        {
            array[i][j] = board[i][j];
        }
    }
}

void OthelloBoard::displayBoard()
{
    std::cout << "  ";
    for (int i = 1; i <= OTHELLO_BOARD_SIZE; i++)
    {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++)
    {
        std::cout << i + 1 << ' ';
        for (int j = 0; j < OTHELLO_BOARD_SIZE; j++)
        {
            if (array[i][j] == OTHELLO_WHITE)
            {
                std::cout << 'W' << ' ';
            }
            else if (array[i][j] == OTHELLO_BLACK)
            {
                std::cout << 'B' << ' ';
            }
            else
            {
                std::cout << '-' << ' ';
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    OthelloBoard defaultBoard;
    defaultBoard.displayBoard();
}