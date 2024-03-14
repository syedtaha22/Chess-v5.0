#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "ReadWrite.h"
#include "TranspositionTables.h"

class ChessEngine : public ReadWrite {
    int EngineColor;
    int MAX_DEPTH = 2; // Maximum depth for the Minimax algorithm
    const int infinity = numeric_limits<int>::max();
    TranspositionTables transpostionTable;

    bool terminateSearch; // Atomic flag variable to terminate the search
    bool startSearch; // Atomic flag variable to start the search


    const int PawnPST[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
         5,  5, 10, 25, 25, 10,  5,  5,
         0,  0,  0, 20, 20,  0,  0,  0,
         5, -5,-10,  0,  0,-10, -5,  5,
         5, 10, 10,-20,-20, 10, 10,  5,
         0,  0,  0,  0,  0,  0,  0,  0
    };
    const int RooksPST[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
         5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
         0,  0,  0,  5,  5,  0,  0,  0
    };
    const int KnightsPST[64] = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };
    const int BishopsPST[64] = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };
    const int QueensPST[64] = {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
         -5,  0,  5,  5,  5,  5,  0, -5,
          0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };
    const int KingPST[64] = {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
         20, 20,  0,  0,  0,  0, 20, 20,
         20, 30, 10,  0,  0, 10, 30, 20
    };
    const int KingEndgamePST[64] = {
        -50,-40,-30,-20,-20,-30,-40,-50,
        -30,-20,-10,  0,  0,-10,-20,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-30,  0,  0,  0,  0,-30,-30,
        -50,-30,-30,-30,-30,-30,-30,-50
    };

public:
    int engineEloRating = 500;
    int NumberofMovesLookedAhead;
    long long TimeTakenForSearch;
    int NumberOfBranchesPruned;
    int NumberOfTranspositionsFound;

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



