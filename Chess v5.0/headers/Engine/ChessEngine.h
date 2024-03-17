#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "../Other/ReadWrite.h"
#include "TranspositionTables.h"
#include "PieceSquareTables.h"


class ChessEngine : public ReadWrite {
    int EngineColor;
    int MAX_DEPTH = 3; // Maximum depth for the Minimax algorithm
    const int infinity = numeric_limits<int>::max();
    TranspositionTables transpostionTable;

    bool terminateSearch; // Atomic flag variable to terminate the search
    bool startSearch; // Atomic flag variable to start the search



public:
    int engineEloRating = 500;
    int NumberofMovesLookedAhead;
    long double TimeTakenForSearch;
    int NumberOfBranchesPruned;
    int NumberOfTranspositionsFound;
    float EngineSpeed;

    ChessEngine(int Color = EMPTY);

    //Engine Setting
    void setEngineColor(int color);
    void SetDepth(int NewDepth);
    int getDepth() const;

    //Functions Related to Search
    void StartSearch();
    void StopSearching();
    void TerminateSearch();
    bool isSearchStarted() const;

    //Move Searching Functions
    void shuffleMoves(vector<string>& possibleMoves);
    string GenerateMove(const ChessBoard& board);
    int Minimax(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer, int color);

    //Evaluation Functions
    int Evaluate(const ChessBoard& chessboard, char currentPlayerColor) const;
    int* InvertTable(const int* originalArray) const;
    int getPSTValue(ChessPiece piece, int squareIndex, char currentPlayerColor) const;
    void adjustEndgamePositionalAdvantage(const ChessBoard& chessboard, int currentPlayerColor, int& positionalAdvantage) const;

    //Sorting Moves
    //void SortMoves(vector<string>& moves, const ChessBoard& board, int color) const;
    bool CheckAfterMove(const string& move, const ChessBoard& board, int color) const;
    bool IsCaptureMove(const string& move, const ChessBoard board) const;

    pair<int, int> convertChessNotationToIndices(const string& move) const;

    void PlayMove(const string& move, ChessBoard& board) const;
    void DisplayMoves(vector<string>& moves);

    //From ReadWrite.h
    int readEloFromFile() override;
    void saveEloToFile() const override;

    void Reset();

    //Functions Related To Transposition Tables
    double getSizeOfTranspositionTable() const;
    void SaveTranspositionTable();

};


#endif // !CHESSENGINE_H



