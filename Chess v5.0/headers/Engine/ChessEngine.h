#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H


#include "EngineState.h"
#include "PieceSquareTables.h"

class ChessEngine{
public:
    EngineState state;
    const std::string filename = "EngineELO.txt";
   
    ChessEngine(int Color = EMPTY);

    //Move Searching Functions
    void shuffleMoves(std::vector<std::string>& possibleMoves);
    std::string GenerateMove(const ChessBoard& board);
    std::string IterativeDeepening(const ChessBoard& board, int maxDepth);
    int Minimax(ChessBoard& board, int depth, int alpha, int beta, auto time);

    //Evaluation Functions
    int Evaluate(const ChessBoard& chessboard, char currentPlayerColor) const;
    int* InvertTable(const int* originalArray) const;
    int getPSTValue(ChessPiece* piece, int squareIndex, char currentPlayerColor) const;
    void adjustEndgamePositionalAdvantage(const ChessBoard& chessboard, int currentPlayerColor, int& positionalAdvantage) const;

    void PlayMove(const std::string& move, ChessBoard& board) const;

};


#endif // !CHESS_ENGINE_H