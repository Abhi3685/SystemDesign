#include <bits/stdc++.h>
using namespace std;

/* =========================================================== */
/* ==================== Tic Tac Toe Design =================== */
/* =========================================================== */

/* In general, we have two piece types but this can be extended to support multiple piece types */
enum PlayingPieceType
{
    PieceTypeX,
    PieceTypeO
    // PieceTypeD
    // PieceTypeH
};

/* Players can choose from different types of playing piece at the start of the game */
class PlayingPiece
{
public:
    PlayingPieceType pieceType;
    PlayingPiece(PlayingPieceType pieceType) {
        this->pieceType = pieceType;
    }

    virtual char getPieceSign() = 0;
};

/* PieceX extends Playing Piece to represent a piece type */
class PlayingPieceX: public PlayingPiece
{
public:
    PlayingPieceX(): PlayingPiece(PlayingPieceType::PieceTypeX) {

    }

    char getPieceSign() {
        return 'X';
    }
};


/* PieceY extends Playing Piece to represent a piece type */
class PlayingPieceO: public PlayingPiece
{
public:
    PlayingPieceO(): PlayingPiece(PlayingPieceType::PieceTypeO) {

    }

    char getPieceSign() {
        return 'O';
    }
};

/* Player is one of the main entity and has a playing piece assigned at the start of the game */
class Player
{
public:
    string name;
    PlayingPiece* playingPiece;

    Player(string name, PlayingPiece* playingPiece) {
        this->name = name;
        this->playingPiece = playingPiece;
    }
};

/* GameBoard represents the playing board and at each position it keeps the Piece information placed on that position */
class GameBoard
{
public:
    int size;
    vector<vector<PlayingPiece*>> board;

    GameBoard(int size) {
        this->size = size;
        this->board.resize(size, vector<PlayingPiece*>(size));
    }

    /* Given a row & col and player piece, it marks the current player move on the board if applicable otherwise returns false */
    bool makeMove(int r, int c, PlayingPiece* piece)
    {
        if (board[r][c] != NULL)
            return false;
        
        board[r][c] = piece;
        return true;
    }

    /* Checks if there are any moves left to be made. If the board gets full and no winner yet, it's a tie. */
    bool checkMoveAvailable()
    {
        for (auto row: board) {
            for (auto cell: row) {
                if (cell == NULL)
                    return true;
            }
        }
        return false;
    }

    /* On the basis of last player's move decide if the game is complete or not by following the rules. */
    bool checkWinner(int r, int c, PlayingPiece* piece)
    {
        bool isRowComplete = true;
        bool isColComplete = true;
        bool isDiagonalComplete = true;
        bool isAntiDiagonalComplete = true;
        
        // check row
        for (int i=0; i<size; i++) {
            if (board[r][i] != piece) {
                isRowComplete = false;
                break;
            }
        }

        // check col
        for (int i=0; i<size; i++) {
            if (board[i][c] != piece) {
                isColComplete = false;
                break;
            }
        }

        // check diagonal
        for (int i=0, j=0; i<size; i++, j++) {
            if (board[i][j] != piece) {
                isDiagonalComplete = false;
                break;
            }
        }

        // check anti diagonal
        for (int i=0, j=size-1; i<size; i++, j--) {
            if (board[i][j] != piece) {
                isAntiDiagonalComplete = false;
                break;
            }
        }

        return isRowComplete || isColComplete || isAntiDiagonalComplete || isDiagonalComplete;
    }

    /* Prints the current board layout before each player's move */
    void printToConsole() {
        cout << "Board Layout" << endl;
        for (auto row: board) {
            for (auto cell: row) {
                if (cell != NULL) {
                    cout << cell->getPieceSign();
                }
                cout << "\t" << "|";
            }
            cout << endl;
        }
    }
};

/* Wrapper class to bind our game. Game consists of players and a board to play with. */
class TicTacToeGame
{
public:
    std::deque<Player*> players;
    GameBoard *board;

    TicTacToeGame() {
        this->board = new GameBoard(3);
        players.push_back(new Player("Player1", new PlayingPieceX()));
        players.push_back(new Player("Player2", new PlayingPieceO()));
    }

    void playGame() {
        while(true)
        {
            bool isMoveAvailable = this->board->checkMoveAvailable();
            if (!isMoveAvailable) {
                cout << "Game ended with a tie!" << endl;
                break;
            }

            this->board->printToConsole();

            Player* currentPlayer = players.front();
            int row, col;
            cout << "Enter the position to make the move: ";
            cin >> row >> col;

            int success = this->board->makeMove(row, col, currentPlayer->playingPiece);
            if (!success) {
                cout << "Invalid Move! Please try again." << endl;
                continue;
            }
            
            /* Current player has made the move, move it back to end of list so that turns repeat after all the players have made their moves */
            players.pop_front();
            players.push_back(currentPlayer);

            bool isWinner = this->board->checkWinner(row, col, currentPlayer->playingPiece);
            if (isWinner) {
                cout << "Player " << currentPlayer->name << " won the game!" << endl;
                break;
            }
        }
    }
};

/* Driver function */
int main()
{
    TicTacToeGame game;
    game.playGame();

    return 0;
}
