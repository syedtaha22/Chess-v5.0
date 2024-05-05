#ifndef CHESS_ENGINE_STATISTICS_H
#define CHESS_ENGINE_STATISTICS_H

struct ChessEngineStatistics {
    int NumberofMovesLookedAhead = 0;
    int BranchesPruned = 0;
    int TranspositionsFound = 0;
    float TimeTaken = 0;
    float SizeOfTable = 0;
    float Speed = 0; // n/s ---> kn/s
    int maxDepth = 0;
    size_t totalMoves = 0;
    int movesEvaluated = 0;
    size_t totalMovesToEvaluate = 0;

    void reset() {
        TimeTaken = 0;
        BranchesPruned = 0;
        TranspositionsFound = 0;
        NumberofMovesLookedAhead = 0;
        TimeTaken = 0;
        Speed = 0; // n/s ---> kn/s
        totalMoves = 0;
        movesEvaluated = 0;
        totalMovesToEvaluate = 0;
    }
};

//Defined in EngineState.cpp

//Contains Data Related to the Engine
extern ChessEngineStatistics Heuristics;

#endif // !CHESS_ENGINE_STATISTICS_H

