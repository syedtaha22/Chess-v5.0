#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "../Pieces/ChessPiece.h"
#include "../Pieces/Pieces.h"

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
private:

    //PreComputed Moves for Pieces
    vector<int> SlidingPieceMoveData;
    vector<int> PawnMoveData;
    vector<int> KnightMoveData; 
    vector<int> KingMoveData;

    //PreComputed Moves for Opponents
    vector<int> OpponentMoves;

    //Returns true if it is Whites Turn To Play
    bool currentPlayerIsWhite;

    //Determines wether the Move made is a Castling Move or not
    bool DoCastle;

    // Scores for each piece type
    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 10;

    //Stores Index of Pawn that is Capturable by En Passant
    int enPassantTarget;


public:

    //The Board
    ChessPiece* board[64];

    //Precomputes Move for Selected Piece by User
    vector<int> MovesForSelectedPiece;

    //Contains a History of all Moves Mades
    vector<string> moveHistory;

    //Determines wether a board is Reversed or Not.(For Multiplayer Mode)
    bool isBoardReversed;

    //Wether the Move made was A Captured Move or not
    bool PieceIsCaptured;

    //Stores the last Move that was made in the Format
    //<Initial Index, Final Index>
    pair<int, int> MoveIndices;

    ChessBoard();

    ChessPiece* GetPieceAtPosition(int index) const;

    // Deep copy constructor
    ChessBoard(const ChessBoard& other);

    int calculatePlayerScore(int playerColor) const;
    //Gets Number of attacks on a Square
    int getAttacksOnSquare(int squareIndex, int opponentColor) const;
    int GetKingIndex(const int& playercolor) const;
    int getPieceScore(const int& type) const;
    //Returns the Index of the Piece, Based on its Location
    int getTileIndex(float x, float y, int tileSize);

    void AddMoveToHistory(string move);

    //Functions to Compute Moves for a Piece
    void ComputeKingMoves(int KingIndex, vector<int>& possibleMoves) const;
    void ComputeKnightMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputePawnMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputeSlidingPieceMoves(int pieceIndex, vector<int>& possibleMoves) const;

    //Calculates opponents Moves, and Stores in to OpponentMoves
    void ComputeOpponentMoves();

    void DestroyBoard();

    //Displays Board Elements, on Terminal(For Debugging)
    void DisplayBoard() const;
    void DisplayMoves();
    void DisplayScores() const;

    //Functions to Initialise a Board
    void initializeBoard(); //--> Hard Coded Initilisation
    void initializeBoardFromFEN(const string& fen, bool loadTextures); //Initialises From a Given FEN string
    void InitializeDefaultBoard(); //--> Uses the Defualt FEN string to initialise using 

    //Functions To make Moves 
    void MakeMove(int fromTile, int toTile); //--> Swaps the Squares on the Board
    void MakeCompleteMove(int fromTile, int toTile, string move);

    void PlayChessSound() const;

    void promotePawn(int toTile);
    void ReverseBoard();
    void saveCurrentFENtofile(string file) const;

    //Sets the Postion of Chess Piece on the GUI
    void SetPiecePositions();

    //Enables Pieces to be Dragged and Dropped, Verifies Moves, and Makes a Complete Valid Move
    void UpdateChessPiece(ChessPiece* piece, int InitialIndex);


    //See if a King and Castle Either side based on the Board
    bool canCastleKingSide(int KingIndex) const;
    bool canCastleQueenide(int KingIndex) const;

    bool IsCastlingMove(string move, ChessPiece* pieceMoved);

    bool isCheck(const ChessBoard& chessboard, int playerColor, string calledby) const;
    bool isCheckmate(ChessBoard& chessboard, int playerColor) const;

    // returns currentPlayerIsWhite
    bool isCurrentPlayerWhite() const; 

    bool IsTileUnderAttack(int squareIndex) const;

    bool isValidCaptureMove(int fromTile, int toTile) const;
    bool isValidMove(int index);

    bool IsEnPassantCapture(int fromTile, int toTile) const;

    //Checks if Enpassant is Legal
    bool IsEnPassantLegal(int pawnIndex, int targetIndex) const;

    //Converts a Chess Notation to Indices, for e.g e2e4 --> <60, 44>
    pair<int, int> convertChessNotationToIndices(const string& move) const;
    //Returns <rank, file>
    pair<int, int> PieceCoordinates(int pieceIndex) const;

    vector<int> FilterValidMoves(int fromIndex, vector<int> possibleMoves) const;

    //Functions to get a list of all Possible Moves for a Player or Piece in different Formats
    vector<int> GetAllPossibleMoves(int playerColor) const;
    vector<int> GetAllPossibleMovesForPiece(int type, int index) const;
    vector<string> GetAllCaptureMovesInChessNotation(int color) const;
    vector<string> GetAllPossibleMovesInChessNotation(int playerColor) const;

    //Converts a Indexs to Moves
    string ConvertToChessNotation(int fromTile, int toTile) const;
    //Calculates and returns the Current FEN position of the Board
    string GetCurrentFEN() const;

    //Loops over the The Board and Loads textures for each Piece
    void LoadTextures();

    ~ChessBoard(){
 
        for (ChessPiece* piece : board) {
            delete piece;
        }

    }


};


#endif // !CHESSBOARD_H



