#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "../Other/ReadWrite.h"
#include "TranspositionTables.h"
#include "PieceSquareTables.h"


class ChessEngine : public ReadWrite {
    int EngineColor;
    int MAX_DEPTH = 2; // Maximum depth for the Minimax algorithm

    TranspositionTables transpostionTable;

    bool terminateSearch; 
    bool startSearch;
    const float timeLimit = 0;

    const bool useTranspositions = true;
    const bool useAlphaBetaPruning = true;

    const int infinity = numeric_limits<int>::max();


public:
    int engineEloRating = 500;
    int NumberofMovesLookedAhead;
    double TimeTakenForSearch;
    int NumberOfBranchesPruned;
    int NumberOfTranspositionsFound;
    float EngineSpeed;
    int totalMoves;
    int movesEvaluated;
    int totalMovesToEvaluate;
    
   
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
    string IterativeDeepening(const ChessBoard& board, int maxDepth);
    int Minimax(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer, int color, auto time);

    //Evaluation Functions
    int Evaluate(const ChessBoard& chessboard, char currentPlayerColor) const;
    int* InvertTable(const int* originalArray) const;
    int getPSTValue(ChessPiece* piece, int squareIndex, char currentPlayerColor) const;
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


    void SortMoves(vector<string>& moves, const ChessBoard& board, int color) const {
        // Define custom comparator to sort moves
        auto customComparator = [&](const string& move1, const string& move2) {
            // Check if move1 gives check
            bool check1 = CheckAfterMove(move1, board, color);

            // Check if move2 gives check
            bool check2 = CheckAfterMove(move2, board, color);

            // If one move gives check and the other doesn't, prioritize the move that gives check
            if (check1 != check2)
                return check1;

            // If engineh moves give check or engineh moves don't give check, prioritize captures
            bool capture1 = IsCaptureMove(move1, board);
            bool capture2 = IsCaptureMove(move2, board);

            return capture1 > capture2; // Sorting captures before non-captures
            };

        // Sort moves using the custom comparator
        sort(moves.begin(), moves.end(), customComparator);
    }

};


#endif // !CHESSENGINE_H



