#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "../Pieces/ChessPiece.h"
#include "../Other/Flags.h"


class ChessBoard {

private:
    vector<int> SlidingPieceMoveData;
    vector<int> PawnMoveData;
    vector<int> KnightMoveData;
    vector<int> KingMoveData;

    vector<int> MovesForSelectedPiece;
    vector<int> OpponentMoves;

    bool currentPlayerIsWhite;
    bool DoCastle;
    bool PieceIsCaptured;
    pair<int, int> MoveIndices;
    // Scores for each piece type
    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 10;

    int enPassantTarget;

    Flags flags;

public:
    ChessPiece board[64];
    vector<string> moveHistory;
    bool isBoardReversed;

    ChessBoard();
    ChessPiece GetPieceAtPosition(int index) const;

    int calculatePlayerScore(int playerColor) const;
    int getAttacksOnSquare(int squareIndex, int opponentColor) const;
    int GetKingIndex(const int& playercolor) const;
    int getPieceScore(const int& type) const;
    int getTileIndex(float x, float y, int tileSize);

    void AddMoveToHistory(string move);
    void ComputeKingMoves(int KingIndex, vector<int>& possibleMoves) const;
    void ComputeKnightMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputePawnMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void ComputeOpponentMoves();
    void ComputeSlidingPieceMoves(int pieceIndex, vector<int>& possibleMoves) const;
    void DestroyBoard();
    void DisplayBoard() const;
    void DisplayMoves();
    void DisplayScores() const;
    void DrawBoard() const;
    void DrawSquareIndices() const;
    void DrawCoordinates(int index, pair<int, int> PieceCoords, int LocX, int LocY) const;
    void DrawChessPiece() const;
    void initializeBoard();
    void initializeBoardFromFEN(const string& fen);
    void InitializeDefaultBoard();
    void MakeCompleteMove(int fromTile, int toTile, string move);
    void MakeMove(int fromTile, int toTile);
    void PlayChessSound() const;
    void promotePawn(int toTile);
    void ReverseBoard();
    void saveMatchHistoryToFile() const; // Function to generate and save FEN string to file
    void SetPiecePositions();
    void UpdateChessPiece(ChessPiece& piece, int InitialIndex);

    bool canCastleKingSide(int KingIndex) const;
    bool canCastleQueenide(int KingIndex) const;
    bool IsCastlingMove(string move, ChessPiece pieceMoved);
    bool isCheck(const ChessBoard& chessboard, int playerColor, string calledby) const;
    bool isCheckmate(ChessBoard& chessboard, int playerColor) const;
    bool isCurrentPlayerWhite() const;
    bool IsTileUnderAttack(int squareIndex) const;
    bool isValidCaptureMove(int fromTile, int toTile) const;
    bool isValidMove(int index);
    bool IsEnPassantCapture(int fromTile, int toTile) const;

    pair<int, int> convertChessNotationToIndices(const string& move) const;
    pair<int, int> PieceCoordinates(int pieceIndex) const;

    vector<int> FilterValidMoves(int fromIndex, vector<int> possibleMoves) const;
    vector<int> GetAllPossibleMoves(int playerColor) const;
    vector<int> GetAllPossibleMovesForPiece(int type, int index) const;

    vector<string> GetAllCaptureMovesInChessNotation(int color) const;
    vector<string> GetAllPossibleMovesInChessNotation(int playerColor) const;

    string ConvertToChessNotation(int fromTile, int toTile) const;

    bool IsEnPassantLegal(int pawnIndex, int targetIndex) const;
};


#endif // !CHESSBOARD_H



