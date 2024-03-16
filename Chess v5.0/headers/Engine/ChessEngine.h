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

    void setEngineColor(int color);

    void shuffleMoves(vector<string>& possibleMoves);

    void SetDepth(int NewDepth);

    string GenerateMove(const ChessBoard& board);

    //string GenerateMove(const ChessBoard& board) {
    //    string bestMove;
    //    auto start = high_resolution_clock::now();
    //    NumberofMovesLookedAhead = 0;
    //    NumberOfTranspositionsFound = 0;
    //    for (int depth = 1; depth <= MAX_DEPTH; ++depth) {
    //        bestMove = IterativeDeepening(board, depth);
    //        auto end = high_resolution_clock::now();
    //        auto duration = duration_cast<seconds>(end - start);
    //        TimeTakenForSearch = duration.count();
    //    }
    //    return bestMove;
    //}

    //string IterativeDeepening(const ChessBoard& board, int maxDepth) {
    //    int bestScore = -infinity;
    //    string bestMove;
    //
    //    vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(EngineColor);
    //    //SortMoves(possibleMoves, board, EngineColor);
    //    shuffleMoves(possibleMoves);
    //
    //    for (const string& move : possibleMoves) {
    //        if (terminateSearch) {
    //            cout << "xxxxxxxxxx Search Terminated xxxxxxxxxx" << endl;
    //            terminateSearch = false;
    //            break;
    //        }
    //
    //        ChessBoard tempBoard1566 = board;
    //        pair<int, int> Indices = convertChessNotationToIndices(move);
    //        tempBoard1566.MakeMove(Indices.first, Indices.second);
    //
    //        int score = Minimax(tempBoard1566, maxDepth, -infinity, infinity, false, EngineColor == White ? Black : White);
    //
    //        if (score >= bestScore) {
    //            bestScore = score;
    //            bestMove = move;
    //        }
    //    }
    //
    //    possibleMoves.clear();
    //
    //    return bestMove;
    //}

    int Minimax(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer, int color);

    int Evaluate(const ChessBoard& chessboard, char currentPlayerColor) const;

    void adjustEndgamePositionalAdvantage(const ChessBoard& chessboard, int currentPlayerColor, int& positionalAdvantage) const;

    int getPSTValue(ChessPiece piece, int squareIndex, char currentPlayerColor) const;

    pair<int, int> convertChessNotationToIndices(const string& move) const;

    void PlayMove(const string& move, ChessBoard& board) const;

    int* InvertTable(const int* originalArray) const;

    int readEloFromFile();

    void saveEloToFile() const;

    void Reset();

    // Function to check if a move results in a check
    bool CheckAfterMove(const string& move, const ChessBoard& board, int color) const;

    bool IsCaptureMove(const string& move, const ChessBoard board) const;


    void SortMoves(vector<string>& moves, const ChessBoard& board, int color) const;

    double getSizeOfTranspositionTable() const;

    void StartSearch();

    void StopSearching();

    void TerminateSearch();

    bool isSearchStarted() const;

    void DisplayMoves(vector<string>& moves);

    void SaveTranspositionTable();

    int getDepth() const;

    


};


#endif // !CHESSENGINE_H



