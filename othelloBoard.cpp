#include "src/othelloBoard.h"

using namespace std;
using namespace constants;

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

// Display the current board in text format
void OthelloBoard::displayBoard()
{
    cout << "  ";
    for (int i = 1; i <= OTHELLO_BOARD_SIZE; i++)
    {
        cout << i << ' ';
    }
    cout << endl;

    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++)
    {
        cout << i + 1 << ' ';
        for (int j = 0; j < OTHELLO_BOARD_SIZE; j++)
        {
            if (array[i][j] == OTHELLO_WHITE)
            {
                // White piece
                cout << 'W' << ' ';
            }
            else if (array[i][j] == OTHELLO_BLACK)
            {
                // Black piece
                cout << 'B' << ' ';
            }
            else
            {
                // Empty square
                cout << '-' << ' ';
            }
        }
        cout << endl;
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

// Get the current score on the board
Score OthelloBoard::getScores()
{
    int blackScore = 0;
    int whiteScore = 0;

    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++)
    {
        for (int j = 0; j < OTHELLO_BOARD_SIZE; j++)
        {
            if (array[i][j] == OTHELLO_WHITE)
            {
                // Found white piece
                whiteScore++;
            }
            else if (array[i][j] == OTHELLO_BLACK)
            {
                // Found black piece
                blackScore++;
            }
        }
    }

    return make_pair(blackScore, whiteScore);
}

// Make the move on the othello board
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

    // checkAndFlip flips pieces based on the move. If there are no flipped pieces, the move is not valid.
    bool flipped = copyBoard->checkAndFlip(row, col, piece);

    if (flipped)
    {
        // Valid Move
        return copyBoard;
    }

    return NULL;
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
            pair<int, int> curSquare = make_pair(row, col);

            // See if a flip is possible in this direction
            // To flip we need a piece of the same color in the direction as the newly placed piece
            // and at least one opposition piece in between
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

            // Actually record the flips
            curSquare = make_pair(row, col);
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

// Makes a vector list of all of the legal Moves on the board for the player
vector<Move> OthelloBoard::legalMoves(int player)
{
    vector<Move> moves;

    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++)
    {
        for (int j = 0; j < OTHELLO_BOARD_SIZE; j++)
        {
            OthelloBoard *result = this->makeMove(i, j, player);
            if (result != NULL)
            {
                Move move = make_pair(i, j);
                moves.push_back(move);
            }
            delete result;
        }
    }

    return moves;
}
