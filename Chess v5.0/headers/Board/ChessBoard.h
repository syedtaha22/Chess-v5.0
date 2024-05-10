#ifndef CHESSBOARD_H
#define CHESSBOARD_H

//#include "../Pieces/ChessPiece.h"
#include "../Functors/DebugItem.h"
#include "BoardState.h"

    /*
     
    Default Board: 
        Numbers Represent Indexes   

    8 [00][01][02][03][04][05][06][07]
    7 [10][11][12][13][14][15][16][17]
    6 [20][21][22][23][24][25][26][27]
    5 [30][31][32][33][34][35][36][37]
    4 [40][41][42][43][44][45][46][47]
    3 [50][51][52][53][54][55][56][57]
    2 [60][61][62][63][64][65][66][67]
    1 [70][71][72][73][74][75][76][77]
        a   b   c   d   e   f   g   h


    */

    /*
    
    Default Board: 
        Numbers Represent Piece Types

    8 [5][3][4][6][1][4][3][5]
    7 [2][2][2][2][2][2][2][2]
    6 [0][0][0][0][0][0][0][0]
    5 [0][0][0][0][0][0][0][0]
    4 [0][0][0][0][0][0][0][0]
    3 [0][0][0][0][0][0][0][0]
    2 [2][2][2][2][2][2][2][2]
    1 [5][3][4][6][1][4][3][5]
        a  b  c  d  e  f  g  h


    */

class ChessBoard {

public:
    BoardState state;

    //The Board
    ChessPiece* board[64];

    ChessBoard();

    ChessPiece* GetPieceAtPosition(int index) const;

    // Deep copy constructor
    ChessBoard(const ChessBoard& other);

    int calculatePlayerScore(int playerColor) const;
    //Gets Number of attacks on a Square
    int getAttacksOnSquare(int squareIndex, int opponentColor) const;
    int GetKingIndex(const int& playercolor) const;


    void AddMoveToHistory(std::string move);

    //Functions to Compute Moves for a Piece
    void ComputeKingMoves(int KingIndex, std::vector<int>& possibleMoves) const;
    void ComputeKnightMoves(int pieceIndex, std::vector<int>& possibleMoves) const;
    void ComputePawnMoves(int pieceIndex, std::vector<int>& possibleMoves) const;
    void ComputeSlidingPieceMoves(int pieceIndex, std::vector<int>& possibleMoves) const;

    //Calculates opponents Moves, and Stores in to OpponentMoves
    void ComputeOpponentMoves();

    void DestroyBoard();

    //Displays Board Elements, on Terminal(For Debugging)
    void DisplayMoves();
    void DisplayScores() const;

    //Functions to Initialise a Board
    void initializeBoard(); //--> Hard Coded Initilisation
    void initializeBoardFromFEN(const std::string& fen, bool loadTextures); //Initialises From a Given FEN std::string
    void InitializeDefaultBoard(); //--> Uses the Defualt FEN string to initialise using 

    //Functions To make Moves 
    void MakeMove(int fromTile, int toTile); //--> Swaps the Squares on the Board
    void MakeCompleteMove(int fromTile, int toTile, std::string move);

    void PlayChessSound() const;

    void promotePawn(int toTile);
    void ReverseBoard();
    void saveCurrentFENtofile(std::string file) const;

    //Sets the Postion of Chess Piece on the GUI
    void SetPiecePositions();

    //Enables Pieces to be Dragged and Dropped, Verifies Moves, and Makes a Complete Valid Move
    void UpdateChessPiece(ChessPiece* piece, int InitialIndex);

    //See if a King and Castle Either side based on the Board
    bool canCastleKingSide(int KingIndex) const;
    bool canCastleQueenide(int KingIndex) const;

    bool IsCastlingMove(std::string move, ChessPiece* pieceMoved);

    bool isCheck(const ChessBoard& chessboard, const int playerColor, std::string calledby);
    bool isCheckmate(ChessBoard& chessboard, const int playerColor) const;

    // returns currentPlayerIsWhite
    bool isCurrentPlayerWhite() const; 

    bool IsTileUnderAttack(int squareIndex) const;

    bool isValidCaptureMove(int fromTile, int toTile) const;
    bool isValidMove(int index);

    bool IsEnPassantCapture(int fromTile, int toTile) const;

    //Checks if Enpassant is Legal
    bool IsEnPassantLegal(int pawnIndex, int targetIndex) const;

    std::vector<int> FilterValidMoves(int fromIndex, std::vector<int> possibleMoves) const;

    //Functions to get a list of all Possible Moves for a Player or Piece in different Formats
    std::vector<int> GetAllPossibleMoves(int playerColor) const;
    std::vector<int> GetAllPossibleMovesForPiece(int type, int index) const;
    std::vector<std::string> GetAllCaptureMovesInChessNotation(int color) const;
    std::vector<std::string> GetAllPossibleMovesInChessNotation(int playerColor) const;

    //Calculates and returns the Current FEN position of the Board
    std::string GetCurrentFEN() const;

    //Loops over the The Board and Loads textures for each Piece
    void LoadTextures();

    int getCurrentPlayer() const { return isCurrentPlayerWhite() ? White : Black; }

    std::vector<std::string> getMoveHistory() const { return state.moveHistory; }

    int getCheckedPlayer() const { return state.checkedPlayer; }

    ~ChessBoard(){
        for (ChessPiece* piece : board) {
            delete piece;
        }
    }
};


#endif // !CHESSBOARD_H



