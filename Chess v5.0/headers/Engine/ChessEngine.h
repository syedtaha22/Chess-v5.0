#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H


#include "EngineState.h"


class ChessEngine{

    int Minimax(ChessBoard& board, int depth, int alpha, int beta, auto time, int currentPlayer);

    //Evaluation Functions
    int Evaluate(const ChessBoard& chessboard, int currentPlayerColor) const;
    void adjustEndgamePositionalAdvantage(const ChessBoard& chessboard, int currentPlayerColor, int& positionalAdvantage) const;

    void shuffleMoves(std::vector<std::string>& possibleMoves);

public:

    EngineState state;
    
    ChessEngine(int Color = EMPTY, bool loadSettings = true);

    //Move Searching Functions
    std::string GenerateMove(const ChessBoard& board);

    void PlayMove(const std::string& move, ChessBoard& board) const;

};


#endif // !CHESS_ENGINE_H