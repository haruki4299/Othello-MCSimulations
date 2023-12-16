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

int OthelloBoard::getElement(int row, int col)
{
    return array[row][col];
}

void OthelloBoard::setElement(int row, int col, int piece)
{
    array[row][col] = piece;
}

std::pair<int, int> OthelloBoard::getScores()
{
    int blackScore = 0;
    int whiteScore = 0;

    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++)
    {
        for (int j = 0; j < OTHELLO_BOARD_SIZE; j++)
        {
            if (array[i][j] == OTHELLO_WHITE)
            {
                whiteScore++;
            }
            else if (array[i][j] == OTHELLO_BLACK)
            {
                blackScore++;
            }
        }
    }

    return std::make_pair(blackScore, whiteScore);
}

OthelloBoard *OthelloBoard::makeMove(int row, int col, int piece)
{
    // A move cannot be made if a piece is already there.
    if (array[row][col] != OTHELLO_EMPTY)
        return NULL;

    if (piece != OTHELLO_BLACK && piece != OTHELLO_WHITE)
        return NULL;

    // Make a copy of the board
    OthelloBoard *copyBoard = new OthelloBoard(array);
    copyBoard->array[row][col] = piece;

    bool flipped = copyBoard->checkAndFlip(row, col, piece);

    if (flipped)
    {
        // copyBoard->displayBoard();
        return copyBoard;
    }

    return NULL;
}

bool OthelloBoard::checkAndFlip(int row, int col, int piece)
{
    bool flipped = false;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            int count = 0;
            std::pair<int, int> curSquare = std::make_pair(row, col);

            while (true)
            {
                curSquare.first = curSquare.first + i;
                curSquare.second = curSquare.second + j;

                if (curSquare.first < 0 || curSquare.first >= OTHELLO_BOARD_SIZE || curSquare.second < 0 || curSquare.second >= OTHELLO_BOARD_SIZE)
                {
                    // if out of bounds
                    count = 0;
                    break;
                }
                else if (array[curSquare.first][curSquare.second] == -1 * piece)
                {
                    count += 1;
                }
                else if (array[curSquare.first][curSquare.second] == piece)
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

            // actually record the flips
            curSquare = std::make_pair(row, col);
            for (int k = 0; k < count; k++)
            {
                curSquare.first = curSquare.first + i;
                curSquare.second = curSquare.second + j;
                array[curSquare.first][curSquare.second] = piece;
            }
        }
    }

    return flipped;
}

std::vector<Move> OthelloBoard::legalMoves(int player)
{
    std::vector<Move> moves;

    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++)
    {
        for (int j = 0; j < OTHELLO_BOARD_SIZE; j++)
        {
            OthelloBoard *result = this->makeMove(i, j, player);
            if (result != NULL)
            {
                Move move = std::make_pair(i, j);
                moves.push_back(move);
            }
            delete result;
        }
    }

    return moves;
}
